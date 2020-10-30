#ifndef COUNTRY_H
#define COUNTRY_H
#include "constant.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <sstream>
#include <climits>
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


void build_country(unordered_set<Country*> &country_set, string file_path) {
    
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
            
            do {
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
            }while (ss);
        }
        myfile.close();
    }

    else cout << "Unable to open file"; 

}


int count_relationship(Country* country) {
    int result = 0;
    for (auto f : country->friend_list) {
        result = result + f.second.size();
    }
    return result;
}

void print_country_info(unordered_set<Country*> &country_set) {
    for (auto country : country_set) {
        cout << country->name << "      "  << country->user_list.size() << "        "  << count_relationship(country)<< endl;
    }
    cout << endl;
}

string country_list(unordered_set<Country*> &country_set) {
    
    string result = "";
    for (auto country : country_set) {
        result = result + country->name + " ";
    }
    return result;
}

unordered_set<string> convert_string_to_set(string input_string) {
    unordered_set<string> country_set;
    istringstream ss(input_string);
    
    do {
        string word;
        ss >> word;
        if (word == "") continue;
        country_set.insert(word);
    }while (ss);

    cout << endl;
    return country_set;

}

vector<string> convert_string_to_vector(string input_string) {
    vector<string> result;
    istringstream ss(input_string);

    do {
        string word;
        ss >> word;
        if (word == "") continue;
        result.push_back(word);
    }while (ss);

    return result;
}

int intersection(unordered_set<int> &set1, unordered_set<int> &set2) {
    int result = 0;
    for (int i : set1) {
        if (set2.find(i) != set2.end()) {
            result++;
        }
    }
    return result;
}


Country* find_country(unordered_set<Country*> &country_set, string country_name) {
    for (auto country : country_set) {
        if (country->name == country_name) {
            return country;
        }
    }
    return nullptr;
}


string recommendation_system(string country_name, int userID, unordered_set<Country*> &country_set) {
    
    Country* country = find_country(country_set, country_name);
    if (country == nullptr) {
        return NO_RECOMMENDATION;        
    }

    // check userID
    if (country->user_list.find(userID) == country->user_list.end()) {
        return "<" + to_string(userID) + "> " + NO_USER_FOUND;
    }

    // check none
    if (country->user_list.size() == (country->friend_list)[userID].size()){
        return NO_RECOMMENDATION;
    }

    if ((country->friend_list)[userID].size() == 0) {
        return NO_RECOMMENDATION;
    }

    // find unconnected set N
    unordered_set<int> unconnected_set;
    unordered_set<int> user_friend_set = (country->friend_list)[userID];
    
    for (int id : country->user_list) {
        if (user_friend_set.find(id) == user_friend_set.end() && id != userID) {
            unconnected_set.insert(id);
        }
    }

    //check common freinds
    int max_num = 1;
    long max_id = LONG_MAX;
    
    for (int id : unconnected_set) {
        int inter_count = intersection(country->friend_list[id], user_friend_set);
        if (max_num == inter_count && id < max_id) {
            max_id = id;
        }
        else if (max_num < inter_count) {
            max_num = inter_count;
            max_id = id;
        }
    }

    if (max_id != LONG_MAX) {
        return RECOMMENDATION + " <" + to_string(max_id) + ">";
    }
    
    // highest degree
    max_num = 0;
    max_id = LONG_MAX;
    for (int id : unconnected_set) {
        int count = country->friend_list[id].size();
        if (max_num == count && id < max_id) {
            max_id = id;
        }
        else if (max_num < count) {
            max_num = count;
            max_id = id;
        }
    }
    
    if (max_id != LONG_MAX) {
        return RECOMMENDATION + " <" + to_string(max_id) + ">";
    }

    return NO_RECOMMENDATION;

}

#endif 