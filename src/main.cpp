#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/TimeoutWithPredicate.hpp>
#include <cmath>
#include <random>
#include <vector>
#define _USE_MATH_DEFINES 
//future: use vertex idea by chatgpt and claude

// future: group x/y/z cordinates into their own vector and do x[i]
struct star {
  double x, y, z;
};

int main() {
  // init sfml stuff
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "???");
  sf::CircleShape shape(1.f);
  window.setFramerateLimit(60); 
  // sf::Texture image(std::filesystem::absolute("../../image.png"));
  // shape.setTexture(&image);
  sf::Clock clock;
  sf::Time time;
  sf::Vector2f screensize = sf::Vector2f(window.getSize());

  // init stars
  const int n = 400;
  const double maxz = 2000;
  std::vector<star> cluster(n);
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> x(-1000, 1000);
  std::uniform_real_distribution<double> y(-1000, 1000);
  std::uniform_real_distribution<double> z(0, maxz);
  for (int i = cluster.size(); i != 0; i--) {
    cluster[i - 1].x = x(rng);
    cluster[i - 1].y = y(rng);
    cluster[i - 1].z = z(rng);
  }

  // stuff for calculations
  const double maxradius = 50;
  const double minradius = 0;
  const double fov = 90;
  const double m = 1 / std::sin((fov / 2)*  M_PI / 180.0);
  const double r = (maxradius - minradius) / (m - maxz);
  const double t = maxradius - r * m;
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
    window.clear();
    for (int i = cluster.size(); i != 0; i--) {
      if (cluster[i - 1].z >= 0) {
        cluster[i - 1].z--;
      } else {
        cluster[i - 1].z = maxz;
      }

      shape.setRadius(static_cast<float>(r * cluster[i - 1].z + t));
      // i think there is a bug with my fov but
      // i dont even know what fov means i have to do more ressearch
      shape.setPosition(sf::Vector2f(
          ((cluster[i - 1].x / cluster[i - 1].z) * m + 1) * screensize.x / 2 -
              shape.getRadius(),
          ((cluster[i - 1].y / cluster[i - 1].z) * m + 1) * screensize.y / 2 -
              shape.getRadius()));
      window.draw(shape);
    }
    window.display();
  }
}
