#pragma once

#include <string.h>
#include <fstream>
#include <string>
#include <map>

using std::string;
using std::map;

map<string, string> parseHTTPMessage(string http_header);
string readFile(string path);
