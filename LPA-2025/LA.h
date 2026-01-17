#pragma once
#include"stdafx.h"
#define MAX_LEX_SIZE 4096
#define MAX_STR_SIZE 258
#define MAX_SYMB_SIZE 4
#define FST_AMOUNT 19
#define ID_SIZE 9

#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '"'
#define NEW_LINE '\n'
#define SEMICOLON ';'
#define COMMA ','
#define LEFT_BRACE '{'
#define RIGHT_BRACE '}'
#define LEFTTHESIS '('
#define RIGHTTHESIS ')'

#define AMPERSAND '&'
#define PIPE '|'
#define TILDE '~'
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define EQUAL '='
#define DIRSLASH '/'
#define GREATER '>'
#define LESS '<'

#define INC '['     // ++ префексный
#define DEC ']'		// --перфексный

#define POST_INC '^'     // постфиксный++ 
#define POST_DEC '_'	 // постфиксный--

#define FST_BYTE FST::FST _byte(str,\
	5,\
	FST::NODE(1,FST::RELATION('b',1)),\
	FST::NODE(1,FST::RELATION('y',2)),\
	FST::NODE(1,FST::RELATION('t',3)),\
	FST::NODE(1,FST::RELATION('e',4)),\
	FST::NODE()\
);
#define FST_INT FST::FST _int(str,\
	4,\
	FST::NODE(1, FST::RELATION('i', 1)),\
	FST::NODE(1, FST::RELATION('n', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE()\
);

#define FST_TORF FST::FST _torf(str,\
	5,\
	FST::NODE(1,FST::RELATION('t',1)),\
	FST::NODE(1,FST::RELATION('o',2)),\
	FST::NODE(1,FST::RELATION('r',3)),\
	FST::NODE(1,FST::RELATION('f',4)),\
	FST::NODE()\
);

#define FST_SYMB FST::FST _symb(str,\
	5,\
	FST::NODE(1, FST::RELATION('s', 1)),\
	FST::NODE(1, FST::RELATION('y', 2)),\
	FST::NODE(1, FST::RELATION('m', 3)),\
	FST::NODE(1, FST::RELATION('b', 4)),\
	FST::NODE()\
);
#define FST_STR FST::FST _string(str,\
	4,\
	FST::NODE(1, FST::RELATION('s', 1)),\
	FST::NODE(1, FST::RELATION('t', 2)),\
	FST::NODE(1, FST::RELATION('r', 3)),\
	FST::NODE()\
);
#define FST_TASK FST::FST _task(str,\
	5,\
	FST::NODE(1, FST::RELATION('t', 1)),\
	FST::NODE(1, FST::RELATION('a', 2)),\
	FST::NODE(1, FST::RELATION('s', 3)),\
	FST::NODE(1, FST::RELATION('k', 4)),\
	FST::NODE()\
);
#define FST_DEF FST::FST _def(str,\
	4,\
	FST::NODE(1, FST::RELATION('d', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('f', 3)),\
	FST::NODE()\
);
#define FST_RESPONSE FST::FST _response(str,\
	9,\
	FST::NODE(1, FST::RELATION('r', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('s', 3)),\
	FST::NODE(1, FST::RELATION('p', 4)),\
	FST::NODE(1, FST::RELATION('o', 5)),\
	FST::NODE(1, FST::RELATION('n', 6)),\
	FST::NODE(1, FST::RELATION('s', 7)),\
	FST::NODE(1, FST::RELATION('e', 8)),\
	FST::NODE()\
);

#define FST_WHEN FST::FST _when(str, \
    5, \
    FST::NODE(1, FST::RELATION('w', 1)), \
    FST::NODE(1, FST::RELATION('h', 2)), \
	FST::NODE(1, FST::RELATION('e', 3)), \
    FST::NODE(1, FST::RELATION('n', 4)), \
    FST::NODE() \
);

