#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

bool createDir (string name);
void dirMake();