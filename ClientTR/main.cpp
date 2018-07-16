#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>
#include <TGUI/TGUI.hpp>
#include<iostream>
#include <fstream>
#include<string>
#include<vector>
#include<ctime>
using namespace sf;
using namespace std;

RenderWindow wind(VideoMode(450, 300, 32), "TR Queue Client");
tgui::Gui gui{ wind };
TcpSocket socket;
bool connright = false;
double d1 = 0;
void networkingHandler()
{
	while (true)
	{
		if (connright)
		{
			Packet packet;
			socket.receive(packet);
			string id;
			string red;
			string blue;
			if (packet >> id >> red >> blue)
			{
				tgui::Label::Ptr r = tgui::Label::create("Red:");
				tgui::Label::Ptr rppl = tgui::Label::create(red);
				tgui::Label::Ptr b = tgui::Label::create("Blue:");
				tgui::Label::Ptr bppl = tgui::Label::create(blue);
				r->setPosition(100, 175);
				rppl->setPosition(140, 175);
				b->setPosition(100, 200);
				bppl->setPosition(140, 200);
				gui.add(r);
				gui.add(b);
				gui.add(rppl);
				gui.add(bppl);
				gui.get<tgui::Label>("isqueued")->setText("Game Found!");
			}
		}
	}
}
void connect()
{
	std::string name = gui.get<tgui::EditBox>("edit")->getText();
	std::string role = gui.get<tgui::ComboBox>("combo")->getSelectedItem();
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
	bool done = false;
	int roleid;
	string status = "connected";
	string text = "";

	bool b = false;
	if (role == "top" || role == "Top")
	{
		roleid = 0;
		b = true;
	}
	else if (role == "jungle" || role == "Jungle")
	{
		roleid = 1;
		b = true;
	}
	else if (role == "mid" || role == "Mid")
	{
		roleid = 2;
		b = true;
	}
	else if (role == "bot" || role == "Bot")
	{
		roleid = 3;
		b = true;
	}
	else if (role == "support" || role == "Support")
	{
		roleid = 4;
		b = true;
	}
	if (b)
	{
		socket.connect(ip, 2000);
		Packet packet;

		packet << name << roleid << status;
		socket.send(packet);
		socket.setBlocking(false);
		connright = true;
		gui.get<tgui::Label>("isqueued")->setVisible(true);
		gui.draw();
	}
	else
	{
		tgui::Label::Ptr p = gui.get<tgui::Label>("isqueued");
		p->setText("Failed to queue up!");
		p->setVisible(true);
		gui.draw();

	}
}

void WinMain()
{
	sf::Thread t(&networkingHandler);
	t.launch();
	tgui::Label::Ptr lsumm = tgui::Label::create("Summoner Name");
	tgui::Label::Ptr lrole = tgui::Label::create("Role");
	tgui::Label::Ptr failed = tgui::Label::create("Queued up!");
	tgui::Button::Ptr bqueue = tgui::Button::create("Queue Up");
	tgui::EditBox::Ptr ebsumm = tgui::EditBox::create();
	tgui::ComboBox::Ptr cbrole = tgui::ComboBox::create();
	lrole->setPosition(100, 75);
	cbrole->setPosition(150, 75);
	cbrole->addItem("Top", "top");
	cbrole->addItem("Jungle", "jungle");
	cbrole->addItem("Mid", "mid");
	cbrole->addItem("Bot", "bot");
	cbrole->addItem("Support", "support");
	lsumm->setPosition(25, 25);
	ebsumm->setPosition(150, 25);

	bqueue->setPosition(175, 150);
	failed->setPosition(175, 125);
	failed->setVisible(false);
	bqueue->connect("pressed", &connect);
	gui.add(bqueue);
	gui.add(ebsumm, "edit");
	gui.add(cbrole, "combo");
	gui.add(lsumm);
	gui.add(lrole);
	gui.add(failed, "isqueued");
	
	wind.setActive(true);
	while (wind.isOpen())
	{
		Event event;
		while (wind.pollEvent(event))
		{
			gui.handleEvent(event);
			switch (event.type)
			{
			case Event::Closed:
				wind.close();
				exit(0);
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{
				}
				else if (event.key.code == Keyboard::Return)
				{
				}
				break;
			}
		}
		wind.clear(sf::Color::White);
		gui.draw();
		wind.display();
	}


}
