#include "actions.h"

int getPrimaryKey(const string& tablePath ){
    string filePath = tablePath + "_pk_sequence";
    ifstream infile(filePath);
    int primaryKey = 0;
    if (infile.is_open()) {
        infile >> primaryKey;
        infile.close();
    }
    return primaryKey;
}

void updatePrimaryKey(const string& tableName, int newPrimaryKey) {
    string filePath = tableName + "_pk_sequence";
    ofstream outfile(filePath);
    if (outfile.is_open()) {
        outfile << newPrimaryKey;
        outfile.close();
    }
}

int getRowCount(const string& filePath) {
    ifstream infile(filePath);
    int count = 0;
    string line;
    while (getline(infile, line)) {
        count++;
    }
    infile.close();
    return count;
}

void insertCSV(const Schema& schema, const SQLQuery& query) {
    int primaryKey = getPrimaryKey(schema.name+"/"+query.tableName+"/"+query.tableName);
    int fileCount= 1;
    string filePath = schema.name+"/"+query.tableName+"/"+to_string(fileCount)+".csv";
    int rowCount = getRowCount(filePath);
    while (rowCount >= schema.tuples_limit) {   //Проверка количества строк
        fileCount++;    //Если больше предела, переходим в новый файл
        rowCount = 0;   //Сбрасываем счетчик для нового файла
        filePath = schema.name+"/"+query.tableName+"/"+to_string(fileCount)+".csv";
        rowCount = getRowCount(filePath);
    }
    ifstream fin(filePath); //Проверка на совпадение количества добавляемых 
    if(!fin.is_open()) {    //элементов с количеством колонок в таблице
        cout << "File "+filePath+" not found. " <<endl;
        return;
    };
    int count_column =0;
    string column_name;
    getline(fin,column_name);
    stringstream ss(column_name);
    while(getline(ss,column_name,',')) {
        count_column++;
    }
    for (Node * current = query.values->head;current!=nullptr;current=current->next,count_column--);
    if (count_column!=1) {
        cout << "Insert error. Wrong amount of arguments. " << endl;
        return;
    }
    //ios::app: Это флаг, который указывает, что файл должен быть открыт в режиме добавления (append mode).
    //Это означает, что любые данные, записанные в файл, будут добавлены в конец существующего содержимого файла,
    //а не перезаписывать его.
    ofstream outfile(filePath, ios::app);   //Открываем файл для добавления
    if (outfile.is_open()) {
        outfile << primaryKey << ",";   //Добавляем первичный ключ как первую колонку
        for (Node* current = query.values->head; current != nullptr; current = current->next) {
            outfile << current->data;   //Добавляем остальные значения
            if (current->next != nullptr) {
                outfile << ",";
            }
        }
        outfile << endl;    //Закрываем файл
        outfile.close();    //Обновляем первичный ключ в файле
        updatePrimaryKey(schema.name+"/"+query.tableName+"/"+query.tableName, primaryKey + 1);
        cout << "Database updated succesfully. " << endl;
    } else {
        cout << "An error occured when opening file " << filePath << endl;
    }
}
