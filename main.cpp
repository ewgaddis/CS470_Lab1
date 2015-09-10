#undef UNICODE

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <vector>
#include <conio.h>

using namespace std;

#define SERVER_PORT 51401

bool initialize(SOCKET);
int  readSocket(SOCKET, char *, int);
bool writeSocket(SOCKET, const char *);

#define BUFFER_SIZE 256

void process(SOCKET);
char *sendCommand(SOCKET, const char *);
int scanString(const char *, int, char *);

typedef struct sTeam
{
	char *color;
	int  numTanks;

	sTeam() : color(0), numTanks(0) {}
	~sTeam()
	{
		if(color)
		{
			delete[] color;
		}
	}
} Team;

typedef struct sObstacle
{
	float x1, y1;
	float x2, y2;
	float x3, y3;
	float x4, y4;

	sObstacle() : x1(0.0f), x2(0.0f), y1(0.0f), y2(0.0f),
				  x3(0.0f), x4(0.0f), y3(0.0f), y4(0.0f)
	{
	}
} Obstacle;

bool sendShoot(SOCKET, int);
bool sendSpeed(SOCKET, int, float);
bool sendAngVel(SOCKET, int, float);
bool sendTeam(SOCKET, vector<Team *> *);
bool sendObstacles(SOCKET, vector<Obstacle *> *);

int main(int argc, char **argv)
{
	int mainResult = 0;

	WSADATA wsaData;
	int error = WSAStartup(0x0202, &wsaData);

	if(error)
	{
		cout << "Failed to startup WSA" << endl;
		mainResult = 1;
	}
	else
	{
		cout << "Connecting..." << endl;

		SOCKADDR_IN target;
		memset(&target, 0, sizeof(SOCKADDR_IN));

		target.sin_family		= AF_INET;
		target.sin_port			= htons(SERVER_PORT);
		target.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);

		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(s == INVALID_SOCKET)
		{
			cout << "Failed to create socket" << endl;
			mainResult = 1;
		}
		else if(connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
		{
			closesocket(s);

			cout << "Failed to connect!" << endl;
			mainResult = 1;
		}
		else
		{
			cout << "Successfully connected!" << endl;

			if(initialize(s))
			{
				process(s);
			}

			closesocket(s);
		}

		WSACleanup();
	}

	cout << "Press any key to continue";
	while(!_kbhit());

	return mainResult;
}

bool initialize(SOCKET s)
{
	cout << "Getting initial response from BZRFlag..." << endl;

	char buffer[32];
	if(readSocket(s, buffer, 32) < 0)
	{
		return false;
	}

	if(strncmp(buffer, "bzrobots 1\n", 11))
	{
		cout << "Failed to get response \"bzrobots 1\" from BZRFlag" << endl;
		return false;
	}

	cout << "Received response from BZRFlag..." << endl;
	cout << "Sending a response back..." << endl;

	strcpy(buffer, "agent 1\n");

	return writeSocket(s, buffer);
}

int readSocket(SOCKET s, char *buffer, int numBytes)
{
	memset(buffer, 0, numBytes);

	int result = recv(s, buffer, numBytes - 1, 0);

	if(result == SOCKET_ERROR)
	{
		cout << "ERROR: Failed to read socket" << endl;
		return -1;
	}

	return result;
}

bool writeSocket(SOCKET s, const char *buffer)
{
	if(send(s, buffer, strlen(buffer), 0) > 0)
	{
		return true;
	}

	cout << "ERROR: Failed to write socket" << endl;
	return false;
}

void process(SOCKET s)
{
	sendShoot(s, 0);

	vector<Team *> teams;
	sendTeam(s, &teams);

	vector<Team *>::iterator itTeam = teams.begin();
	while(itTeam != teams.end())
	{
		cout << "Team: color = " << (*itTeam)->color << ", n = " << (*itTeam)->numTanks << endl;
		delete (*itTeam);
		++itTeam;
	}

	vector<Obstacle *> obstacles;
	sendObstacles(s, &obstacles);

	vector<Obstacle *>::iterator itObstacle = obstacles.begin();
	while(itObstacle != obstacles.end())
	{
		cout << "Obstacle:" << endl;
		cout << "  (" << (*itObstacle)->x1 << ", " << (*itObstacle)->y1 << ")" << endl;
		cout << "  (" << (*itObstacle)->x2 << ", " << (*itObstacle)->y2 << ")" << endl;
		cout << "  (" << (*itObstacle)->x3 << ", " << (*itObstacle)->y3 << ")" << endl;
		cout << "  (" << (*itObstacle)->x4 << ", " << (*itObstacle)->y4 << ")" << endl;

		delete (*itObstacle);
		++itObstacle;
	}

	sendSpeed(s, 0, 0.5);
	sendAngVel(s, 0, -1.0);
}

