#pragma once
#include<string>
#include<cstdio>

bool readLine(FILE* file,std::string& line)
{
	unsigned char ch;
	while ((ch = fgetc(file)) != EOF && ch != '\n')
	{
		line += ch;
		if (feof(file))
		{
			return false;
		}
	}
	return true;
}

