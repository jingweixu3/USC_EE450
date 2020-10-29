#include "country.h"


using namespace std;


int main () {

    cout << "testcase 1: " << endl;

    string file_path1 = "./testcase1/data1.txt";
    string file_path2 = "./testcase1/data2.txt";

    unordered_set<Country*> country_set1;
    build_grapgh(country_set1, file_path1);
    build_grapgh(country_set1, file_path2);

    print_country_info(country_set1);

    cout << endl<<"testcase 2: " << endl;

    file_path1 = "./testcase2/data1.txt";
    file_path2 = "./testcase2/data2.txt";

    unordered_set<Country*> country_set2;
    build_grapgh(country_set2, file_path1);
    build_grapgh(country_set2, file_path2);

    print_country_info(country_set2);

    cout << endl <<"testcase 3: " << endl;

    file_path1 = "./testcase3/data1.txt";
    file_path2 = "./testcase3/data2.txt";

    unordered_set<Country*> country_set3;
    build_grapgh(country_set3, file_path1);
    build_grapgh(country_set3, file_path2);

    print_country_info(country_set3);


    return 0;
}