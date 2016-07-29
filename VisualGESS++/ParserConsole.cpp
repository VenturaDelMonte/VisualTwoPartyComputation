/*
	file name: ParserConsole.cpp
	author: Anna De Angelis
	created on: 1/07/13
*/

#include "stdafx.h"
#include "ParserConsole.h"
#include <direct.h>
#include <fcntl.h>
#include <io.h>

void usage()
{
	Log("usage:");
	Log("-f: set boolean function");
	Log("-xi: set i-th x value");
	Log("-o: set output dir (path must exist)");
	Log("-e: set shares pixel zoom");
	Log("-t: set images size");
}

CParserConsole::~CParserConsole()
{
}

/**
	Parse input. 
	Generic input is: -f (to indicate that what is ahead is the function) followed by function. // o.O ma ke significa? 
	To give values to variables: -xi and the corresponding value
	It can set also the output directory 
*/
void CParserConsole::Parse(char** p, int size)
{
	bool func_found = false;
	char current_path[MAX_PATH];
	_getcwd(current_path, sizeof(current_path));
	//Log("%s", current_path);
	output.assign(current_path);
	if (size <= 1)
	{
		usage();
		exit(0);
	}
	for(int i = 0; i < size; i++)
	{
		if(!strcmp(p[i], "-f") || !strcmp(p[i], "-F"))
		{
			func.assign(strlwr(p[i + 1]));
			func_found = true;
			i++;
		}
		else if (!strcmp(p[i], "-h") || !strcmp(p[i], "/?"))
		{
			usage();
			exit(0);
		}
		else if (!strcmp(p[i], "-o") || !strcmp(p[i], "-O"))
		{
			output = p[i + 1];
		}
		else if (!strcmp(p[i], "-e") || !strcmp(p[i], "-E"))
		{
			exp = atoi(p[i+1]);
		}
		else if(!strncmp(p[i], "-X", 2) || !strncmp(p[i], "-x", 2))
		{
			int variable = atoi(&(p[i][2]));
			int value = atoi(p[i+1]);
			
			if(notFoundVariables(variable))
			{
				switch (value)
				{
					case 0: variables_vector[variable] = false; break;
					case 1: variables_vector[variable] = true; break;
					default: ASSERT(0, "wrong value"); 
				}
			}
		}
		else if (!strcmp(p[i], "-t") || !strcmp(p[i], "-T") || !strcmp(p[i], "-fdp"))
		{
			fdp = atoi(p[i+1]);
		}
	}
	ASSERT(!variables_vector.empty(), "Error variable");
	ASSERT(func_found, "no input function");
}

LPCSTR CParserConsole::getFunction() const
{
	return func.c_str();
}

bool CParserConsole::notFoundVariables(int variable)
{
	if (!variables_vector.size())
		return true;
	if(variables_vector.find(variable) == variables_vector.end())
			return true;
	return false;
}