char *sendCommand(SOCKET s, const char *command)
{
	char buffer[BUFFER_SIZE + 1];

	if(!writeSocket(s, command))
	{
		return 0;
	}

	if(readSocket(s, buffer, BUFFER_SIZE + 1) < 0)
	{
		return 0;
	}

	if(strncmp(buffer, "ack", 3))
	{
		cout << "Server failed to acknowledge command" << endl;
		return 0;
	}

	int numBytes = readSocket(s, buffer, BUFFER_SIZE + 1);
	int numBytesRead = numBytes;

	char *response = new char[numBytes + 1];
	strcpy(response, buffer);

	while(numBytes == BUFFER_SIZE &&
		  strncmp(buffer + BUFFER_SIZE - 5, "\nend\n", 5))
	{
		numBytes = readSocket(s, buffer, BUFFER_SIZE + 1);
		numBytesRead += numBytes;

		if(numBytes > 0)
		{
			char *temp = response;

			response = new char[numBytesRead + 1];
			strcpy(response, temp);
			strcat(response, buffer);

			delete[] temp;
		}
	}

	return response;
}

int scanString(const char *string, int index, char *buffer)
{
	int startIndex = index;

	while(string[index] != ' ' && string[index] != '\n')
	{
		++index;
	}

	strncpy(buffer, string + startIndex, index - startIndex);
	buffer[index - startIndex] = 0;

	return index + 1;
}

bool sendShoot(SOCKET s, int agent)
{
	char buffer[16];
	sprintf(buffer, "shoot %i\n", agent);

	char *response = sendCommand(s, buffer);

	if(response == 0)
	{
		return false;
	}

	if(strncmp(response, "ok", 2))
	{
		return false;
	}

	delete[] response;

	return true;
}

bool sendSpeed(SOCKET s, int agent, float speed)
{
	char buffer[128];
	sprintf(buffer, "speed %i %f\n", agent, speed);

	char *response = sendCommand(s, buffer);

	if(response == 0)
	{
		return false;
	}

	if(strncmp(response, "ok", 2))
	{
		return false;
	}

	delete[] response;

	return true;
}

bool sendAngVel(SOCKET s, int agent, float angVel)
{
	char buffer[128];
	sprintf(buffer, "angvel %i %f\n", agent, angVel);

	char *response = sendCommand(s, buffer);

	if(response == 0)
	{
		return false;
	}

	if(strncmp(response, "ok", 2))
	{
		return false;
	}

	delete[] response;

	return true;
}

bool sendTeam(SOCKET s, vector<Team *> *teams)
{
	char buffer[256];
	strcpy(buffer, "teams\n");

	char *response = sendCommand(s, buffer);

	if(response == 0)
	{
		return false;
	}

	int index = scanString(response, 0, buffer);

	if(strcmp(buffer, "begin") == 0)
	{
		index = scanString(response, index, buffer);

		while(strcmp(buffer, "end"))
		{
			index = scanString(response, index, buffer);

			char *color = new char[strlen(buffer) + 1];
			strcpy(color, buffer);

			index = scanString(response, index, buffer);

			int numTanks = atoi(buffer);

			Team *team = new Team();
			team->color    = color;
			team->numTanks = numTanks;

			teams->push_back(team);

			index = scanString(response, index, buffer);
		}
	}

	delete[] response;

	return true;
}

bool sendObstacles(SOCKET s, vector<Obstacle *> *obstacles)
{
	char buffer[256];
	strcpy(buffer, "obstacles\n");

	char *response = sendCommand(s, buffer);

	if(response == 0)
	{
		return false;
	}

	int index = scanString(response, 0, buffer);

	if(strcmp(buffer, "begin") == 0)
	{
		index = scanString(response, index, buffer);

		while(strcmp(buffer, "end"))
		{
			Obstacle *obstacle = new Obstacle();

			index = scanString(response, index, buffer);
			obstacle->x1 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->y1 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->x2 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->y2 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->x3 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->y3 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->x4 = (float)atof(buffer);

			index = scanString(response, index, buffer);
			obstacle->y4 = (float)atof(buffer);

			obstacles->push_back(obstacle);

			index = scanString(response, index, buffer);
		}
	}

	delete[] response;

	return true;
}