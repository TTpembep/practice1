//#include "structures.h"
#include "DBinit.h"
#include "syntaxCheck.h"
#include "actions.h"

int main(){
    Schema schema;
    dbInit(schema);   //Функция создания и проверки наличия БД
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
            stringstream ss (query);
            string token;
            getline(ss,token,' ');  //Пропуск INSERT
            getline(ss,token,' ');  //Пропуск INTO
            getline(ss,token,' ');
            string tableName = token;   //Запись названия таблицы
            getline(ss,token,' ');  //Пропуск VALUES
            getline(ss,token);  //Получение строки значений
            token.erase(0,1);
            token.erase(token.size()-1,1);
            insertCSV(schema, tableName, token);
        }if (action == "DELETE"){   //Вызов функции удаления
            //костыль
        }if (action == "SELECT"){   //Вызов функции выборки
            //костыль
        }if (action == "EXIT"){ //Выход из программы
            return 0;
        }
        cout << "Waiting for query: ";
    }return 0;
}
//INSERT INTO table1 VALUES ('somedata', '12345', 'hello', 'goyda')
//EXIT