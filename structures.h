#ifndef INCLUDE_STRUCTURES_H
#define INCLUDE_STRUCTURES_H

#include <iostream>
#include <string>
using namespace std;

struct Node;
struct fList;
struct Schema { //Структура данных для хранения информации о схеме
    string name;
    int tuples_limit;
};

#endif