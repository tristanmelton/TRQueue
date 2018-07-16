#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include <fstream>
#include<string>
#include<vector>
#include<conio.h>

using namespace sf;
using namespace std;

void main()
{

	IpAddress ip;
	ifstream options;
	options.open("options.cfg");
	if (options.is_open())
	{
		string line = "";
		while (getline(options, line))
		{
			ip = IpAddress(line);
		}
		if (line == "")
			ip = IpAddress::getLocalAddress();
	}
	else
		cout << "Problem opening file!" << endl;
	TcpSocket socket;
	bool done = false;
	cout << ip.toString() << endl;
	string id;
	string role;
	int roleid;
	string status = "connected";
	string text = "";

	cout << "Enter summoner name: ";
	cin >> id;
	cout << endl;
	bool b = false;
	while (!b)
	{
		cout << "Enter role: ";
		cin >> role;
		cout << endl;

		if (role == "top")
		{
			roleid = 0;
			b = true;
		}
		else if (role == "jungle")
		{
			roleid = 1;
			b = true;
		}
		else if (role == "mid")
		{
			roleid = 2;
			b = true;
		}
		else if (role == "bot")
		{
			roleid = 3;
			b = true;
		}
		else if (role == "support")
		{
			roleid = 4;
			b = true;
		}
		else
		{
			std::cout << "Please input a valid role (top, jungle, mid, bot, support)." << std::endl;
		}
	}
	socket.connect(ip, 2000);

	Packet packet;

	packet << id << roleid << status;
	socket.send(packet);
	socket.setBlocking(false);
	while (true)
	{
		Packet packet;
		socket.receive(packet);
		string id;
		string text;

		if (packet >> id >> text)
		{
			cout << text << endl;
			cout << "Please close your queue client!! " << endl;
		}

	}

	
	/*RenderWindow window(VideoMode(800, 600, 32), id);
	vector<Text> chat;

	Font font;
	font.loadFromFile("Data/font.ttf");

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{
					text.clear();
				}
				else if (event.key.code == Keyboard::Return)
				{
					Packet packet;
					packet << id + ":" + text;
					socket.send(packet);
					Text displaytext(text, font, 20);
					displaytext.setFillColor(Color::Green);
					chat.push_back(displaytext);
					text = "";
				}
				break;
			case Event::TextEntered:
				if (event.text.unicode == '\b' && text.size() >0)
				{
					text.erase(text.size() - 1, 1);
				}
				else
					text += event.text.unicode;
				break;
			}
		}

		Packet packet;
		socket.receive(packet);
		string temptext;
		if (packet >> temptext)
		{
			Text displaytext(temptext, font, 20);
			displaytext.setFillColor(Color::Blue);
			chat.push_back(displaytext);
		}
		int i = 0;
		for (i; i<chat.size(); i++)
		{
			chat[i].setPosition(0, i * 20);
			window.draw(chat[i]);
		}

		Text drawtext(text, font, 20);
		drawtext.setFillColor(Color::Red);
		drawtext.setPosition(0, i * 20);
		window.draw(drawtext);

		window.display();
		window.clear();
	}*/

}
