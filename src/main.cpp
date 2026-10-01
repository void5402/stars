#include <SFML/Window/WindowEnums.hpp>
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/TimeoutWithPredicate.hpp>
#include <cmath>
#include <random>
#include <vector>

struct star {
  float x, y, z;
};

int main() {
  // init sfml stuff
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "???",
                          sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(60);
  sf::Vector2f screensize = sf::Vector2f(window.getSize());

  // init stars
  const int n = 1000;
  const float maxz = 20000;
  std::vector<star> cluster(n);
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<float> xy(-maxz / 2, maxz / 2);
  std::uniform_real_distribution<float> z(1, maxz);
  for (int i = n; i != 0; i--) {
    cluster[i - 1].x = xy(rng);
    cluster[i - 1].y = xy(rng);
    cluster[i - 1].z = z(rng);
  }
  sf::VertexArray tris(sf::PrimitiveType::Triangles);
  tris.resize(n * 12); // 4 triangles per star

  // stuff for calculations
  const float maxradius = 50;
  const float minradius = 0;
  const float fov = 90;
  const float m = 1 / std::sin((fov / 2) * M_PI / 180.0);
  const float r = (maxradius - minradius) / (m - maxz);
  const float t = maxradius - r * m;
  const float halfW = screensize.x * 0.5f;
  const float halfH = screensize.y * 0.5f;
  const float inv15 = 1.f / 15.f;
  int v = 0;
  while (window.isOpen()) {

    // event handler
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scan::Q) {
          window.close();
        }
      }
    }

    // stuff
    v = 0;
    for (int i = n; i != 0; i--) {
      star &s = cluster[i - 1];
      if (s.z > 55) {
        s.z -= 25;
      } else {
        s.z = maxz;
      }

      const float Minvz = (1.f / s.z) * m;
      const float tx = (s.x * Minvz + 1) * halfW;
      const float ty = (s.y * Minvz + 1) * halfH;
      const float tr = r * s.z + t;
      const float tr2 = tr * inv15;

      tris[v++] = {{tx - tr, ty}, sf::Color::Transparent};
      tris[v++] = {{tx + tr2, ty + tr2}, sf::Color::White};
      tris[v++] = {{tx + tr2, ty - tr2}, sf::Color::White};
      tris[v++] = {{tx, ty + tr}, sf::Color::Transparent};
      tris[v++] = {{tx + tr2, ty - tr2}, sf::Color::White};
      tris[v++] = {{tx - tr2, ty - tr2}, sf::Color::White};
      tris[v++] = {{tx + tr, ty}, sf::Color::Transparent};
      tris[v++] = {{tx - tr2, ty - tr2}, sf::Color::White};
      tris[v++] = {{tx - tr2, ty + tr2}, sf::Color::White};
      tris[v++] = {{tx, ty - tr}, sf::Color::Transparent};
      tris[v++] = {{tx - tr2, ty + tr2}, sf::Color::White};
      tris[v++] = {{tx + tr2, ty + tr2}, sf::Color::White};
    }
    window.clear();
    window.draw(tris);
    window.display();
  }
}