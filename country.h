#ifndef COUNTRY_H
#define COUNTRY_H
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <sstream>
#include <string>

using namespace std;

class Country {      
    public:
        string name;
        unordered_set<int> user_list;
        unordered_map<int, unordered_set<int>> friend_list;

        Country(string name){
            this->name = name;
        }
};


void build_grapgh(unordered_set<Country*> &country_set, string file_path) {
    
    string line;
    ifstream myfile (file_path);

    if (myfile.is_open()){
       
        string country_name;
        Country *country = nullptr;
       
        while (getline (myfile,line) ){
            if (isalpha(line[0])) {
                country_name = line;
                country = new Country(country_name);
                country_set.insert(country);
                continue;
            }

            istringstream ss(line);
            int count = 0;
            int userID = -1;
            
            while (ss) {

                string word;
                ss >> word;

                if (!isdigit(word[0])) {
                    continue;
                }

                if (count == 0) {
                    userID = stoi(word);
                    country->user_list.insert(userID);
                    count++;
                    continue;
                }
                (country->friend_list)[userID].insert(stoi(word));
               
                count++;
            }
        }
        myfile.close();
    }

    else cout << "Unable to open file"; 

}


long count_relationship(Country* country) {
    long result = 0;
    for (auto f : country->friend_list) {
        result = result + f.second.size();
    }
    return result;
}

void print_country_info(unordered_set<Country*> &country_set) {
    for (auto country : country_set) {
        cout << country->name << " "  << country->user_list.size() << " "  << count_relationship(country)<< endl;
    }
}


#endif 