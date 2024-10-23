#include "utils.h"

map<string, string> parseHTTPMessage(string http_header)
{
	return map<string, string>();
}

string readFile(string path)
{
	string contents;
	string line;
	std::ifstream f(path);

	while (std::getline(f, line)) {
		contents.append(line);
	};

	f.close();
	return contents;
}
