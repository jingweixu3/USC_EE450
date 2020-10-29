#include "constant.h"
#include "udp.h"
#include "country.h"

using namespace std;

int main(){

    // create country list and build graph
    string file_path = "./testcase3/data2.txt";
    unordered_set<Country*> country_set;
    build_grapgh(country_set, file_path);
    print_country_info(country_set);

    // initialize UDP server
    int sockfd;
    sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_LENGTH];
    socklen_t addr_size;
    string message = "OK";

    create_UDP(sockfd, serverAddr, UDP_SERVER_B, LOCAL_IP);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))){
        cout << "[-]Error in binding.\n";
		return -1;
    }

    cout << "[+]ServerA is up and running!" << endl;
    
    // receiving and send back
    while (1) {
        memset(&buffer, '\0', sizeof(buffer));
        memset(&clientAddr, '\0', sizeof(clientAddr));
        
        socklen_t  siaddr_size = sizeof(clientAddr);

        recvfrom(sockfd, buffer, BUFFER_LENGTH, 0, (sockaddr*) &clientAddr, &siaddr_size);
        message = string(buffer);
        cout << "[+]Data Received:" << message << endl;

        if (message == REQUEST_COUNTRY_LIST) {
            message = country_list(country_set);
        }
        else {
            vector<string> message_list = convert_string_to_vector(string(buffer));
            cout << message_list.size() << endl;
            if (message_list.size() != 2) {
                cout << "[-]Error in message from client." << endl;
                continue;
            }

            int userID = stoi(message_list[0]);
            string country_name = message_list[1];

            cout << "[+]UserID: " <<userID << " Country Name: " << country_name << endl;

            // recommendation system
            message = recommendation_system(country_name, userID, country_set);

            // message = "DO SOMETHING";
        }

        sendto(sockfd, message.c_str(), message.size() + 1, 0, (sockaddr*) &clientAddr, sizeof(clientAddr));
        cout << "[+]Data Send: " << message << endl;
    }
    
    close(sockfd);

    return 0;
}