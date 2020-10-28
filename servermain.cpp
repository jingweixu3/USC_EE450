#include "constant.h"
#include <stdio.h> 
#include <stdlib.h> 

using namespace std;

int main(){

    cout << "Main Server is up and running\n";

	int server_socket, bind_return;
	sockaddr_in serverAddr;

	int newSocket;
	sockaddr_in newAddr; 

	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

    // create server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cout << "Failed to create server socket.\n";
		return -1;
	}

    // Bind the ip address and port to a socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TCP_MAIN);
    inet_pton(AF_INET, "0.0.0.0", &serverAddr.sin_addr);

	bind_return = bind(server_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	
    if(bind_return < 0) {
		cout << "Error in binding.\n";
		return -1;
	}

	if(listen(server_socket, 10) != 0) {
		cout << "Error in listening.\n";
        return -1;
	}

    memset(&buffer, '\0', sizeof(buffer));

	while(1) {
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";


		// child process 
		if((childpid = fork()) == 0) {
			close(server_socket);

			while(1) {
				if (recv(newSocket, buffer, 1024, 0) == 0){
					continue;
				}
				
				printf("Client: %s\n", buffer);
				send(newSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));	
			}
		}
	}

	close(newSocket);
	close(server_socket);

	return 0;
}