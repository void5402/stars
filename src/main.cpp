#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <vector>

struct StarCluster {
  std::vector<float> x, y, z;

  StarCluster(int n) : x(n), y(n), z(n) {}
};

int main() {
  // init sfml stuff
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "???",
                          sf::Style::Default, sf::State::Windowed, settings);
  window.setVerticalSyncEnabled(true);

  // init stars
  const int n = 1000;
  const float maxz = 20000.f;
  StarCluster cluster(n);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<float> xy(-maxz / 2.f, maxz / 2.f);
  std::uniform_real_distribution<float> z(1.f, maxz);

  for (int i = 0; i < n; i++) {
    cluster.x[i] = xy(rng);
    cluster.y[i] = xy(rng);
    cluster.z[i] = z(rng);
  }

  // vertex buffer
  std::vector<sf::Vertex> verts(n * 12);
  const sf::Color white = sf::Color::White;
  const sf::Color clear = sf::Color::Transparent;
  const sf::Color tmpl[12] = {
      clear, white, white, // tri 1
      clear, white, white, // tri 2
      clear, white, white, // tri 3
      clear, white, white  // tri 4
  };
  for (int i = 0; i < n * 12; i++) {
    verts[i].color = tmpl[i % 12];
  }

  // pre-computed vertex offsets (in units of tr and tr*inv15)
  // format: {x_multiplier, y_multiplier, y_uses_inv15}
  struct Offset {
    float x, y;
  };
  const Offset offsets[12] = {
      {-1.f, 0.f},                // tri 1, v0
      {1.f / 15.f, 1.f / 15.f},   // tri 1, v1
      {1.f / 15.f, -1.f / 15.f},  // tri 1, v2
      {0.f, 1.f},                 // tri 2, v3
      {1.f / 15.f, -1.f / 15.f},  // tri 2, v4
      {-1.f / 15.f, -1.f / 15.f}, // tri 2, v5
      {1.f, 0.f},                 // tri 3, v6
      {-1.f / 15.f, -1.f / 15.f}, // tri 3, v7
      {-1.f / 15.f, 1.f / 15.f},  // tri 3, v8
      {0.f, -1.f},                // tri 4, v9
      {-1.f / 15.f, 1.f / 15.f},  // tri 4, v10
      {1.f / 15.f, 1.f / 15.f}    // tri 4, v11
  };

  // pre-compute projection constants
  const float FOV_RAD = 90.f * M_PI / 180.f;
  const float M =
      1.f / std::tan(FOV_RAD * 0.5f); // KIDS ALWAYS CHECK YOUR NOTES TWISE
  const float maxradius = 50.f;
  const float minradius = 0.f;
  const float R = (maxradius - minradius) / (M - maxz);
  const float T = maxradius - R * M;
  const float Z_WRAP = 25.f;

  sf::Vector2f screensize = sf::Vector2f(window.getSize());
  const float halfW = screensize.x * 0.5f;
  const float halfH = screensize.y * 0.5f;

  while (window.isOpen()) {
    // event handler
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scan::Q) {
          window.close();
        }
      }
    }

    // update and render stars
    for (int i = 0; i < n; i++) {
      float &star_z = cluster.z[i];
      star_z = (star_z > Z_WRAP) ? star_z - Z_WRAP : maxz;

      const float inv_z = M / star_z;
      const float tx = (cluster.x[i] * inv_z + 1.f) * halfW;
      const float ty = (cluster.y[i] * inv_z + 1.f) * halfH;
      const float tr = R * star_z + T;

      sf::Vertex *v = &verts[i * 12];
      for (int k = 0; k < 12; k++) {
        v[k].position = {tx + offsets[k].x * tr, ty + offsets[k].y * tr};
      }
    }

    window.clear();
    window.draw(verts.data(), n * 12, sf::PrimitiveType::Triangles);
    window.display();
  }

  return 0;
}
