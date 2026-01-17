#include"LA.h"
#include"stdafx.h"
#include"In.h"
#include"Error.h"
#include <stdio.h>
#include <string.h>
#include <iomanip>

namespace LA
{
    LT::lexTable lexTable = LT::Create(LT_MAXSIZE - 1);
    IT::idTable idTable = IT::Create(TI_MAXSIZE - 1);

    bool byteFlag = false;
    bool intFlag = false;
    bool torfFlag = false;
    bool falseFlag = false;
    bool trueFlag = false;
    bool symbFlag = false;
    bool stringFlag = false;
    bool parmFlag = false;
    bool beginFlag = false;
    bool defFlag = false;
    bool keyWord = false;
    bool declareFunctionflag = false;
    bool addedToITFlag = false;
    bool literalFlag = false;
    char* str = new char[MAX_LEX_SIZE];

    char FST()
    {
        FST_BYTE
            FST_INT
            FST_TORF
            FST_SYMB
            FST_STR
            FST_TASK
            FST_DEF
            FST_RESPONSE
            FST_BEGIN
            FST_COUT
            FST_WHEN
            FST_OTHER
            FST_SWITCH
            FST_CASE
            FST_DEFAULT
            FST_STRCPR
            FST_ATOII
            FST_LITERAL
            FST_IDENF


            FstLexeme lexemes[] = {
            {_byte, LEX_BYTE, &byteFlag},
            {_int, LEX_INT, &intFlag},
            {_torf, LEX_TORF, &torfFlag},
            {_symb, LEX_SYMB, &symbFlag},
            {_string, LEX_STRING, &stringFlag},
            {_task, LEX_TASK, nullptr},
            {_def, LEX_DECLARE, nullptr},
            {_response, LEX_RESPONSE, nullptr},
            {_begin, LEX_BEGIN, &beginFlag},
            {_cout, LEX_COUT, nullptr},
            {_when, LEX_WHEN, nullptr},
            {_other, LEX_OTHER, nullptr},
            {_switch, LEX_SWITCH, nullptr},
            {_case, LEX_CASE, nullptr},
            {_default, LEX_DEFAULT, nullptr},
            {_strcpr, LEX_STRCPR, nullptr},
            {_atoli, LEX_ATOII, nullptr},
            {literal_int, LEX_LITERAL, nullptr},
            {idenf, LEX_ID, nullptr}

        };

        for (int i = 0; i < FST_AMOUNT; i++) {
            if (Execute(lexemes[i].fst)) {
                if (lexemes[i].flag) {
                    *lexemes[i].flag = true;
                    keyWord = true;
                }
                return lexemes[i].lexeme;
            }
        }

        return NULL;
    }

