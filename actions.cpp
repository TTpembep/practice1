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
    int columnCount =0;
    string columnName;
    getline(fin,columnName);
    stringstream ss(columnName);
    while(getline(ss,columnName,',')) {
        columnCount++;
    }
    for (Node * current = query.values->head;current!=nullptr;current=current->next,columnCount--);
    if (columnCount!=1) {
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

string buildConditionString(Node* node) {
    stringstream ss;    //преобразуем ноду в строку
    while (node != nullptr) {
        ss << node->data << " ";
        node = node->next;
    }
    return ss.str();
}

bool isConditionTrue(const string& line, const string& columnNames, const string& delLine) {
    stringstream ss(delLine);
    string token;
    fList* tokens = new fList();    //Разбиваем запрос на отдельные части
    while (getline(ss, token, ' ')) {
        tokens->push_back(token);
    }
    if (tokens->head->data == "WHERE") {    //Проверка на "WHERE"
        Node* andOrNode = nullptr;  //Ищем "AND" или "OR" в списке
        for (Node* current = tokens->head->next; current != nullptr; current = current->next) {
            if (current->data == "AND" || current->data == "OR") {
                andOrNode = current;
                break;
            }
        }
        if (andOrNode != nullptr) { //Найден оператор "AND" или "OR"
            string operatorToken = andOrNode->data;
            //Разделяем условие на две части
            Node* leftPart = tokens->head->next;
            Node* rightPart = andOrNode->next;
            //Рекурсивно проверяем левую и правую части
            bool leftResult = isConditionTrue(line, columnNames, buildConditionString(leftPart));
            bool rightResult = isConditionTrue(line, columnNames, buildConditionString(rightPart));
            //Возвращаем результат в соответствии с оператором
            if (operatorToken == "AND") {
                return leftResult && rightResult;
            } else if (operatorToken == "OR") {
                return leftResult || rightResult;
            }
        }
    }
    if(tokens->head->data == "WHERE") tokens->remove("WHERE");
    if (tokens->head->next->data == "=") {
        string column = tokens->head->data;
        stringstream ss(columnNames);
        string columnName;
        int count=0;
        while(getline(ss,columnName,',') && columnName!=column){
            count++;    //Пока не дошли до нужной колонки считаем
        }
        ss.str(""); //Очищаем поток
        string value = tokens->head->next->next->data;  //Достаем нужное значение
        ss << line; //Текущую строку вносим в поток
        string current_value;
        while (getline(ss, current_value,',') && count!=0){
            count--;    //Доходим до нужной колонки
        }
        if (current_value == value) {   //Проверяем совпадения значений
            return current_value == value;  //Если верно - строка не записывается
        }
    }
    tokens->clear();
    delete tokens;
    return false;
}

void deleteFromCSV(const Schema& schema, const SQLQuery& query){
    int fileCount=1;
    string filePath = schema.name+"/"+query.tableName+"/" +to_string(fileCount)+ ".csv";
    ifstream infile(filePath);
    while(infile.is_open()){
        ofstream outfile(filePath + ".tmp", ios::out); //Открытие временного файла
        if (infile.is_open() && outfile.is_open()) {
            string line, columnNames;
            getline(infile,columnNames);    //Запись строки колонок
            outfile << columnNames << endl;
            while (getline(infile, line)) {
                if (!isConditionTrue(line, columnNames, query.delLine)) { 
                    outfile << line << endl;    //Если условие не выполняется, записываем строку в временный файл
                }
            }
        infile.close();
        outfile.close();
        remove(filePath.c_str());  //Удаляем прошлый основной файл
        rename((filePath + ".tmp").c_str(), filePath.c_str());  //Переименовываем временный в основной
        cout << "Database updated succesfully. " << filePath << endl;
        }else {
            cout << "An error occured when opening file " << filePath << endl;
        }
        fileCount++;    //Если файлов несколько переходит к следующему
        filePath = schema.name+"/"+query.tableName+"/" +to_string(fileCount)+ ".csv";
        infile.open(filePath);
    }
}