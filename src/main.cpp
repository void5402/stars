#include <SFML/Graphics.hpp>
// plan:
// 1.generate stars
// 2.place them randomly
// 3.make them move and resize correctly
// 4.them returning randomly
// 5.profit?
//
//

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "???");
	sf::CircleShape shape(100.f);
	
	sf::Texture image(std::filesystem::absolute("../../image.png"));
	shape.setTexture(&image);
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			if (const auto *key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->scancode == sf::Keyboard::Scan::Q)
				{
					window.close();
				}
			}
		}
		window.clear();
		shape.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)-sf::Vector2i(shape.getRadius(),shape.getRadius())));
		window.draw(shape);
		window.display();
	}
}
