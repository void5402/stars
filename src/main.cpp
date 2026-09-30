#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
// plan:
// 1.generate stars
// 2.place them randomly
// 3.make them move and resize correctly
// 4.them returning randomly
// 5.profit?

//future: group x/y/z cordinates into their own vector and do x[i] 
struct star
{
	int8_t x,y,z;
};

int main()
{
	//init sfml stuff
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "???");
	sf::CircleShape shape(100.f);
	sf::Texture image(std::filesystem::absolute("../../image.png"));
	shape.setTexture(&image);

	//init stars
	int n = 20;
	std::vector<star> cluster(n);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> numb(INT8_MIN, INT8_MAX);
	for(int i = cluster.size(); i != 0; i--)
	{
		cluster[i-1].x = static_cast<int8_t>(numb(rng));
		cluster[i-1].y = static_cast<int8_t>(numb(rng));
		cluster[i-1].z = static_cast<int8_t>(numb(rng));
	}

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent()) //event handler
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
		
		//for()


		sf::Vector2u a = window.getSize();
		window.clear();
		for(int i = cluster.size(); i != 0; i--){
		shape.setPosition(sf::Vector2f(1.-shape.getRadius(),1.-shape.getRadius()));
		window.draw(shape);
		}
		window.display();
	}
}
