#pragma once

#ifndef _IRC_H

#include <WinSock2.h>
#include <queue>
#include <stdio.h>
#include <WS2tcpip.h>
#include <string>
#include "xor.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class IRC
{
private:
	SOCKET ConnectSocket;
	bool run = true;
public:
	IRC()
	{
	}
	~IRC()
	{
		run = false;
		WSACleanup();
	}
	void Send(std::string msg)
	{
		std::string temp = XorStr("PRIVMSG #kebscs :") + msg + "\r\n";
		send(ConnectSocket, temp.c_str(), temp.size(), NULL);
	}
	void Run()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			WSACleanup();
		//SOCKET ConnectSocket;
		addrinfo* result;
		if (getaddrinfo(XorStr("irc.twitch.tv"), XorStr("6667"), NULL, &result) != 0)
			WSACleanup();
		if (result)
		{
			ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen) != 0)
				WSACleanup();
			char recvbuf[100];
			send(ConnectSocket, XorStr("PASS oauth:lckk8rregkgsvswy24thnmz1500d9p\r\n"), strlen(XorStr("PASS oauth:lckk8rregkgsvswy24thnmz1500d9p\r\n")), NULL);
			send(ConnectSocket, XorStr("NICK talontochallenger\r\n"), strlen(XorStr("NICK talontochallenger\r\n")), NULL);
			send(ConnectSocket, XorStr("JOIN #kebscs\r\n"), strlen(XorStr("JOIN #kebscs\r\n")), NULL);
			bool Loading = true;

			while (run)
			{
				int recvbytes = 0;
				ZeroMemory(&recvbuf, 100);
				recvbytes = recv(ConnectSocket, recvbuf, 100, 0);
				if (recvbytes < 0)
				{
					run = false;
					M.ExitBot = true;
					MessageBoxA(0, XorStr("Failed to connect"), 0, 0);
				}
				std::string result = std::string(recvbuf);

				if (Loading)
				{
					if (result.find(XorStr("End of /NAMES list")) != std::string::npos)
						Loading = false;
				}
				else
				{
					if (result.find("PING") != std::string::npos)
						send(ConnectSocket, XorStr("PONG tmi.twitch.tv\r\n"), strlen(XorStr("PONG tmi.twitch.tv\r\n")), NULL);
					std::string temp = result.substr(1);
					size_t pos1 = temp.find("!");
					size_t pos2 = temp.find(":");
					if (pos1 != std::string::npos && pos2 != std::string::npos)
					{
						std::string user = temp.substr(0, pos1);
						std::string message = temp.substr(pos2 + 1, temp.size());
						if (!message.empty() && user == XorStr("kebscs"))
						{
							M.ServerInfo = message;
							M.StartTime = GetTickCount();
						}
					}
				}
			}
		}
	}
};

extern IRC* irc;
IRC* irc = new IRC();

#endif // !_IRC_H
