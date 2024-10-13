#include <iostream>
#include <string>
using namespace std;

bool syntaxCheck(string query){
    string action = query.substr(0,query.find(' '));
    if (action == "INSERT"){
        //костыль
    }if (action == "DELETE"){ 
        //костыль
    }if (action == "SELECT"){
         //костыль
    }if (action == "EXIT" and query == "EXIT"){ 
        return 1;
    }return 0;
}