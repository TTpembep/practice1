#include "syntaxCheck.h"

bool isServiceWord(string word){
    string serviceWords = "INSERT INTO VALUES WHERE OR AND DELETE SELECT EXIT FROM";
    if (serviceWords.find(word) == string::npos) return 0;
    return 1;
}
SQLQuery syntaxCheck(string query){
    SQLQuery temp;
    stringstream ss (query);
    getline(ss, temp.action, ' ');
    string token;
    if (temp.action == "INSERT"){
        getline(ss, token, ' ');
        if (token!="INTO"){
            temp.isRight = false;
            return temp;
        }
        getline(ss, token, ' ');
        if (isServiceWord(token)){
            temp.isRight = false;
            return temp;
        }
        temp.tableName = token;
        getline(ss, token, ' ');
        if (token!="VALUES"){
            temp.isRight = false;
            return temp;
        }
        getline(ss, token);
        if (token[0] != '(' || token[token.size()-1] != ')'){
            temp.isRight = false;
            return temp;
        }
        temp.values = new fList();
        stringstream valueSS (token);
        string value;
        while (getline(valueSS,value, '\'')){
            if (value[0] != '(' && value[0] != ',' && value[0] != ')'){
                temp.values->push_back(value);
            }
        }temp.isRight = true;
        return temp;
    }if (temp.action == "DELETE"){ 
        //костыль
    }if (temp.action == "SELECT"){
         //костыль
    }if (temp.action == "EXIT" and query == "EXIT"){ 
        temp.isRight = true;
        return temp;
    }temp.isRight = false;
    return temp;
}
//INSERT INTO table1 VALUES ('somedata', '12345')