#include "constant.h"
#include "tcp.h"
#include "udp.h"
#include "country.h"

using namespace std;

int main(){

    cout << "Main Server is up and running\n";

	// initialize udp
    int sockfd_UDP;
    sockaddr_in A_serverAddr_UDP;

    create_UDP(sockfd_UDP, A_serverAddr_UDP, UDP_SERVER_A, LOCAL_IP);

	// send country request 
	string countryList = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	cout << "[+] country list: " << countryList << endl<<endl;
	unordered_set<string> country_set = convert_string_to_set(countryList);
	
	// initialize tcp server
	int server_socket, bind_return;
	sockaddr_in serverAddr;

	if (create_TCP_server(server_socket, serverAddr, TCP_MAIN, LOCAL_IP) == false) {
		return -1;
	}

	while(1) {
		int newSocket;
		sockaddr_in newAddr; 

		socklen_t addr_size;
		pid_t childpid;

		//accept request
		newSocket = accept(server_socket, (sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0) {
			return -1;
		}
		cout << "Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";

		// child process 
		if((childpid = fork()) == 0) {
			close(server_socket);

			while(1) {

				// TCP receive from client
				char buffer[BUFFER_LENGTH];
    			memset(&buffer, '\0', sizeof(buffer));

				if (recv(newSocket, buffer, BUFFER_LENGTH, 0) == 0){
					continue;
				}
				
				cout << "[+]Data from Client: " << buffer << endl;

				// split message
				vector<string> message_list = convert_string_to_vector(string(buffer));
				cout << message_list.size() << endl;
				if (message_list.size() != 2) {
					cout << "[-]Error in message from client." << endl;
					continue;
				}

				int userID = stoi(message_list[0]);
				string country_name = message_list[1];

				// check country existed or not, and do something
				if (country_set.find(country_name) == country_set.end()) {
					
					// send back to client
					string reply = "<" + country_name + "> " + NO_COUNTRY_FOUND;
					send(newSocket, reply.c_str(),reply.size() + 1, 0);
					cout << "[+]Data send to Client: " <<  reply << endl;
					continue;
				}

				// sending info to UDP

				string reply = UDP_send_receive(sockfd_UDP, A_serverAddr_UDP, buffer);

				// send back to client
				send(newSocket, reply.c_str(),reply.size() + 1, 0);
				cout << "[+]Data send to Client: " <<  reply << endl;

			}
		}
		close(newSocket);

	}

	close(sockfd_UDP);
	close(server_socket);

	return 0;
}