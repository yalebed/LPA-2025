#include "stdafx.h"

namespace In {
    IN getin(wchar_t infile[]) {
        IN in;
        int cols = 1, error_pos = 0;
        in.text = new unsigned char[IN_MAX_LEN_TEXT];
        in.forbiddenChar = new unsigned char[IN_MAX_LEN_TEXT];
        in.errorLine = new int[IN_MAX_LEN_TEXT];
        in.errorCol = new int[IN_MAX_LEN_TEXT];

        bool isSingleQuotesOpened = false;
        bool isDoubleQuotesOpened = false;

        ifstream fin(infile);

        if (!fin.is_open()) {
            throw ERROR_THROW(110);
        }

        char* buff = new char[BUFF_SIZE];

        while (fin.getline(buff, BUFF_SIZE)) {

            in.lines++;
            cols = 1;
            int len = strlen(buff);

            for (int position = 0; position < len; position++) {

                unsigned char c = (unsigned char)buff[position];

                if (c == SINGLE_QUOTE) {
                    if (!isDoubleQuotesOpened) {
                        isSingleQuotesOpened = !isSingleQuotesOpened;
                    }
                }
                else if (c == DOUBLE_QUOTE) {
                    if (!isSingleQuotesOpened) {
                        isDoubleQuotesOpened = !isDoubleQuotesOpened;
                    }
                }
                bool insideQuotes = isSingleQuotesOpened || isDoubleQuotesOpened;

                switch (in.code[int(c)]) {

                case IN::T:
                    in.text[in.size++] = c;
                    cols++;
                    break;

                case IN::I:
                    if (!insideQuotes) {
                        in.ignore++;
                    }
                    else {
                        in.text[in.size++] = c;
                    }
                    cols++;
                    break;

                case IN::F:
                    //  внутри строки разрешены любые символы
                    // if (insideQuotes) { in.text[in.size++] = c; break; }

                    in.forbiddenChar[error_pos] = c;
                    in.text[in.size++] = '^';
                    in.errorLine[error_pos] = in.lines;
                    in.errorCol[error_pos++] = position + 1;
                    in.error_size++;
                    throw ERROR_THROW_IN(121, in.lines, cols);
                    break;

                case IN::P: 
                    if (insideQuotes) {
                        in.text[in.size++] = (unsigned)SPACE;
                    }
                    else if ((position > 0 && buff[position - 1] == SPACE) || position == 0 || position == len - 1) {
                        in.ignore++;
                    }
                    else {
                        in.text[in.size++] = (unsigned)SPACE;
                    }
                    break;

                case IN::S:

                    if (!insideQuotes) {
                        if (position > 0 && buff[position - 1] == SPACE && in.code[in.text[in.size - 1]] != IN::S) {
                            in.text[in.size - 1] = c;
                            in.ignore++;

                            if (buff[position + 1] == SPACE) {
                                position++;
                                in.ignore++;
                            }
                        }
                        else if (buff[position + 1] == SPACE) {
                            in.text[in.size++] = c;
                            position++;
                            in.ignore++;
                        }
                        else {
                            in.text[in.size++] = c;
                        }
                    }
                    else {
                        in.text[in.size++] = c;
                    }
                    break;

                default:
                    in.text[in.size++] = static_cast<unsigned char>(in.code[buff[position]]);
                    cols++;
                    break;
                }
            }
            if (!fin.eof())
            {
                in.text[in.size++] = '\n';
            }
        }

        in.text[in.size] = '\0';
        fin.close();
        delete[] buff;

        if (isSingleQuotesOpened || isDoubleQuotesOpened) {
            throw ERROR_THROW(122);
        }

        return in;
    }

    void deleteIn(IN in)
    {
        delete[] in.text;
        delete[] in.forbiddenChar;
        delete[] in.errorLine;
        delete[] in.errorCol;
    }
}