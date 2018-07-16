#include <iostream>
#include <SFML\Network.hpp>

using namespace std;
using namespace sf;



struct User
{

	TcpSocket* sock;
	string user;
};
int main()
{
	cout << "Server Running" << endl;
	TcpListener listener;
	SocketSelector selector;
	bool done = false;
	vector<User>clients;

	vector<string> mids;
	vector<string> tops;
	vector<string> bots;
	vector<string> jungles;
	vector<string> supports;

	listener.getLocalPort();
	listener.listen(2000);
	selector.add(listener);

	while (true)
	{
		if (tops.size() > 1 && jungles.size() > 1 && mids.size() > 1 && bots.size() > 1 && supports.size() > 1)
		{
			std::random_shuffle(tops.begin(), tops.end());
			std::random_shuffle(jungles.begin(), jungles.end());
			std::random_shuffle(mids.begin(), mids.end());
			std::random_shuffle(bots.begin(), bots.end());
			std::random_shuffle(supports.begin(), supports.end());
			vector<string> red;
			vector<string> blue;
			red.push_back(tops.at(0));
			blue.push_back(tops.at(1));
			red.push_back(jungles.at(0));
			blue.push_back(jungles.at(1));
			red.push_back(mids.at(0));
			blue.push_back(mids.at(1));
			red.push_back(bots.at(0));
			blue.push_back(bots.at(1));
			red.push_back(supports.at(0));
			blue.push_back(supports.at(1));

			string s = "Create a game with: \n" + red[0] + ", " + red[1] + ", " + red[2] + ", " + red[3] + ", " + red[4] + " on the red team \n" + blue[0] + ", " + blue[1] + ", " + blue[2] + ", " + blue[3] + ", " + blue[4] + " on the blue team.";


			for (int i = 0; i < 5; i++)
			{
				User r;
				User b;
				for (int j = 0; j < clients.size(); j++)
				{
					if (clients[j].user == red[i] || clients[j].user == blue[i])
					{
						if (clients[j].user == red[i])
							r = clients[j];
						else
							b = clients[j];
					}
				}
				Packet p;
				p << "game" << s;
				cout << "telling " << r.user << " and " << b.user << endl;
				r.sock->send(p);
				b.sock->send(p);
				for (int j = 0; j < clients.size(); j++)
				{
					if (clients[j].user == r.user || clients[j].user == b.user)
					{
						clients.erase(clients.begin() + j);
						j--;
					}
				}
			}

			tops.erase(tops.begin(), tops.begin() + 1);
			jungles.erase(jungles.begin(), jungles.begin() + 1);
			mids.erase(mids.begin(), mids.begin() + 1);
			bots.erase(bots.begin(), bots.begin() + 1);
			supports.erase(supports.begin(), supports.begin() + 1);
			cout << clients.size() << endl;
		}
		if (selector.wait())
		{

			if (selector.isReady(listener))
			{
				TcpSocket *socket = new TcpSocket;

				listener.accept(*socket);
				Packet packet;
				string id;
				int role;
				string status;
				if (socket->receive(packet) == Socket::Done)
					packet >> id >> role >> status;
				cout << id << " has " << status << " with role " << role << endl;
				User u;
				u.sock = socket;
				u.user = id;
				clients.push_back(u);
				selector.add(*socket);
				switch (role)
				{
					case 0:
						tops.push_back(id);
						break;
					case 1:
						jungles.push_back(id);
						break;
					case 2:
						mids.push_back(id);
						break;
					case 3:
						bots.push_back(id);
						break;
					case 4:
						supports.push_back(id);
						break;
					default:
						break;
				}
			}
			/*else
			{
				for (int i = 0; i < clients.size(); i++)
				{
					if (selector.isReady(*clients[i]))
					{
						Packet packet, sendpacket;
						if (clients[i]->receive(packet) == Socket::Done)
						{
							string text;
							packet >> text;
							sendpacket << text;
							for (int j = 0; j < clients.size(); j++)
							{
								if (i != j)
								{
									clients[j]->send(sendpacket);
								}
							}
						}
					}
				}
			}*/
		}
		for (int l = 0; l < clients.size(); l++)
		{
			if (clients[l].sock->getRemoteAddress() == IpAddress::None)
			{
				clients.erase(clients.begin() + l);
				cout << clients[l].user << " has disconnected!" << endl;
			}
		}
	}

	for (vector<User>::iterator it = clients.begin(); it != clients.end(); it++)
		delete (it)->sock;
}