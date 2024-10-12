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

void dbInit(){
  ifstream file("schema.json");
  json j;
  file >>j;
  string schemeName;
  int tuplesLimit;
  for ( const auto& data:j){
  schemeName = data["name"].get<string>();
  createDir(schemeName);
  tuplesLimit=data["tuples_limit"].get<int>();
  for (const auto& [tableName, columns] : data["structure"].items()){
    createDir(schemeName+"/"+tableName);
    ifstream fin(schemeName+"/"+tableName+"/"+"1.csv");
    if (fin.good()){
      cout << "1.csv already exist!" << endl;
      }else {
        ofstream fout;
        fout.open (schemeName+"/"+tableName+"/"+"1.csv"); 
        fout << tableName+"_" <<"pk" <<",";
        int i=0;
    
        for( const auto& column : columns) {
          fout << column.get<string>();
          if (i<columns.size()-1) fout << ",";
          i++;
        }
        fout << endl;
        fout.close();
        cout << "Created "+schemeName+"/"+tableName+"/"+tableName+"/1.csv" << endl;
        }
        fin.close();
        fin.open(schemeName+"/"+tableName+"/"+tableName+"_pk_sequence");
        if (fin.good()) {
          cout << schemeName+"/"+tableName+"/"+tableName+"_pk_sequence is already exist!" << endl;
        } else{
          ofstream fout;
          fout.open(schemeName+"/"+tableName+"/"+tableName+"_pk_sequence");
          fout << 1;
          fout.close();
          cout << "Created "+schemeName+"/"+tableName+"/"+tableName+"_pk_sequence" << endl;
        }
        fin.close();
        fin.open(schemeName+"/"+tableName+"/"+tableName+"_lock");
        if (fin.good()){
          cout << schemeName+"/"+tableName+"/"+tableName+"_lock is already exist!" << endl;
        }else{
          ofstream fout;
          fout.open(schemeName+"/"+tableName+"/"+tableName+"_lock");
          fout << "0";
          fout.close();
          cout << "Created "+schemeName+"/"+tableName+"/"+tableName+"_lock" << endl;
          }fin.close();
          fin.open(schemeName+"/"+tableName+"/tuplesLimit");
          if (fin.good()){
            cout << schemeName+"/"+tableName+"/tuplesLimit is already exist" << endl;
          }else{
            ofstream fout;
            fout.open(schemeName+"/"+tableName+"/tuplesLimit");
            fout << tuplesLimit;
            fout.close();
            cout << "Created "+schemeName+"/"+tableName+"/tuplesLimit" << endl;
          }fin.close();
        }   
    }
}