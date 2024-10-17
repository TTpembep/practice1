//#include "structures.h"
#include "DBinit.h"
#include "syntaxCheck.h"
#include "actions.h"

int main(){
    Schema schema;
    dbInit(schema);   //Функция создания и проверки наличия БД
    cout << "Database ready. ";
    while (true){
        string query;
        SQLQuery parsedQuery;
        cout << "Waiting for query: ";
        do {
            getline(cin, query);
            parsedQuery = syntaxCheck(query);
            if (!parsedQuery.isRight) {
            cout << "Command is not defined. Try again: ";
            }
            } while (!parsedQuery.isRight);
        if (parsedQuery.action == "INSERT"){    //Вызов функции вставки
            insertCSV(schema, parsedQuery);
        }if (parsedQuery.action == "DELETE"){   //Вызов функции удаления
            //костыль
        }if (parsedQuery.action == "SELECT"){   //Вызов функции выборки
            //костыль
        }if (parsedQuery.action == "EXIT"){ //Выход из программы
            return 0;
        }
    }return 0;
}
//INSERT INTO table1 VALUES ('somedata', '12345', 'hello', 'goyda')
//EXIT