#define FST_OTHER FST::FST _other(str, \
	6, \
	FST::NODE(1, FST::RELATION('o', 1)), \
    FST::NODE(1, FST::RELATION('t', 2)), \
	FST::NODE(1, FST::RELATION('h', 3)), \
    FST::NODE(1, FST::RELATION('e', 4)), \
    FST::NODE(1, FST::RELATION('r', 5)), \
	FST::NODE() \
);


#define FST_STRCPR FST::FST _strcpr(str, \
    7, \
    FST::NODE(1, FST::RELATION('s', 1)), \
    FST::NODE(1, FST::RELATION('t', 2)), \
    FST::NODE(1, FST::RELATION('r', 3)), \
    FST::NODE(1, FST::RELATION('c', 4)), \
    FST::NODE(1, FST::RELATION('p', 5)), \
    FST::NODE(1, FST::RELATION('r', 6)), \
    FST::NODE() \
);

#define FST_ATOII FST::FST _atoli(str, \
    6, \
    FST::NODE(1, FST::RELATION('a', 1)), \
    FST::NODE(1, FST::RELATION('t', 2)), \
    FST::NODE(1, FST::RELATION('o', 3)), \
    FST::NODE(1, FST::RELATION('l', 4)), \
    FST::NODE(1, FST::RELATION('i', 5)), \
    FST::NODE() \
);


#define FST_BEGIN FST::FST _begin(str,\
	6,\
	FST::NODE(1, FST::RELATION('b', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('g', 3)),\
	FST::NODE(1, FST::RELATION('i', 4)),\
	FST::NODE(1, FST::RELATION('n', 5)),\
	FST::NODE()\
);
#define FST_COUT FST::FST _cout(str,\
	5,\
	FST::NODE(1, FST::RELATION('c', 1)),\
	FST::NODE(1, FST::RELATION('o', 2)),\
	FST::NODE(1, FST::RELATION('u', 3)),\
	FST::NODE(1, FST::RELATION('t', 4)),\
	FST::NODE()\
);

// === ƒќЅј¬Ћя≈ћ Ё“ќ (FST) ===
#define FST_SWITCH FST::FST _switch(str, \
    7, \
    FST::NODE(1, FST::RELATION('s', 1)), \
    FST::NODE(1, FST::RELATION('w', 2)), \
    FST::NODE(1, FST::RELATION('i', 3)), \
    FST::NODE(1, FST::RELATION('t', 4)), \
    FST::NODE(1, FST::RELATION('c', 5)), \
    FST::NODE(1, FST::RELATION('h', 6)), \
    FST::NODE() \
);

#define FST_CASE FST::FST _case(str, \
    3, \
    FST::NODE(1, FST::RELATION('i', 1)), \
    FST::NODE(1, FST::RELATION('s', 2)), \
    FST::NODE() \
);

