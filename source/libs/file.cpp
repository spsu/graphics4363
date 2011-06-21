#include "file.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

string readFile(string filename)
{
	ifstream fs(filename.c_str());
	return string(istreambuf_iterator<char>(fs), istreambuf_iterator<char>());
}

