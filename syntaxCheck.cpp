#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool isServiceWord(string word){
    string serviceWords = "INSERT INTO VALUES WHERE OR AND DELETE SELECT EXIT FROM";
    if (serviceWords.find(word) == string::npos) return 0;
    return 1;
}
bool syntaxCheck(string query){
    stringstream ss (query);
    string action;
    getline(ss, action, ' ');
    string token;
    if (action == "INSERT"){
        getline(ss, token, ' ');
        if (token!="INTO"){
            cout << "Syntax error. ";
            return 0;
        }
        getline(ss, token, ' ');
        if (isServiceWord(token)){
            cout << "Syntax error. ";
            return 0;
        }
        getline(ss, token, ' ');
        if (token!="VALUES"){
            cout << "Syntax error. ";
            return 0;
        }
        getline(ss, token);
        if (token[0] != '(' || token[token.size()-1] != ')'){
            cout << "Syntax error. ";
            return 0;
        }return 1;
    }if (action == "DELETE"){ 
        //костыль
    }if (action == "SELECT"){
         //костыль
    }if (action == "EXIT" and query == "EXIT"){ 
        return 1;
    }return 0;
}
//INSERT INTO table1 VALUES ('somedata', '12345')