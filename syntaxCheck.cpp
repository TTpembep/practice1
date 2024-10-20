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
    string token;   //Проверка синтаксиса команды вставки
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
    }   //Проверка синтаксиса команды удаления
    if (temp.action == "DELETE"){ 
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
        temp.delLine = token;
        getline(ss,token);  //PARCER START
        stringstream valueSS (token);
        string value;
        int counter = 1;
        while (getline(valueSS,value, ' ')){
            temp.isRight = false;
            if (value.find('.') != string::npos && (counter == 1 || counter == 3)){
                stringstream tempSS (value);
                string tempVal;
                getline(tempSS,tempVal,'.');
                if (tempVal != temp.tableName){
                    temp.isRight = false;
                    return temp;
                }
                getline(tempSS,tempVal);
                temp.delLine += ' ' + tempVal;
                temp.isRight = true;
            }
            if (value == "=" && counter == 2){
                temp.delLine += ' ' + value;
                temp.isRight = true;
            }
            if (value[0] == '\'' && counter == 3){
                value.erase(0,1);
                value.erase(value.size()-1,1);
                temp.delLine += ' ' + value;
                temp.isRight = true;
            }
            if ((value == "AND" || value == "OR") && (counter == 4)){
                temp.delLine += ' ' + value;
                counter = 0;
                temp.isRight = true;
            }
            counter++;
        }//PARCER END
        return temp;
    }   //Проверка синтаксиса функции выборки
    if (temp.action == "SELECT"){  //Требуется переработка
        getline(ss, token, ' ');    //Из фунции для DELETE 
        if (token!="FROM"){         //В функцию для SELECT
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
                temp.isRight = true;    //Можно доработать для нескольких OR AND
            }
            counter++;
        }//PARCER END

        //temp.tables->print();
        //temp.columns->print();
        //temp.values->print();
        //cout << temp.condition << endl;

        return temp;
    }
    if (temp.action == "EXIT" and query == "EXIT"){ 
        temp.isRight = true;
        return temp;
    }temp.isRight = false;
    return temp;
}