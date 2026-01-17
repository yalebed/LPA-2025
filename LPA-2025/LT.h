#pragma once
#define LEXEMA_FIXSIZE 2
#define	LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_BYTE 't'
#define LEX_INT 't'
#define LEX_TORF 't'
#define LEX_SYMB 't'
#define LEX_STRING 't'

#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_TASK 'f'
#define LEX_DECLARE 'd'
#define LEX_RESPONSE 'r'
#define LEX_COUT 'p'
#define LEX_BEGIN 'm'
#define LEX_WHEN 's'
#define LEX_OTHER 'e'

#define LEX_STRCPR 'C'
#define LEX_ATOII 'T'

#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_RIGHTBRACE '}'
#define LEX_LEFTTHESIS '('
#define LEX_RIGHTTHESIS ')'
#define LEX_OPERATION 'v'
#define LEX_EQUAL '='
#define LEX_COMPARISON 'b'

#define LEX_SWITCH 'h'   // switch
#define LEX_CASE 'c'     // is
#define LEX_DEFAULT 'a'  // any
#define LEX_COLON ':'    // :

namespace LT
{
	struct Entry
	{
		char lexema[LEXEMA_FIXSIZE];
		int sn;
		int idxTI;
		int col;
	};
	struct lexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};
	lexTable Create(int size);
	void Add(lexTable& lexTable, Entry entry);
	Entry GetEntry(lexTable& lexTable, int n);
	void Delete(lexTable& lexTable);
	void PrintLT(lexTable& lexTable);
}