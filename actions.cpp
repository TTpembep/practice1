#include "actions.h"

bool isUnlocked(const string& schemaName, const string& tableName){
    ifstream fin(schemaName+"/"+tableName+"/"+tableName+"_lock");
    if(!fin.is_open()) {    
        cout << "File "+ schemaName+"/"+tableName+"/"+tableName+"_lock not found. " <<endl;
        return 0;
    };
    string lockStatus;
    getline(fin,lockStatus);
    if (lockStatus == "0"){
        return 1;
    }else {
        cout << "The table is not avaible now. Try again later. " << endl;
        return 0;
    }
}
void lock(const string& schemaName, const string& tableName){
    std::ofstream outputFile(schemaName+"/"+tableName+"/"+tableName+"_lock");
    outputFile << "1";
    outputFile.close();
}
void unlock(const string& schemaName, const string& tableName){
    std::ofstream outputFile(schemaName+"/"+tableName+"/"+tableName+"_lock");
    outputFile << "0";
    outputFile.close();
}

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
    ifstream fin(filePath);  
    if(!fin.is_open()) {    
        cout << "File "+filePath+" not found. " <<endl;
        return;
    };
    int columnCount =0;
    string columnName;
    getline(fin,columnName);
    stringstream ss(columnName);
    while(getline(ss,columnName,',')) { //Проверка на совпадение количества добавляемых
        columnCount++;                  //элементов с количеством колонок в таблице
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
        cout << "Database updated succesfully. Path: " << filePath << endl;
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

bool isConditionTrue(const string& row, const string& columnNames, const string& line) {
    stringstream ss(line);
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
            bool leftResult = isConditionTrue(row, columnNames, buildConditionString(leftPart));
            bool rightResult = isConditionTrue(row, columnNames, buildConditionString(rightPart));
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
        ss << row; //Текущую строку вносим в поток
        string curVal;
        while (getline(ss, curVal,',') && count!=0){
            count--;    //Доходим до нужной колонки
        }
        if (curVal == value) {   //Проверяем совпадения значений
            return curVal == value;  //Если верно - строка не записывается
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
            string row, columnNames;
            getline(infile,columnNames);    //Запись строки колонок
            outfile << columnNames << endl;
            while (getline(infile, row)) {
                if (!isConditionTrue(row, columnNames, query.line)) { 
                    outfile << row << endl;    //Если условие не выполняется, записываем строку в временный файл
                }
            }
        infile.close();
        outfile.close();
        remove(filePath.c_str());  //Удаляем прошлый основной файл
        rename((filePath + ".tmp").c_str(), filePath.c_str());  //Переименовываем временный в основной
        cout << "Database updated succesfully. Path: " << filePath << endl;
        }else {
            cout << "An error occured when opening file " << filePath << endl;
        }
        fileCount++;    //Если файлов несколько переходит к следующему
        filePath = schema.name+"/"+query.tableName+"/" +to_string(fileCount)+ ".csv";
        infile.open(filePath);
    }
}

string superPrintFunc(const string& row, const string& columnNames, const string& line) {
    stringstream ss(line);
    string token;
    fList* tokens = new fList();    //Разбиваем запрос на отдельные части
    while (getline(ss, token, ' ')) {
        tokens->push_back(token);
    }
    
    string column = tokens->head->data;
    stringstream sss(columnNames);
    string columnName;
    int count=0;
    while(getline(sss,columnName,',') && columnName!=column){
        count++;    //Пока не дошли до нужной колонки считаем
    }
    sss.str(""); //Очищаем поток
    //string value = tokens->head->next->next->data;  //Достаем нужное значение
    sss << row; //Текущую строку вносим в поток
    string curVal;
    string curPk;
    getline(sss,curPk,',');
    count--;
    while (getline(sss, curVal,',') && count!=0){
        count--;    //Доходим до нужной колонки
    }
    //cout << curVal << endl;
    tokens->clear();
    delete tokens;
    return curPk + "," + curVal;
}
void selectTables(const Schema& schema, const SQLQuery& query){
    string tmPath = schema.name+"/"+"tmp.csv";
    string iterPath = schema.name+"/"+"iter.csv";
    
    Node* curTab = query.tables->head;
    Node* curCol = query.columns->head;
    while (curTab != nullptr){
        ofstream tmpfile (tmPath, ios::out); //Создание временного файла для отборки
        int fileCount=1;
        string filePath = schema.name+"/"+curTab->data+"/" +to_string(fileCount)+ ".csv";
        ifstream infile(filePath);
        ifstream iterfile(iterPath);
        while(infile.is_open()){
            if (infile.is_open() && iterfile.is_open()){
                string iterRow;
                getline(iterfile, iterRow);
                tmpfile << iterRow + "," + curTab->data + "_pk," + curCol->data << endl;
                infile.close();
                string row, columnNames;
                while(getline(iterfile,iterRow)){
                    infile.open(filePath);
                    getline(infile,columnNames);
                    while (getline(infile, row)) {  //что-то кто-то
                        tmpfile << iterRow + "," + superPrintFunc(row, columnNames, curCol->data) << endl;
                    }
                    infile.close();
                }
                infile.close();
                iterfile.close();
            }else if(infile.is_open()){
                string row, columnNames;
                getline(infile,columnNames);
                tmpfile << curTab->data + "_pk," + curCol->data << endl;
                while (getline(infile, row)) {  //что-то кто-то
                    tmpfile << superPrintFunc(row, columnNames, curCol->data) << endl;
                }
                infile.close();
            }else {
                cout << "An error occured when opening file " << filePath << endl;
            }
        
            fileCount++;    //Если файлов несколько переходит к следующему
            filePath = schema.name+"/"+query.tableName+"/" +to_string(fileCount)+ ".csv";
            infile.open(filePath);
        }
        curTab = curTab->next;
        curCol = curCol->next;
        tmpfile.close();
        rename(tmPath.c_str(), iterPath.c_str());  //Переименовываем временный в основной
    }
    remove(tmPath.c_str());
    ifstream file(iterPath);
        string result;
        while (getline(file, result)){
            stringstream ss (result);
            string temp;
            while (getline(ss, temp, ',')){
                cout << temp << "\t";
                //getline(ss, temp, ',');
                //cout << temp << "\t";
            }cout << endl;
        }
        file.close();
    remove(iterPath.c_str());
}