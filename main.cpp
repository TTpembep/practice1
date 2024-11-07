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
            getline(cin, query);    //Проверка синтаксиса команды
            parsedQuery = syntaxCheck(query);
            if (!parsedQuery.isRight) {
            cout << "Command is not defined. Try again: ";
            }
            } while (!parsedQuery.isRight);
        if (parsedQuery.action == "INSERT"){    //Вызов функции вставки
            if (isUnlocked(schema.name, parsedQuery.tableName)){    //Проверка что таблица доступна
                lock(schema.name,parsedQuery.tableName);
                insertCSV(schema, parsedQuery);
                unlock(schema.name,parsedQuery.tableName);
            }
        }if (parsedQuery.action == "DELETE"){   //Вызов функции удаления
            if (isUnlocked(schema.name, parsedQuery.tableName)){
                lock(schema.name,parsedQuery.tableName);
                deleteFromCSV(schema, parsedQuery);
                unlock(schema.name,parsedQuery.tableName);
            }
        }if (parsedQuery.action == "SELECT"){   //Вызов функции выборки
            //Нужна проверка блокировки двух таблиц
            //Блокировка двух таблиц
            parsedQuery.tableName = "table1";
            selectTables(schema, parsedQuery); //МОЖНО СДЕЛАТЬ ОБРАЩЕНИЕ ПО ИНДЕКСУ ФЛИСТА
            //Разблокировка двух таблиц
        }if (parsedQuery.action == "EXIT"){ //Выход из программы
            return 0;
        }
    }return 0;
}
/*
INSERT INTO table1 VALUES ('somedata', '12345', 'hello', 'melon')
INSERT INTO table1 VALUES ('chicken', 'world', '123', 'peace')
INSERT INTO table2 VALUES ('17', 'chicken')
DELETE FROM table1 WHERE table1.column3 = '123' AND table1.column1 = 'chicken'
DELETE FROM table1 WHERE table1.table1_pk = '4'
DELETE FROM table1 WHERE table2.column1 = '17'
SELECT table1.column1 table2.column1 FROM table1, table2
WHERE table1.column1 = table2.column2 AND table1.column2 = 'string'
EXIT
DELETE FROM table2 WHERE table2_pk = '3'
*/