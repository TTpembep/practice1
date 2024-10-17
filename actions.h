#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "structures.h"
using namespace std;

int getPrimaryKey(const string& tablePath );
void updatePrimaryKey(const string& tableName, int newPrimaryKey);
int getRowCount(const string& filePath);
void insertCSV(const Schema& schema, const SQLQuery& query);

#endif