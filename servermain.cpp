#include "constant.h"
#include "tcp.h"
#include "udp.h"
#include "country.h"

using namespace std;

int main(){

    cout << "Main Server is up and running" << endl << endl;

	// initialize udp
    int sockfd_UDP_A, sockfd_UDP_B;
    sockaddr_in A_serverAddr_UDP, B_serverAddr_UDP;
    create_UDP(sockfd_UDP_A, A_serverAddr_UDP, UDP_SERVER_A, LOCAL_IP);
    create_UDP(sockfd_UDP_B, B_serverAddr_UDP, UDP_SERVER_B, LOCAL_IP);

	// send country request 
		// from server A
	string countryList_A = UDP_send_receive(sockfd_UDP_A, A_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	cout << "[+] Country list from server A over UDP port <" << UDP_SERVER_A << "> : " << countryList_A << endl << endl;
	unordered_set<string> country_set_A = convert_string_to_set(countryList_A);
		// from server B
	string countryList_B = UDP_send_receive(sockfd_UDP_B, B_serverAddr_UDP, REQUEST_COUNTRY_LIST);
	cout << "[+] Country list from server B over UDP port <" << UDP_SERVER_B << "> : "  << countryList_B << endl << endl;
	unordered_set<string> country_set_B = convert_string_to_set(countryList_B);
	
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
		// cout << "[+] Connection accepted from " << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) << "\n";

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
				cout<< endl << "----------------------------------------------------" << endl;

				cout << "[+] Request from Client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
					 << "> over TCP port <" << TCP_MAIN << "> :" << buffer << endl << endl;

				// split message
				vector<string> message_list = convert_string_to_vector(string(buffer));
				cout << message_list.size() << endl;
				if (message_list.size() != 2) {
					cout << "[-] Error in message from client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
						 << "> over TCP port <" << TCP_MAIN << ">" << endl << endl;
					send(newSocket, ERROR.c_str(), ERROR.size() + 1, 0);

					continue;
				}

				int userID = stoi(message_list[0]);
				string country_name = message_list[1];
				string reply;

				// check country existed or not, and do something
				if (country_set_A.find(country_name) == country_set_A.end() && country_set_B.find(country_name) == country_set_B.end()) {
					
					// send back to client
					reply = "<" + country_name + "> " + NO_COUNTRY_FOUND;
					send(newSocket, reply.c_str(),reply.size() + 1, 0);
					cout << "[+]Data send to Client:  <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
						 << "> over TCP port <" << TCP_MAIN << "> :" <<  reply << endl << endl;
					continue;
				}
				else if (country_set_A.find(country_name) != country_set_A.end()) {
					reply = UDP_send_receive(sockfd_UDP_A, A_serverAddr_UDP, buffer);
				}
				else {
					reply = UDP_send_receive(sockfd_UDP_B, B_serverAddr_UDP, buffer);
				}

				// send back to client
				send(newSocket, reply.c_str(),reply.size() + 1, 0);
				cout << "[+]Data send to Client <" << inet_ntoa(newAddr.sin_addr) << ": " << ntohs(newAddr.sin_port) 
					 << "> over TCP port <" << TCP_MAIN << "> :" <<  reply << endl << endl;

			}
		}
		close(newSocket);

	}

	close(sockfd_UDP_A);
	close(sockfd_UDP_B);
	close(server_socket);

	return 0;
}