    LA::LEX LA(Parm::PARM parm, In::IN in)
    {
        LEX tables;
        int indexIT;
        int col = 0;
        LT::Entry current_entry_LT;
        int bufferIndex = 0;
        current_entry_LT.sn = 0;
        current_entry_LT.idxTI = 0;
        current_entry_LT.lexema[0] = NULL;
        stack<IT::Entry*> scope;
        scope.push(NULL);
        int number_literal = 0;
        IT::Entry current_entry_IT;
        lexTable.size = 0;
        int currentLine = 1;
        ofstream LT_file;
        ofstream IT_file;
        LT_file.open("LT.txt");
        IT_file.open("IT.txt");
        for (int i = 0; i < in.size; i++)
        {
            col++;
            if (!literalFlag && in.text[i] == '!' && i + 1 < in.size && in.text[i + 1] == '!')
            {
                while (i < in.size && in.text[i] != NEW_LINE) {
                    i++;
                }
                i--;
                continue;
            }
            if (in.code[(int)in.text[i]] == In::IN::T || in.text[i] == SINGLE_QUOTE || in.text[i] == DOUBLE_QUOTE || literalFlag)
            {
                str[bufferIndex++] = in.text[i];
                if (bufferIndex >= MAX_LEX_SIZE)
                {
                    throw ERROR_THROW(119);
                }

            }
            else
            {
                str[bufferIndex] = '\0';
                current_entry_LT.lexema[0] = FST();
                if (current_entry_LT.lexema[0] == NULL && str[0] == '-' && bufferIndex > 1) {
                    bool isNumber = true;
                    for (int k = 1; k < bufferIndex; k++) {
                        if (!isdigit(str[k]) && str[k] != 'd' && str[k] != 'b') {
                            isNumber = false;
                            break;
                        }
                    }
                    if (isNumber) {
                        current_entry_LT.lexema[0] = LEX_LITERAL;
                    }
                }
                if (lexTable.size > 0)
                {
                    char currentLex = current_entry_LT.lexema[0];
                    char prevLex = lexTable.table[lexTable.size - 1].lexema[0];

                    bool isPrevType = (prevLex == LEX_INT || prevLex == LEX_BYTE ||
                        prevLex == LEX_STRING || prevLex == LEX_SYMB ||
                        prevLex == LEX_TORF);

                    bool isForbidden = (
                        currentLex == LEX_INT || currentLex == LEX_BYTE ||
                        currentLex == LEX_STRING || currentLex == LEX_SYMB ||
                        currentLex == LEX_TORF ||
                        currentLex == LEX_DECLARE ||
                        currentLex == LEX_BEGIN ||
                        currentLex == LEX_RESPONSE ||
                        currentLex == LEX_COUT ||
                        currentLex == LEX_WHEN ||
                        currentLex == LEX_OTHER ||
                        currentLex == LEX_SWITCH ||
                        currentLex == LEX_CASE ||
                        currentLex == LEX_DEFAULT
                        );

                    if (isPrevType && isForbidden)
                    {
                        throw ERROR_THROW_IN(113, currentLine, col - bufferIndex);
                    }
                }

                switch (current_entry_LT.lexema[0])
                {
                case LEX_BEGIN: {
                    beginFlag = true;
                    current_entry_LT.idxTI = idTable.size;
                    memcpy(current_entry_IT.id, str, 5);
                    current_entry_IT.id[5] = '\0';
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.idtype = IT::M;
                    current_entry_IT.value.vint = NULL;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    current_entry_IT.scope = NULL;
                    indexIT = IT::search(idTable, current_entry_IT);
                    if (indexIT >= 0)
                    {
                        throw ERROR_THROW(116);
                    }
                    if (indexIT == -1)
                    {
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                    }

                    break;
                }
                case LEX_LITERAL: {
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.idtype = IT::L;

                    int len = strlen(str);
                    char lastChar = str[len - 1];
                    //int value = 0;

                    //if (lastChar == 'b') {
                    //    str[len - 1] = '\0'; //  'b'
                    //    value = strtol(str, nullptr, 2); 
                    //}
                    //else if (lastChar == 'd') {
                    //    str[len - 1] = '\0'; //  'd'
                    //    value = strtol(str, nullptr, 10); 
                    //}
                    //else {
                    //    value = atoi(str);
                    //}

                    //current_entry_IT.value.vint = value;

                    //current_entry_IT.iddatatype = IT::INT;
                    //if (trueFlag) {
                    //    current_entry_IT.iddatatype = IT::TORF;
                    //    current_entry_IT.value.vint = 1;
                    //}
                    //if (falseFlag) {
                    //    current_entry_IT.iddatatype = IT::TORF;
                    //    current_entry_IT.value.vint = 0;
                    //}
                    long long valCheck = 0; 

                    if (lastChar == 'b') {
                        str[len - 1] = '\0'; // 'b'
                        valCheck = strtoll(str, nullptr, 2);
                    }
                    else if (lastChar == 'd') {
                        str[len - 1] = '\0'; // 'd'
                        valCheck = strtoll(str, nullptr, 10);
                    }
                    else {
                        valCheck = strtoll(str, nullptr, 10);
                    }

                    if (valCheck > 2147483647LL || valCheck < -2147483648LL) {
                        throw ERROR_THROW_IN(126, currentLine, col - bufferIndex);
                    }

                    int value = (int)valCheck;
                    current_entry_IT.value.vint = value;

                    current_entry_IT.iddatatype = IT::INT;

                    if (trueFlag) {
                        current_entry_IT.iddatatype = IT::TORF;
                        current_entry_IT.value.vint = 1;
                    }
                    if (falseFlag) {
                        current_entry_IT.iddatatype = IT::TORF;
                        current_entry_IT.value.vint = 0;
                    }

                    indexIT = IT::search(idTable, current_entry_IT);
                    if (indexIT >= 0)
                    {
                        current_entry_LT.idxTI = indexIT;
                    }
                    else {
                        sprintf_s(current_entry_IT.id, "L%d", number_literal);
                        number_literal++;

                        if (trueFlag) {
                            strcpy_s(current_entry_IT.id, "true");
                            trueFlag = false;
                        }
                        if (falseFlag) {
                            strcpy_s(current_entry_IT.id, "false");
                            falseFlag = false;
                        }

                        current_entry_IT.line = currentLine;
                        current_entry_IT.idxfirstLE = lexTable.size;
                        current_entry_IT.scope = NULL;
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                        memset(current_entry_IT.id, NULL, ID_SIZE);
                        current_entry_IT.iddatatype = IT::INT;
                        current_entry_IT.value.vint = 0;
                        break;
                    }
                    memset(current_entry_IT.id, 0, ID_SIZE);
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.value.vint = 0;
                    break;

                }
                case LEX_ID: {
                    if (bufferIndex > ID_SIZE)
                    {
                        throw ERROR_THROW_IN(125, currentLine, col - bufferIndex);
                    }

                    if (scope.empty())
                        current_entry_IT.scope = NULL;
                    else
                        current_entry_IT.scope = scope.top();

                    current_entry_LT.idxTI = idTable.size;
                    memcpy(current_entry_IT.id, str, ID_SIZE);
                    current_entry_IT.id[ID_SIZE] = '\0';
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.value.vint = 0;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    current_entry_IT.idtype = IT::V;

                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_DECLARE)
                    {

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_SYMB && symbFlag)
                        {
                            current_entry_IT.iddatatype = IT::SYMB;
                            current_entry_IT.value.vsymb = '\0';
                            symbFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }


                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_TORF && torfFlag)
                        {
                            current_entry_IT.iddatatype = IT::TORF;
                            current_entry_IT.value.vtorf = 0;
                            torfFlag = false;
                        }

                        indexIT = IT::search(idTable, current_entry_IT);
                        if (indexIT != -1)
                        {
                            throw ERROR_THROW_IN(114, currentLine, col);
                        }
                        defFlag = false;
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                        addedToITFlag = true;
                    }

                    if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_TASK)
                    {
                        current_entry_IT.idtype = IT::F;
                        declareFunctionflag = true;

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_SYMB && symbFlag)
                        {
                            throw ERROR_THROW_IN(120, currentLine, col);
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_TORF && torfFlag)
                        {
                            current_entry_IT.iddatatype = IT::TORF;
                            current_entry_IT.value.vtorf = 0;
                            torfFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }

                        indexIT = IT::search(idTable, current_entry_IT);
                        if (indexIT != -1)
                        {
                            throw ERROR_THROW_IN(114, currentLine, col);
                        }
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                        addedToITFlag = true;
                    }

                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_LEFTTHESIS &&
                        lexTable.table[lexTable.size - 3].lexema[0] == LEX_ID &&
                        lexTable.table[lexTable.size - 3].idxTI == idTable.size - 1 &&
                        idTable.table[idTable.size - 1].idtype == IT::F)
                    {
                        current_entry_IT.idtype = IT::P;

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_SYMB && symbFlag)
                        {
                            current_entry_IT.iddatatype = IT::SYMB;
                            current_entry_IT.value.vsymb = '\0';
                            symbFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }
                        indexIT = IT::search(idTable, current_entry_IT);
                        if (indexIT != -1)
                        {
                            throw ERROR_THROW_IN(114, currentLine, col);
                        }
                        current_entry_LT.idxTI = idTable.size;
                        IT::Add(idTable, current_entry_IT);
                        addedToITFlag = true;
                        intFlag = false;
                        byteFlag = false;
                        torfFlag = false;
                        symbFlag = false;
                        stringFlag = false;
                    }

                    if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_COMMA && idTable.table[lexTable.table[lexTable.size - 2].idxTI].idtype == IT::P)
                    {
                        current_entry_IT.idtype = IT::P;

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_BYTE && byteFlag)
                        {
                            current_entry_IT.iddatatype = IT::BYTE;
                            current_entry_IT.value.vbyte = 0;
                            byteFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_SYMB && symbFlag)
                        {
                            current_entry_IT.iddatatype = IT::SYMB;
                            current_entry_IT.value.vsymb = '\0';
                            symbFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_INT && intFlag)
                        {
                            current_entry_IT.iddatatype = IT::INT;
                            current_entry_IT.value.vint = 0;
                            intFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_TORF && torfFlag)
                        {
                            current_entry_IT.iddatatype = IT::TORF;
                            current_entry_IT.value.vtorf = 0;
                            torfFlag = false;
                        }

                        if (lexTable.table[lexTable.size - 1].lexema[0] == LEX_STRING && stringFlag)
                        {
                            current_entry_IT.iddatatype = IT::STR;
                            strcpy_s(current_entry_IT.value.vstr->str, "");
                            stringFlag = false;
                        }

                        indexIT = IT::search(idTable, current_entry_IT);

                        if (indexIT != -1)
                        {
                            throw ERROR_THROW_IN(114, currentLine, col);
                        }

                        IT::Add(idTable, current_entry_IT);
                        addedToITFlag = true;
                        intFlag = false;
                        byteFlag = false;
                        torfFlag = false;
                        symbFlag = false;
                        stringFlag = false;
                    }

                    if (!addedToITFlag)
                    {
                        indexIT = IT::search(idTable, current_entry_IT);

                        if (indexIT >= 0)
                        {

                            current_entry_LT.idxTI = indexIT;
                        }
                        else {
                            if (lexTable.size > 0) {
                                char prevLex = lexTable.table[lexTable.size - 1].lexema[0];

                                if (prevLex == LEX_COUT || prevLex == LEX_SWITCH || prevLex == LEX_WHEN) {
                                    throw ERROR_THROW_IN(613, currentLine, col); 
                                }

                                bool isBegin = (prevLex == LEX_BEGIN) || (prevLex == 'b') || (prevLex == 'm');
                                bool isColon = (prevLex == LEX_COLON) || (prevLex == ':');
                                bool isOther = (prevLex == LEX_OTHER);

                                if (isBegin || isOther || isColon) {
                                    throw ERROR_THROW_IN(610, currentLine, col);
                                }

                                for (int k = lexTable.size - 1; k >= 0; k--) {
                                    char tok = lexTable.table[k].lexema[0];

                                    if (tok == LEX_BEGIN || tok == 'b' || tok == 'm') {
                                        if (k == lexTable.size - 1 || lexTable.table[k + 1].lexema[0] != LEX_LEFTBRACE) {
                                            throw ERROR_THROW_IN(610, lexTable.table[k].sn, lexTable.table[k].col);
                                        }
                                        break;
                                    }

                                    if (tok == LEX_RIGHTBRACE) break;
                                }
                            }

                            throw ERROR_THROW_IN(115, currentLine, col);
                        }
                    }

                    memset(current_entry_IT.id, NULL, ID_SIZE);
                    current_entry_IT.iddatatype = IT::INT;
                    current_entry_IT.value.vint = NULL;
                    addedToITFlag = false;
                    break;
                }

                case LEX_DECLARE: {
                    defFlag = true;
                    break;
                }
                case LEX_STRCPR:
                case LEX_ATOII: {
                    current_entry_LT.idxTI = idTable.size;
                    memcpy(current_entry_IT.id, str, ID_SIZE);
                    current_entry_IT.id[ID_SIZE] = '\0';
                    current_entry_IT.iddatatype = IT::BYTE;
                    current_entry_IT.idtype = IT::SF;
                    current_entry_IT.value.vint = NULL;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;

                    current_entry_LT.idxTI = idTable.size;
                    IT::Add(idTable, current_entry_IT);

                    break;
                }
                }

                if (current_entry_LT.lexema[0] == NULL && str[0] != '\0') {
                    throw ERROR_THROW_IN(113, currentLine, col);
                }

                bufferIndex = 0;
                memset(str, 0, sizeof(str));
            }
            if (current_entry_LT.lexema[0] != NULL)
            {
                current_entry_LT.sn = currentLine;
                current_entry_LT.col = col;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;

            }
            if (keyWord && lexTable.size >= 2)
            {
                char prevLex = lexTable.table[lexTable.size - 2].lexema[0];

                bool isPrevType = (prevLex == LEX_INT || prevLex == LEX_BYTE ||
                    prevLex == LEX_STRING || prevLex == LEX_SYMB ||
                    prevLex == LEX_TORF);

                if (isPrevType) {
                    throw ERROR_THROW(113, currentLine, col);
                }
            }

            /*if (lexTable.table[lexTable.size - 4].lexema[0] == LEX_DECLARE && keyWord) {
                throw ERROR_THROW(113);
            }*/

            keyWord = false;

            if ((literalFlag && in.text[i] != '\'') && (literalFlag && in.text[i] != '"'))
                continue;

            switch (in.text[i])
            {

            case SINGLE_QUOTE: {
                if (literalFlag) {
                    if (bufferIndex == 2) {
                        current_entry_IT.value.vsymb = '\0';
                    }
                    else if (bufferIndex == 3) {
                        current_entry_IT.value.vsymb = str[1]; 
                    }
                    else {
                        throw ERROR_THROW_IN(123, currentLine, col);
                    }

                    literalFlag = false; 

                    current_entry_IT.iddatatype = IT::SYMB;
                    number_literal++;

                    current_entry_LT.idxTI = idTable.size;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    current_entry_LT.sn = currentLine;

                    sprintf_s(current_entry_IT.id, "C%d", number_literal);
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;

                    if (!scope.empty()) current_entry_IT.scope = scope.top();
                    else current_entry_IT.scope = NULL;

                    LT::Add(lexTable, current_entry_LT);
                    IT::Add(idTable, current_entry_IT);

                    memset(str, 0, MAX_LEX_SIZE);
                    memset(current_entry_IT.id, 0, ID_SIZE);
                    bufferIndex = 0;
                    current_entry_LT.lexema[0] = NULL;
                }
                else {
                    literalFlag = true;
                }
                break;
            }
            case DOUBLE_QUOTE: {

                literalFlag = true;
                if (str[0] == DOUBLE_QUOTE && bufferIndex != 1)
                {

                    if (bufferIndex > MAX_STR_SIZE) {
                        throw ERROR_THROW_IN(124, currentLine, col);
                    }

                    current_entry_IT.iddatatype = IT::STR;
                    for (int i = 0; i < bufferIndex; i++)
                    {
                        current_entry_IT.value.vstr->str[i] = str[i];
                    }
                    current_entry_IT.value.vstr->str[bufferIndex] = '\0';
                    current_entry_IT.value.vstr->len = bufferIndex;
                    number_literal++;

                    current_entry_LT.idxTI = idTable.size;
                    str[bufferIndex] = '\0';
                    literalFlag = false;
                    current_entry_LT.lexema[0] = LEX_LITERAL;
                    sprintf_s(current_entry_IT.id, "L%d", number_literal);
                    current_entry_IT.idtype = IT::L;
                    current_entry_IT.line = currentLine;
                    current_entry_IT.idxfirstLE = lexTable.size;

                    current_entry_LT.sn = currentLine;
                    if (!scope.empty())
                        current_entry_IT.scope = scope.top();
                    else
                        current_entry_IT.scope = NULL;
                    number_literal++;
                    LT::Add(lexTable, current_entry_LT);
                    IT::Add(idTable, current_entry_IT);
                    memset(str, 0, MAX_LEX_SIZE);
                    memset(current_entry_IT.id, 0, ID_SIZE);

                    bufferIndex = 0;
                    current_entry_LT.lexema[0] = NULL;

                    break;
                }
                /*bufferIndex++;*/
                break;
            }
            case NEW_LINE: {
                col = 0;
                current_entry_LT.sn = currentLine++;
                current_entry_LT.lexema[0] = NULL;
                break;
            }
            case SEMICOLON: {
                if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] == LEX_EQUAL) {
                    throw ERROR_THROW_IN(602, currentLine, col);
                }
                current_entry_LT.lexema[0] = LEX_SEMICOLON;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            }
            case LEFT_BRACE: {

                current_entry_LT.lexema[0] = LEX_LEFTBRACE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;

                if (beginFlag)
                {
                    if (idTable.table[idTable.size - 1].idtype != IT::L)
                    {
                        scope.push(&idTable.table[idTable.size - 1]);
                    }
                    else {
                        for (int j = idTable.size - 1; j >= 0; j--)
                        {
                            if (idTable.table[j].idtype == IT::M)
                            {
                                scope.push(&idTable.table[j]);
                                break;
                            }
                        }
                    }
                }
                else {
                    for (int j = idTable.size - 1; j >= 0; j--)
                    {
                        if (idTable.table[j].idtype == IT::F)
                        {
                            scope.push(&idTable.table[j]);
                            break;
                        }
                    }
                }
                break;
            }
            case RIGHT_BRACE: {
                current_entry_LT.lexema[0] = LEX_RIGHTBRACE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (!scope.empty())
                    scope.pop();

                break;
            }
            case LEFTTHESIS: {
                current_entry_LT.lexema[0] = LEX_LEFTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                if (declareFunctionflag)
                {
                    for (int j = idTable.size - 1; j >= 0; j--)
                    {
                        if (idTable.table[j].idtype == IT::F)
                        {
                            scope.push(&idTable.table[j]);
                            break;
                        }
                    }
                }
                break;
            }
            case RIGHTTHESIS: {
                if (lexTable.size > 0 && lexTable.table[lexTable.size - 1].lexema[0] == LEX_LEFTTHESIS) {
                    throw ERROR_THROW_IN(602, currentLine, col);
                }
                current_entry_LT.lexema[0] = LEX_RIGHTTHESIS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                parmFlag = false;
                if (!scope.empty() && declareFunctionflag)
                {
                    scope.pop();
                    declareFunctionflag = false;
                }
                break;
            }

            case COMMA:
            case EQUAL:
            case TILDE:
            case LEX_COLON: {
                current_entry_LT.lexema[0] = in.text[i];
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            }
            case AMPERSAND: {
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = AMPERSAND;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            }
            case PIPE: {
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = PIPE;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            }
            case PLUS:
            {
                if (i + 1 < in.size && in.text[i + 1] == PLUS) {
                    bool isPostfix = false;
                    if (lexTable.size > 0) {
                        char prevLex = lexTable.table[lexTable.size - 1].lexema[0];
                        if (prevLex == LEX_ID || prevLex == LEX_RIGHTTHESIS || prevLex == LEX_LITERAL) {
                            isPostfix = true;
                        }
                    }

                    if (isPostfix)
                        current_entry_LT.lexema[0] = POST_INC; // '^'
                    else
                        current_entry_LT.lexema[0] = INC;      // '['

                    current_entry_LT.sn = currentLine;
                    LT::Add(lexTable, current_entry_LT);
                    current_entry_LT.lexema[0] = NULL;
                    i++;
                }
                else {
                    current_entry_LT.lexema[0] = LEX_OPERATION;
                    current_entry_LT.lexema[1] = PLUS;
                    current_entry_LT.sn = currentLine;
                    LT::Add(lexTable, current_entry_LT);
                    current_entry_LT.lexema[0] = NULL;
                    current_entry_LT.lexema[1] = NULL;
                }
                break;
            }
            case MINUS:
            {
                bool isUnaryContext = false;
                if (lexTable.size == 0) isUnaryContext = true;
                else {
                    char prev = lexTable.table[lexTable.size - 1].lexema[0];
                    if (prev == LEX_EQUAL || prev == LEX_LEFTTHESIS || prev == LEX_RESPONSE || prev == LEX_COMMA || prev == LEX_OPERATION || prev == LEX_COMPARISON || prev == LEX_COLON) {
                        isUnaryContext = true;
                    }
                }

                if (isUnaryContext && i + 1 < in.size && isdigit(in.text[i + 1])) {
                    str[bufferIndex++] = '-';
                    continue;
                }
                if (i + 1 < in.size && in.text[i + 1] == MINUS) {
                    bool isPostfix = false;
                    if (lexTable.size > 0) {
                        char prevLex = lexTable.table[lexTable.size - 1].lexema[0];
                        if (prevLex == LEX_ID || prevLex == LEX_RIGHTTHESIS || prevLex == LEX_LITERAL) {
                            isPostfix = true;
                        }
                    }

                    if (isPostfix)
                        current_entry_LT.lexema[0] = POST_DEC; // '_'
                    else
                        current_entry_LT.lexema[0] = DEC;      // ']'

                    current_entry_LT.sn = currentLine;
                    LT::Add(lexTable, current_entry_LT);
                    current_entry_LT.lexema[0] = NULL;
                    i++;
                }
                else {
                    current_entry_LT.lexema[0] = LEX_OPERATION;
                    current_entry_LT.lexema[1] = MINUS;
                    current_entry_LT.sn = currentLine;
                    LT::Add(lexTable, current_entry_LT);
                    current_entry_LT.lexema[0] = NULL;
                    current_entry_LT.lexema[1] = NULL;
                }
                break;
            }
            case STAR:
            {
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = STAR;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            }
            case DIRSLASH:
            {
                current_entry_LT.lexema[0] = LEX_OPERATION;
                current_entry_LT.lexema[1] = DIRSLASH;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                current_entry_LT.lexema[1] = NULL;
                break;
            }

            case GREATER:
                memset(current_entry_LT.lexema, 0, 5);
                current_entry_LT.lexema[0] = LEX_COMPARISON;
                current_entry_LT.lexema[1] = GREATER;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            case LESS:
            {
                memset(current_entry_LT.lexema, 0, 5);
                current_entry_LT.lexema[0] = LEX_COMPARISON;
                current_entry_LT.lexema[1] = LESS;
                current_entry_LT.sn = currentLine;
                LT::Add(lexTable, current_entry_LT);
                current_entry_LT.lexema[0] = NULL;
                break;
            }
            }
        }

        currentLine = 1;
        LT_file << currentLine;
        LT_file << '\t';
        for (int i = 0; i < lexTable.size; i++)
        {
            current_entry_LT = LT::GetEntry(lexTable, i);
            if (currentLine != current_entry_LT.sn)
            {
                currentLine = current_entry_LT.sn;
                LT_file << '\n';
                LT_file << currentLine;
                LT_file << '\t';
            }

            LT_file << current_entry_LT.lexema[0];
        }
        LT_file.close();
        IT_file << std::left
            << std::setw(15) << "id"
            << std::setw(10) << "datatype"
            << std::setw(10) << "idtype"
            << std::setw(10) << "Line"
            << std::setw(15) << "Scope"
            << std::setw(15) << "value" << std::endl;

        for (int i = 0; i < idTable.size; i++) {
            IT::Entry temp_entry = IT::GetEntry(idTable, i);

            std::string id_line = std::string(temp_entry.id) + "_" + std::to_string(temp_entry.line);

            IT_file << std::setw(15) << id_line;

            switch (temp_entry.iddatatype) {
            case 1: IT_file << std::setw(10) << "INT"; break;
            case 2: IT_file << std::setw(10) << "STR"; break;
            case 3: IT_file << std::setw(10) << "BYTE"; break;
            case 4: IT_file << std::setw(10) << "SYMB"; break;
            case 5: IT_file << std::setw(10) << "TORF"; break;
            }

            switch (temp_entry.idtype) {
            case IT::V:  IT_file << std::setw(10) << "V"; break;
            case IT::L:  IT_file << std::setw(10) << "L"; break;
            case IT::F:  IT_file << std::setw(10) << "F"; break;
            case IT::P:  IT_file << std::setw(10) << "P"; break;
            case IT::SF: IT_file << std::setw(10) << "SF"; break;
            case IT::M:  IT_file << std::setw(10) << "M"; break;
            }

            IT_file << std::setw(10) << temp_entry.line;

            if (temp_entry.scope != NULL) {
                IT_file << std::setw(15) << temp_entry.scope->id;
            }
            else {
                IT_file << std::setw(15) << "-";
            }

            if (temp_entry.iddatatype == IT::BYTE) IT_file << std::setw(15) << int(temp_entry.value.vbyte);
            else if (temp_entry.iddatatype == IT::INT) IT_file << std::setw(15) << temp_entry.value.vint;
            else if (temp_entry.iddatatype == IT::TORF) IT_file << std::setw(15) << temp_entry.value.vtorf;
            else if (temp_entry.iddatatype == IT::SYMB) {
                std::string symb = "'" + std::string(1, temp_entry.value.vsymb) + "'";
                IT_file << std::setw(15) << symb;
            }
            else if (temp_entry.iddatatype == IT::STR) {
                IT_file << std::setw(15) << temp_entry.value.vstr->str;
            }

            IT_file << std::endl;
        }
        IT_file.close();


        tables.idTable = idTable;
        tables.lexTable = lexTable;
        return tables;
    }
}