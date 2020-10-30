#ifndef UDP_H
#define UDP_H

#include "constant.h"

using namespace std;

void create_UDP(int &sockfd_UDP, sockaddr_in &serverAddr_UDP, int port, string ip) {
	
	// create udp 
	sockfd_UDP = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr_UDP, '\0', sizeof(serverAddr_UDP));

    serverAddr_UDP.sin_family = AF_INET;
    serverAddr_UDP.sin_port = htons(port);
    serverAddr_UDP.sin_addr.s_addr = inet_addr(ip.c_str());
}

string UDP_send_receive(int &sockfd_UDP, sockaddr_in &serverAddr_UDP, string message) {

	sendto(sockfd_UDP, message.c_str(), message.size() + 1, 0, (struct sockaddr*)&serverAddr_UDP, sizeof(serverAddr_UDP));
	// cout << "[+]Sending: " << message << endl;

    socklen_t  serverAddr_UDP_length = sizeof(serverAddr_UDP);

	char buffer[BUFFER_LENGTH];
	memset(&buffer, '\0', sizeof(buffer));
	
	recvfrom(sockfd_UDP, buffer, BUFFER_LENGTH, 0, (sockaddr*) &serverAddr_UDP, &serverAddr_UDP_length);
	// cout << "[+]Receiving: " << buffer << endl;

	return string(buffer);
}



#endif 