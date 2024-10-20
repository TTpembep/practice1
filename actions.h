#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "structures.h"
using namespace std;

bool isUnlocked(const string& schemaName, const string& tableName);
void lock(const string& schemaName, const string& tableName);
void unlock(const string& schemaName, const string& tableName);

int getPrimaryKey(const string& tablePath );
void updatePrimaryKey(const string& tableName, int newPrimaryKey);
int getRowCount(const string& filePath);
void insertCSV(const Schema& schema, const SQLQuery& query);

string buildConditionString(Node* node);
bool isConditionTrue(const string& line, const string& columnNames, const string* delLine);
void deleteFromCSV(const Schema& schema, const SQLQuery& query);

#endif