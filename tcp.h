#ifndef TCP_H
#define TCP_H

#include "constant.h"

using namespace std;

bool create_TCP_server(int &server_socket, sockaddr_in &serverAddr, int port, string IP) {	

	// create server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "[-]Failed to create server socket.\n";
		return false;
	}

    // Bind the ip address and port to a socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(IP.c_str());
	
    if(bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		cout << "[-]Error in binding.\n";
		return false;
	}

	if(listen(server_socket, 10) != 0) {
		cout << "[-]Error in listening.\n";
        return false;
	}

	return true;
}

#endif 