#define FST_DEFAULT FST::FST _default(str, \
    4, \
    FST::NODE(1, FST::RELATION('a', 1)), \
    FST::NODE(1, FST::RELATION('n', 2)), \
    FST::NODE(1, FST::RELATION('y', 3)), \
    FST::NODE() \
);
// ==========================
#define FST_LITERAL FST::FST literal_int(str,\
	4,\
    FST::NODE(22, \
        FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('0', 3), FST::RELATION('1', 3), \
        FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), \
        FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), \
        FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), \
        FST::RELATION('6', 3), FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3)), \
    FST::NODE(24, \
        FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('0', 3), FST::RELATION('1', 3), \
        FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), \
        FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), \
        FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), \
        FST::RELATION('6', 3), FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3), \
        FST::RELATION('b', 3), FST::RELATION('d', 3)), \
    FST::NODE(21, \
        FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), \
        FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), \
        FST::RELATION('8', 2), FST::RELATION('9', 2), \
        FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), \
        FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3), FST::RELATION('7', 3), \
        FST::RELATION('8', 3), FST::RELATION('9', 3), \
        FST::RELATION('d', 3)), \
    FST::NODE() \
);
#define FST_IDENF FST::FST idenf(str,\
	2,\
	FST::NODE(119,\
		FST::RELATION('a', 1), FST::RELATION('a', 0), FST::RELATION('b', 1), FST::RELATION('b', 0),\
		FST::RELATION('c', 1), FST::RELATION('c', 0), FST::RELATION('d', 1), FST::RELATION('d', 0), FST::RELATION('e', 1), FST::RELATION('e', 0),\
		FST::RELATION('f', 1), FST::RELATION('f', 0), FST::RELATION('g', 1), FST::RELATION('g', 0), FST::RELATION('h', 0), FST::RELATION('h', 1), FST::RELATION('i', 0), FST::RELATION('i', 1),\
		FST::RELATION('j', 0), FST::RELATION('j', 1), FST::RELATION('k', 0), FST::RELATION('k', 1), FST::RELATION('l', 0), FST::RELATION('l', 1),\
		FST::RELATION('m', 0), FST::RELATION('m', 1), FST::RELATION('n', 0), FST::RELATION('n', 1), FST::RELATION('o', 0), FST::RELATION('o', 1),\
		FST::RELATION('p', 0), FST::RELATION('p', 1), FST::RELATION('q', 0), FST::RELATION('q', 1), FST::RELATION('r', 0), FST::RELATION('r', 1),\
		FST::RELATION('s', 0), FST::RELATION('s', 1), FST::RELATION('t', 0), FST::RELATION('t', 1), FST::RELATION('u', 0), FST::RELATION('u', 1),\
		FST::RELATION('v', 0), FST::RELATION('v', 1), FST::RELATION('w', 0), FST::RELATION('w', 1), FST::RELATION('x', 0), FST::RELATION('x', 1),\
		FST::RELATION('y', 0), FST::RELATION('y', 1), FST::RELATION('z', 0), FST::RELATION('z', 1),FST::RELATION('A', 1), FST::RELATION('A', 0), FST::RELATION('B', 1), FST::RELATION('B', 0),\
		FST::RELATION('C', 1), FST::RELATION('C', 0), FST::RELATION('D', 1), FST::RELATION('D', 0), FST::RELATION('E', 1), FST::RELATION('E', 0),\
		FST::RELATION('F', 1), FST::RELATION('F', 0), FST::RELATION('G', 1), FST::RELATION('G', 0), FST::RELATION('H', 0), FST::RELATION('H', 1), FST::RELATION('I', 0), FST::RELATION('I', 1),\
		FST::RELATION('J', 0), FST::RELATION('J', 1), FST::RELATION('K', 0), FST::RELATION('K', 1), FST::RELATION('L', 0), FST::RELATION('L', 1),\
		FST::RELATION('M', 0), FST::RELATION('M', 1), FST::RELATION('N', 0), FST::RELATION('N', 1), FST::RELATION('O', 0), FST::RELATION('O', 1),\
		FST::RELATION('P', 0), FST::RELATION('P', 1), FST::RELATION('Q', 0), FST::RELATION('Q', 1), FST::RELATION('R', 0), FST::RELATION('R', 1),\
		FST::RELATION('S', 0), FST::RELATION('S', 1), FST::RELATION('T', 0), FST::RELATION('T', 1), FST::RELATION('U', 0), FST::RELATION('U', 1),\
		FST::RELATION('V', 0), FST::RELATION('V', 1), FST::RELATION('W', 0), FST::RELATION('W', 1), FST::RELATION('X', 0), FST::RELATION('X', 1),\
		FST::RELATION('Y', 0), FST::RELATION('Y', 1), FST::RELATION('Z', 0), FST::RELATION('Z', 1), FST::RELATION('0', 1), FST::RELATION('1', 1),\
		FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),\
		FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1),\
		FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('_', 1)),\
	FST::NODE()\
);


namespace LA
{

	struct LEX
	{
		IT::idTable idTable;
		LT::lexTable lexTable;
	};

	struct FstLexeme {
		FST::FST& fst;
		int lexeme;
		bool* flag;
	};

	char FST(char* str);
	LEX LA(Parm::PARM parm, In::IN in);
}