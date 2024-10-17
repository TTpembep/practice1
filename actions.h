#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int getPrimaryKey(const string& tablePath );
void updatePrimaryKey(const string& tableName, int newPrimaryKey);
int getRowCount(const string& filePath);
void insertCSV(const string& schemeName, const string& tableName, const string& values);

#endif