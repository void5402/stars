#include <SFML/Graphics/PrimitiveType.hpp>
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
  // window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);

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

  // this could be refactored to a per triangle assignment but for nefarious
  // purpuses i will keep it
  std::vector<sf::Vertex> verts(n * 12);
  const sf::Color white = sf::Color::White;
  const sf::Color clear = sf::Color::Transparent;
  const sf::Color tmpl[12] = {
      clear, white, white, // tri 1
      clear, white, white, // tri 2
      clear, white, white, // tri 3
      clear, white, white  // tri 4
  };
  for (int i = 0; i < n; i++)
    for (int k = 0; k < 12; k++)
      verts[i * 12 + k].color = tmpl[k];

  // stuff for calculations
  const float maxradius = 50;
  const float minradius = 0;
  const float fov = 90;
  const float m = 1 / std::sin((fov / 2) * M_PI / 180.0); //this might need to be changed to tan
  const float r = (maxradius - minradius) / (m - maxz);
  const float t = maxradius - r * m;
  sf::Vector2f screensize = sf::Vector2f(window.getSize());
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
    for (int i = 0; i < n; i++) {
      star &s = cluster[i];
      s.z = (s.z > 25.f) ? s.z - 25.f : maxz;

      const float Minvz = (1.f / s.z) * m;
      const float tx = (s.x * Minvz + 1) * halfW;
      const float ty = (s.y * Minvz + 1) * halfH;
      const float tr = r * s.z + t;
      const float tr2 = tr * inv15;

      sf::Vertex *v = &verts[i * 12];
      v[0].position =  {tx - tr  , ty       };
      v[1].position =  {tx + tr2 , ty + tr2 };
      v[2].position =  {tx + tr2 , ty - tr2 };
      v[3].position =  {tx       , ty + tr  };
      v[4].position =  {tx + tr2 , ty - tr2 };
      v[5].position =  {tx - tr2 , ty - tr2 };
      v[6].position =  {tx + tr  , ty       };
      v[7].position =  {tx - tr2 , ty - tr2 };
      v[8].position =  {tx - tr2 , ty + tr2 };
      v[9].position =  {tx       , ty - tr  };
      v[10].position = {tx - tr2 , ty + tr2 };
      v[11].position = {tx + tr2 , ty + tr2 };
    }
    window.clear();
    window.draw(verts.data(), n * 12, sf::PrimitiveType::Triangles);
    window.display();
  }
}