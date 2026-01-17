#include "PolishNotation.h"
#include "Error.h"

map<char, int> Priorities = {
    {'(', 0}, {')', 0},
    {'~', 4}, {'{', 4}, {'}', 4}, {'[', 4}, {']', 4}, {'^', 4}, {'_', 4},
    {'*', 3}, {'/', 3},
    {'+', 2}, {'-', 2},
    {'>', 1}, {'<', 1}, {'&', 1}, {'|', 1}, {',', 1}, {':', 1},
};

namespace PN {

    bool polishNotation(int i, LA::LEX& lex)
    {
        std::stack<LT::Entry> stack;
        std::queue<LT::Entry> queue;

        LT::Entry aggregate_symbol;
        aggregate_symbol.idxTI = -1;
        aggregate_symbol.lexema[0] = '#';
        aggregate_symbol.sn = lex.lexTable.table[i].sn;

        LT::Entry function_symbol;
        function_symbol.idxTI = LT_TI_NULLIDX;
        function_symbol.lexema[0] = '@';
        function_symbol.sn = lex.lexTable.table[i].sn;
        int idx = LT_TI_NULLIDX;

        int lexem_counter = 0;
        int parm_counter = 0;
        int lexem_position = i;

        char* buf = new char[21];

        bool is_task = false;

        if (lex.lexTable.table[i].lexema[0] == LEX_SEMICOLON) {
            delete[] buf;
            throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
        }

        for (; i < lex.lexTable.size && lex.lexTable.table[i].lexema[0] != LEX_SEMICOLON; i++, lexem_counter++) {

            char currentLexema = lex.lexTable.table[i].lexema[0];

            switch (currentLexema) {
            case LEX_ID:
            case LEX_LITERAL:
                if (lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F) {
                    is_task = true;
                    idx = lex.lexTable.table[i].idxTI;
                }
                else {
                    if (is_task)
                        parm_counter++;
                    queue.push(lex.lexTable.table[i]);
                }
                continue;

            case TILDE:
            case INC:
            case DEC:
            case POST_INC:
            case POST_DEC:
            case LEFTTHESIS:
                stack.push(lex.lexTable.table[i]);
                continue;

            case RIGHTTHESIS:
                while (stack.top().lexema[0] != LEFTTHESIS) {
                    queue.push(stack.top());
                    stack.pop();
                    if (stack.empty()) {
                        delete[] buf;
                        return false;
                    }
                }

                stack.pop(); // Удаляем '('

                if (is_task) {
                    function_symbol.idxTI = idx;
                    idx = LT_TI_NULLIDX;

                    LT::Entry tempFunc = function_symbol;
                    queue.push(tempFunc);

                    _itoa_s(parm_counter, buf, 21, 10);

                    LT::Entry countEntry;
                    countEntry.idxTI = LT_TI_NULLIDX;
                    countEntry.lexema[0] = buf[0];
                    countEntry.sn = function_symbol.sn;

                    queue.push(countEntry);

                    parm_counter = 0;
                    is_task = false;
                }
                continue;

            case LEX_OPERATION:
            {
                char currentOp = lex.lexTable.table[i].lexema[1];

                if (currentOp == '/') {
                    if (i + 1 < lex.lexTable.size) {

                        if (lex.lexTable.table[i + 1].lexema[0] == LEX_LITERAL) {
                            int idxTI = lex.lexTable.table[i + 1].idxTI;
                            IT::Entry entry = lex.idTable.table[idxTI];

                            bool isZero = false;
                            if (entry.iddatatype == IT::INT && entry.value.vint == 0) isZero = true;
                            if (entry.iddatatype == IT::BYTE && entry.value.vbyte == 0) isZero = true;

                            if (isZero) {
                                delete[] buf;
                                throw ERROR_THROW_IN(715, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
                            }
                        }
                        else if (lex.lexTable.table[i + 1].lexema[0] == LEX_LEFTTHESIS) {
                            if (i + 2 < lex.lexTable.size && lex.lexTable.table[i + 2].lexema[0] == LEX_LITERAL) {
                                int idxTI = lex.lexTable.table[i + 2].idxTI;
                                IT::Entry entry = lex.idTable.table[idxTI];

                                bool isZero = false;
                                if (entry.iddatatype == IT::INT && entry.value.vint == 0) isZero = true;
                                if (entry.iddatatype == IT::BYTE && entry.value.vbyte == 0) isZero = true;

                                if (isZero) {
                                    delete[] buf;
                                    throw ERROR_THROW_IN(715, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
                                }
                            }
                        }
                    }
                }

                while (!stack.empty()) {
                    char topOp = stack.top().lexema[0];
                    if (topOp == LEX_OPERATION) topOp = stack.top().lexema[1];

                    if (topOp == '(') break;

                    if (Priorities[topOp] >= Priorities[currentOp]) {
                        queue.push(stack.top());
                        stack.pop();
                    }
                    else {
                        break;
                    }
                }
                stack.push(lex.lexTable.table[i]);
                continue;
            }

            case LEX_STRCPR:
            case LEX_ATOII:
                is_task = true;
                idx = lex.lexTable.table[i].idxTI;
                continue;
            }
        }

        while (!stack.empty()) {
            if (stack.top().lexema[0] == LEFTTHESIS || stack.top().lexema[0] == RIGHTTHESIS) {
                delete[] buf;
                return false;
            }

            queue.push(stack.top());
            stack.pop();
        }

        if (queue.empty()) {
            delete[] buf;
            throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
        }

        while (lexem_counter != 0) {
            if (!queue.empty()) {
                lex.lexTable.table[lexem_position++] = queue.front();
                queue.pop();
            }
            else
                lex.lexTable.table[lexem_position++] = aggregate_symbol;

            lexem_counter--;
        }

        for (int k = 0; k < lexem_position; k++) {
            if (lex.lexTable.table[k].lexema[0] == LEX_OPERATION || lex.lexTable.table[k].lexema[0] == LEX_LITERAL) {
                if (lex.lexTable.table[k].idxTI != -1)
                    lex.idTable.table[lex.lexTable.table[k].idxTI].idxfirstLE = k;
            }
        }

        delete[] buf;
        return true;
    }

    bool startPolish(LA::LEX& lex)
    {
        bool result = false;
        for (int i = 0; i < lex.lexTable.size; i++) {
            if (lex.lexTable.table[i].lexema[0] == '=') {
                result = polishNotation(i + 1, lex);
                if (!result) {
                    return false;
                }
            }
        }
        return true;
    }
}