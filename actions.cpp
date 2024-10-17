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

void insertCSV(const Schema& schema, const string& tableName, const string& values) {
    int primaryKey = getPrimaryKey(schema.name+"/"+tableName+"/"+tableName);
    int fileCount= 1;
    string filePath = schema.name+"/"+tableName+"/"+to_string(fileCount)+".csv";
    //ofstream outfile(filePath, ios::app); //Открываем файл для добавления
    //ios::app: Это флаг, который указывает, что файл должен быть открыт в режиме добавления (append mode).
    //Это означает, что любые данные, записанные в файл, будут добавлены в конец существующего содержимого файла,
    //а не перезаписывать его.
    int rowCount = getRowCount(filePath);
    
    while (rowCount >= schema.tuples_limit) {   //Проверка количества строк
        fileCount++;    //Если больше предела, переходим в новый файл
        rowCount = 0;   //Сбрасываем счетчик для нового файла
        filePath = schema.name+"/"+tableName+"/"+to_string(fileCount)+".csv";
        rowCount = getRowCount(filePath);
    }

    ofstream outfile(filePath, ios::app);
    if (outfile.is_open()) {
        /*
        string temp = to_string(primaryKey) + ",";
        stringstream values_ss(values);
        string value;
        while (getline(values_ss, value, ',')) {
            while (value[0]==' '){
                value.erase(0, 1);
            }
            value.erase(0, 1); // Удаляем кавычки
            value.erase(value.size() - 1, 1);
            temp += value + ",";
        }temp.erase(temp.size()-1,1);
        int columnCount;
        ifstream fin (filePath);
        string columnRow;
        getline(fin, columnRow);
        //columnCount = commaCount(columnRow);
        */
        
        /*
        //Добавляем первичный ключ как первую колонку
        outfile << primaryKey << ",";
        //Добавляем остальные значения
        for (Node* current = query.values->head; current != nullptr; current = current->next) {
            outfile << current->data;
            if (current->next != nullptr) {
                outfile << ",";
            }
        }
        
        outfile << endl;
        outfile.close();
        */
        //Обновляем первичный ключ в файле
        updatePrimaryKey(schema.name+"/"+tableName+"/"+tableName, primaryKey + 1);
        cout << "Database updated succesfully. " << endl;
    } else {
        cout << "Ошибка открытия файла " << filePath << endl;
    }
}
