#include <iostream>
#include <string>
#include "DBinit.h"
#include "syntaxCheck.h"
using namespace std;

int main(){
    dbInit();   //Функция создания и проверки наличия БД
    cout << "Database ready. Waiting for query: ";
    string query;
    while (true){
        getline(cin, query);
        while (!syntaxCheck(query)){   //Проверка на соответствие синтаксису
            cout << "Command is not defined. Try again: ";
            getline (cin, query);   //Если синтаксис не верный - ввод заново
        }
        string action = query.substr(0,query.find(' ')); //Выборка первого слова
        if (action == "INSERT"){    //Вызов функции вставки
            //костыль
        }if (action == "DELETE"){   //Вызов функции удаления
            //костыль
        }if (action == "SELECT"){   //Вызов функции выборки
            //костыль
        }if (action == "EXIT"){ //Выход из программы
            return 0;
        }
    }return 0;
}
//INSERT INTO table1 VALUES ('somedata', '12345')
//EXIT