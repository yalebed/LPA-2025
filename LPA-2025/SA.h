#pragma once
#include "stdafx.h"
#define MAX_PARM_COUNT 2
using namespace std;
namespace SA {

	bool startSA(LA::LEX lex);
	void checkSemicolons(LA::LEX lex); 
	void checkSwitchSyntax(LA::LEX lex);
	void checkBraces(LA::LEX lex);
	void conditions(LA::LEX lex);

};