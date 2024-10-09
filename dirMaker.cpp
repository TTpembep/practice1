#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

bool createDir (string name) {
 if (mkdir(name.c_str(), 0755) == -1) {
    cout << "Error creating directory: Directory exists " << endl;
    return 0;
  } else {
    cout << "Directory " << name << " created successfully!" << endl;
  return 1;
  }
}
void dirMake(){
    ifstream f("schema.json");
    json data = json::parse(f);
    createDir(string(data[0]["name"]));
    //mkdir(data[0]["name"]);
    //string name = data[0]["name"];
    int tuplesLimit = data[0]["tuples_limit"];
    //json object = data["structure"];
}