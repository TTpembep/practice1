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
        getline(ss, token, ' ');
        if (token!="FROM"){
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
        if (token!="WHERE"){
            temp.isRight = false;
            return temp;
        }
        getline(ss,token);  //PARCER START
        temp.values = new fList();
        temp.tables = new fList();
        temp.columns = new fList();
        stringstream valueSS (token);
        string value;
        int counter = 1;
        while (getline(valueSS,value, ' ')){
            temp.isRight = false;
            if (value.find('.') != string::npos && (counter == 1 || counter == 3)){
                stringstream tempSS (value);
                string tempVal;
                getline(tempSS,tempVal,'.');
                temp.tables->push_back(tempVal);
                getline(tempSS,tempVal);
                temp.columns->push_back(tempVal);
                temp.values->push_back("SERVICEWORD");
                temp.isRight = true;
            }
            if (value == "=" && counter == 2){
                temp.isRight = true;
            }
            if (value[0] == '\'' && counter == 3){
                value.erase(0,1);
                value.erase(value.size()-1,1);
                temp.values->push_back(value);
                temp.isRight = true;
            }
            if ((value == "AND" || value == "OR") && (counter == 4)){
                temp.condition = value;
                counter = 0;
                temp.isRight = true;
            }
            counter++;
        }//PARCER END
        return temp;
    }if (temp.action == "SELECT"){
         //костыль
    }if (temp.action == "EXIT" and query == "EXIT"){ 
        temp.isRight = true;
        return temp;
    }temp.isRight = false;
    return temp;
}
/*
INSERT INTO table1 VALUES ('somedata', '12345', 'hello', 'melon')
INSERT INTO table1 VALUES ('chicken', 'world', '123', 'peace')
INSERT INTO table2 VALUES ('17', 'chicken')
DELETE FROM table1 WHERE table1.column1 = table2.column2 AND table1.column3 = '123'
EXIT
*/