#include <SFML/Graphics.hpp>
#include <time.h>

class Gracz
{
private:
	sf::Texture tex;
	sf::Sprite sprite;
	sf::RectangleShape kolizja;
	float grawitacja;
	float predkosc;
public:
	Gracz(float x = 0, float y = 0)
	{
		this->tex.loadFromFile("resources/ptak.png");
		this->sprite.setTexture(tex);
		this->sprite.setScale(sf::Vector2f(4, 4));
		this->sprite.setPosition(sf::Vector2f(x, y));
		this->sprite.setOrigin(sf::Vector2f(this->sprite.getGlobalBounds().width / 8, this->sprite.getGlobalBounds().height / 8));

		this->kolizja.setSize(sf::Vector2f(13, 10));
		this->kolizja.setScale(sf::Vector2f(4, 4));

		this->grawitacja = 0;
		this->predkosc = grawitacja;
	}
	void aktualizuj(bool przycikWcisniety)
	{
		this->kolizja.setPosition(this->sprite.getPosition()-sf::Vector2f(28, 15));
		this->sprite.setRotation(predkosc * 9);
		this->sprite.move(0, predkosc);
		predkosc += grawitacja;
		if(grawitacja < 0.75)
			grawitacja += 0.0625;
		if(przycikWcisniety)
			predkosc = -3.5;
	}
	bool kolidujeZ(sf::Sprite obiekt)
	{
		if (this->kolizja.getGlobalBounds().intersects(obiekt.getGlobalBounds()))
			return true;
		else
			return false;
	}
	bool kolidujeZ(sf::RectangleShape obiekt)
	{
		if (this->kolizja.getGlobalBounds().intersects(obiekt.getGlobalBounds()))
			return true;
		else
			return false;
	}
	sf::Sprite podajSprite()
	{
		return this->sprite;
	}
};

class Tlo
{
private:
	sf::Texture tex;
	sf::Sprite trawa;
	sf::Sprite niebo;
public:
	Tlo()
	{
		this->tex.loadFromFile("resources/tlo.png");
		this->trawa.setTexture(tex);
		this->trawa.setTextureRect(sf::IntRect(0, 575, 2048, 145));
		this->trawa.setPosition(sf::Vector2f(0, 575));
		this->niebo.setTexture(tex);
		this->niebo.setTextureRect(sf::IntRect(0, 0, 2048, 575));
	}
	sf::Sprite podajTrawe()
	{
		return this->trawa;
	}
	sf::Sprite podajNiebo()
	{
		return this->niebo;
	}
};

class Rura
{
private:
	sf::Texture tex;
	sf::Sprite sprite;
	sf::RectangleShape punkt;
	float predkosc;
	bool naOdwrot;
public:
	bool dotknietoPunktu;
	Rura(float x, float y, bool naOdwrot = false)
	{
		this->dotknietoPunktu = false;
		this->predkosc = 2;
		this->tex.loadFromFile("resources/rura.png");
		this->sprite.setTexture(tex);
		this->sprite.setPosition(sf::Vector2f(x, y));
		this->punkt.setSize(sf::Vector2f(100, 150));
		this->naOdwrot = naOdwrot;
		if(naOdwrot)
		{
			this->sprite.setRotation(180);
			this->sprite.setPosition(sf::Vector2f(x + this->sprite.getGlobalBounds().width, y));
		}
	}
	void aktualizuj()
	{
		this->sprite.move(sf::Vector2f(-predkosc, 0));
		this->punkt.setPosition(this->sprite.getPosition() + sf::Vector2f(0, -150));
	}
	void ustawPozycje(float x, float y)
	{
		this->sprite.setPosition(sf::Vector2f(x, y));
	}
	bool pozaEkranem()
	{
		if(this->sprite.getPosition().x + this->sprite.getGlobalBounds().width < 0)
			return true;
		else
			return false;
	}
	bool czyOdwrotne()
	{
		return this->naOdwrot;
	}
	sf::Sprite podajSprite()
	{
		return this->sprite;
	}
	sf::RectangleShape podajPunkt()
	{
		return this->punkt;
	}
};

int main()
{
	bool graZaczeta = false;
	std::srand(time(NULL));
	sf::RenderWindow okno(sf::VideoMode(1024, 720), "Flappy Bird");
	okno.setFramerateLimit(60);
	poczatek:
	Gracz gracz(250, 350);
	Tlo tlo;

	std::vector<Rura> rury;
	float los = std::rand() % 325 + 150;
	Rura r1(1024, los);
	Rura r2(1024, los - 150, true);
	los = std::rand() % 325 + 150;
	Rura r3(1536, los);
	Rura r4(1536, los - 150, true);
	rury.push_back(r1);
	rury.push_back(r2);
	rury.push_back(r3);
	rury.push_back(r4);

	bool przycisk = false;
	sf::Clock zegar;

	int punkty = 0;
	bool dotykaPunktu = false;
	sf::Text tekstEkran;
	sf::Font czcionka;
	czcionka.loadFromFile("resources/Roboto-Regular.ttf");
	tekstEkran.setFont(czcionka);
	tekstEkran.setString(std::to_string(punkty));
	tekstEkran.setPosition(sf::Vector2f((okno.getSize().x - tekstEkran.getGlobalBounds().width) / 2, 10));
	bool pierwszyRaz = false;
	while(okno.isOpen())
	{
		sf::Event zdarzenie;
		while(okno.pollEvent(zdarzenie))
		{
			if(zdarzenie.type == sf::Event::Closed)
				okno.close();
			if(zdarzenie.type == sf::Event::KeyPressed)
			{
				if(zdarzenie.key.code == sf::Keyboard::Space)
				{
					przycisk = true;
					zegar.restart();
				}
				if(!pierwszyRaz)
				{
					if (zdarzenie.key.code == sf::Keyboard::Enter)
					{
						graZaczeta = true;
						pierwszyRaz = true;
					}
				}
				else
					goto poczatek;
			}
		}
		if (graZaczeta)
		{
			tekstEkran.setString(std::to_string(punkty));
			if(przycisk)
			{
				if(zegar.getElapsedTime().asSeconds() > 0.25)
				{
					przycisk = false;
					zegar.restart();
				}
			}
			gracz.aktualizuj(przycisk);
			los = std::rand() % 325 + 150;
			for(int i = 0; i != rury.size(); i++)
			{
				rury[i].aktualizuj();
				int j = i - 1;
				if(rury[i].pozaEkranem())
				{
					if(rury[i].czyOdwrotne())
						rury[i].ustawPozycje(1024, rury[j].podajSprite().getPosition().y - 150);
					else
						rury[i].ustawPozycje(1024, los);
				}
				if(gracz.kolidujeZ(rury[i].podajPunkt()))
				{
					if (!rury[i].dotknietoPunktu)
					{
						punkty++;
						rury[i].dotknietoPunktu = true;
					}
				}
				else
					rury[i].dotknietoPunktu = false;
				if(gracz.kolidujeZ(rury[i].podajSprite()))
					graZaczeta = false;
				if(gracz.kolidujeZ(tlo.podajTrawe()))
					graZaczeta = false;
			}
		}
		
		okno.clear();
		okno.draw(tlo.podajNiebo());
		for (int i = 0; i != rury.size(); i++)
			okno.draw(rury[i].podajSprite());
		okno.draw(tlo.podajTrawe());
		okno.draw(gracz.podajSprite());
		okno.draw(tekstEkran);
		okno.display();
	}

	return 0;
}