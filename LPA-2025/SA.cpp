#include "SA.h"

namespace SA {

	void operands(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {

			if (lex.lexTable.table[i].lexema[0] == LEX_COMPARISON || lex.lexTable.table[i].lexema[0] == LEX_EQUAL)
			{
				if (lex.lexTable.table[i].lexema[0] == LEX_EQUAL) {

					bool hasValue = false;
					int k = 1;
					while ((i + k) < lex.lexTable.size && lex.lexTable.table[i + k].lexema[0] != LEX_SEMICOLON) {
						char token = lex.lexTable.table[i + k].lexema[0];
						if (token == LEX_ID || token == LEX_LITERAL) {
							hasValue = true;
							break;
						}
						k++;
					}

					// дошли до точки с запятой, но не нашли значений — ошибка
					if (!hasValue) {
						throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
					}
					int cur = -1;
					IT::IDDATATYPE datatype = (IT::IDDATATYPE)0;
					
					while (lex.lexTable.table[i + cur].lexema[0] != LEX_SEMICOLON) {
						
						char currentLexema = lex.lexTable.table[i + cur].lexema[0];

						if (currentLexema == LEX_ID || currentLexema == LEX_LITERAL)
						{
							IT::IDDATATYPE currentType = lex.idTable.table[lex.lexTable.table[i + cur].idxTI].iddatatype;

							if (datatype == (IT::IDDATATYPE)0) {
								datatype = currentType;
							}
							else {
								// Проверка совместимости типов
								bool compatible = (datatype == currentType) ||
									(datatype == IT::BYTE && currentType == IT::INT) ||
									(datatype == IT::INT && currentType == IT::BYTE);

								if (!compatible && datatype == IT::TORF && currentType == IT::INT) {
									if (currentLexema == LEX_LITERAL) {
										int val = lex.idTable.table[lex.lexTable.table[i + cur].idxTI].value.vint;
										if (val == 0 || val == 1) {
											compatible = true;
										}
									}
								}

								if (!compatible) {
									throw ERROR_THROW_IN(703, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI);
								}
							}

							// Пропускаем аргументы пользовательских функций
							if (lex.idTable.table[lex.lexTable.table[i + cur].idxTI].idtype == IT::F) {
								while (lex.lexTable.table[i + cur].lexema[0] != LEX_RIGHTTHESIS) {
									cur++;
								}
							}
						}
						else if (currentLexema == LEX_OPERATION) {

							if (datatype == IT::STR) {
								throw ERROR_THROW_IN(716, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].col);
							}
							if (datatype == IT::SYMB) {
								throw ERROR_THROW_IN(716, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].col);
							}
						}
						// пропускаем аргументы стандартных функций
						else if (currentLexema == LEX_STRCPR || currentLexema == LEX_ATOII) {
							while (lex.lexTable.table[i + cur].lexema[0] != LEX_RIGHTTHESIS) {
								cur++;
							}
						}

						// Проверки на недопустимые операции
						if (datatype == IT::STR && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}
						if (datatype == IT::SYMB && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0 && lex.lexTable.table[i + cur].lexema[1] != '+' && lex.lexTable.table[i + cur].lexema[1] != '-') {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}

						if (datatype == IT::TORF && lex.lexTable.table[i + cur].lexema[0] == LEX_COMPARISON && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI)
						}
						cur++;
					}
					i += cur - 1;
				}
			}
		}
	}

	void functions(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_TASK) {
				if (i == 0) {
					// Если task стоит самым первым в файле — точно нет типа
					throw ERROR_THROW_IN(609, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}

				char prevLex = lex.lexTable.table[i - 1].lexema[0];
				bool isType = (prevLex == LEX_INT || prevLex == LEX_BYTE ||
					prevLex == LEX_STRING || prevLex == LEX_SYMB ||
					prevLex == LEX_TORF);

				if (!isType) {
					throw ERROR_THROW_IN(609, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
			if (lex.lexTable.table[i].lexema[0] == LEX_COUT) {
				if (lex.lexTable.table[i + 2].lexema[0] == ')') {
					throw ERROR_THROW_IN(709, lex.lexTable.table[i + 2].sn, lex.lexTable.table[i + 2].idxTI);
				}
			}

			if (lex.lexTable.table[i].lexema[0] == LEX_ID && lex.lexTable.table[i - 1].lexema[0] == LEX_TASK && lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F)
			{
				int cur = 1;
				IT::IDDATATYPE returnType = lex.idTable.table[lex.lexTable.table[i].idxTI].iddatatype;
				while (i + cur < lex.lexTable.size && lex.lexTable.table[i + cur].lexema[0] != LEX_RESPONSE) {
					cur++;
				}
				if (i + cur == lex.lexTable.size) {
					throw ERROR_THROW_IN(700, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
				}
				if (i + cur < lex.lexTable.size && (lex.lexTable.table[i + cur + 1].lexema[0] == LEX_ID || lex.lexTable.table[i + cur + 1].lexema[0] == LEX_LITERAL)
					&& lex.idTable.table[lex.lexTable.table[i + cur + 1].idxTI].idtype != IT::F
					&& lex.idTable.table[lex.lexTable.table[i + cur + 1].idxTI].iddatatype != returnType) {
					
					// Разрешаем возвращать INT в функции типа BYTE
					IT::IDDATATYPE retValType = lex.idTable.table[lex.lexTable.table[i + cur + 1].idxTI].iddatatype;
					if (!(returnType == IT::BYTE && retValType == IT::INT)) {
						throw ERROR_THROW_IN(700, lex.lexTable.table[i + cur].sn, lex.lexTable.table[i + cur].idxTI);
					}
				}
			}
		}

		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_ID && lex.idTable.table[lex.lexTable.table[i].idxTI].idtype == IT::F && lex.lexTable.table[i - 1].lexema[0] == LEX_TASK) {
				IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
				int idsSize = 0;
				int funcPos = lex.idTable.table[lex.lexTable.table[i].idxTI].idxfirstLE;
				while (lex.lexTable.table[funcPos + 1].lexema[0] != LEX_RIGHTTHESIS)
				{
					if (lex.lexTable.table[funcPos + 1].lexema[0] == LEX_ID || lex.lexTable.table[funcPos + 1].lexema[0] == LEX_LITERAL) {
						ids[idsSize] = lex.idTable.table[lex.lexTable.table[funcPos + 1].idxTI].iddatatype;
						idsSize++;
					}
					funcPos++;
					if (idsSize >2) {
						throw ERROR_THROW_IN(705, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
					}
				}
			}

			int idxTI = lex.lexTable.table[i].idxTI;
			if (idxTI != TI_NULLIDX && idxTI >= 0 && idxTI < lex.idTable.size) {

				char* tokenName = lex.idTable.table[idxTI].id;
				bool isCompareStr = (strstr(tokenName, "strcpr") == tokenName) || (strstr(tokenName, "strcp") == tokenName);
				bool isStrToInt = (strstr(tokenName, "atoli") == tokenName) || (strstr(tokenName, "ato") == tokenName);

				if (isCompareStr || isStrToInt) {
					if (i + 1 < lex.lexTable.size && lex.lexTable.table[i + 1].lexema[0] == LEX_LEFTTHESIS) {
						IT::IDDATATYPE paramTypes[16];
						int paramCount = 0;
						int k = i + 2;
						while (k < lex.lexTable.size && lex.lexTable.table[k].lexema[0] != LEX_RIGHTTHESIS) {
							char lexType = lex.lexTable.table[k].lexema[0];
							if (lexType == LEX_ID || lexType == LEX_LITERAL) {
								if (paramCount < 16) {
									int pIdx = lex.lexTable.table[k].idxTI;
									if (lexType == LEX_LITERAL) {
										char firstChar = lex.idTable.table[pIdx].id[0];
										if (firstChar == '\"') paramTypes[paramCount] = IT::STR;
										else if (firstChar == '\'') paramTypes[paramCount] = IT::SYMB;
										else paramTypes[paramCount] = IT::INT;
									}
									else {
										paramTypes[paramCount] = lex.idTable.table[pIdx].iddatatype;
									}
									paramCount++;
								}
							}
							k++;
						}

						if (isCompareStr) {
							if (paramCount != 2) throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
							if (paramTypes[0] != IT::STR || paramTypes[1] != IT::STR) throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
						}
						if (isStrToInt) {
							if (paramCount != 1) throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
							if (paramTypes[0] != IT::STR) throw ERROR_THROW_IN(708, lex.lexTable.table[i].sn, lex.lexTable.table[i].idxTI);
						}
					}
				}
			}
		}
	}

	void literals(LA::LEX lex) {
		for (int i = 0; i < lex.idTable.size; i++) {

			if (lex.idTable.table[i].idtype == IT::L) {
				if (i > 0) {
					IT::IDDATATYPE currentDataType = lex.idTable.table[i].iddatatype; 
					IT::IDDATATYPE prevDataType = lex.idTable.table[i - 1].iddatatype;
					
					if (lex.idTable.table[i - 1].idtype == IT::V) {
						bool isCompatible = (currentDataType == prevDataType);
						if (!isCompatible && prevDataType == IT::BYTE && currentDataType == IT::INT) {
							int val = lex.idTable.table[i].value.vint;
							if (val < -128 || val > 127) {
								throw ERROR_THROW_IN(127, lex.idTable.table[i].line, lex.idTable.table[i].idxfirstLE);
							}
							isCompatible = true;
						}
						if (prevDataType != IT::TORF && !isCompatible) {
							throw ERROR_THROW_IN(703, lex.idTable.table[i].line, lex.idTable.table[i].idxfirstLE);
						}
					}
					else if (lex.idTable.table[i - 1].idtype == IT::L) {
						bool isCompatible = (currentDataType == prevDataType) ||
							(currentDataType == IT::INT && prevDataType == IT::BYTE) ||
							(currentDataType == IT::BYTE && prevDataType == IT::INT);
						
						if (!isCompatible) {
						}
					}
				}
			}
		}
	}
	void conditions(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_WHEN) {

				if (i + 2 >= lex.lexTable.size) return;

				if (lex.lexTable.table[i + 2].lexema[0] == LEX_RIGHTTHESIS) {
					throw ERROR_THROW_IN(713, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}

				bool hasComparison = false;
				bool isBoolean = false;
				int k = 2;

				while ((i + k) < lex.lexTable.size) {
					char token = lex.lexTable.table[i + k].lexema[0];

					if (token == LEX_RIGHTTHESIS) break;
					if (token == LEX_LEFTBRACE || token == LEX_SEMICOLON) break; 

					if (token == LEX_EQUAL) {
						throw ERROR_THROW_IN(606, lex.lexTable.table[i + k].sn, lex.lexTable.table[i + k].col);
					}

					if (token == LEX_COMPARISON) {
						hasComparison = true;
						char op = lex.lexTable.table[i + k].lexema[1];

						if (op != '<' && op != '>') {
							throw ERROR_THROW_IN(606, lex.lexTable.table[i + k].sn, lex.lexTable.table[i + k].col);
						}
					}

					if (token == LEX_ID || token == LEX_LITERAL) {
						int idx = lex.lexTable.table[i + k].idxTI;
						if (idx != -1) {
							if (lex.idTable.table[idx].iddatatype == IT::TORF) {
								isBoolean = true;
							}
						}
					}
					k++;
				}

				if (!hasComparison && !isBoolean) {
					throw ERROR_THROW_IN(717, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
		}
	}
	void switches(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_SWITCH) {

				if (i + 3 >= lex.lexTable.size) {
					continue; 
				}

				int argPos = i + 2;

				char argType = lex.lexTable.table[argPos].lexema[0];
				if (argType != LEX_ID && argType != LEX_LITERAL) {
					throw ERROR_THROW_IN(712, lex.lexTable.table[argPos].sn, lex.lexTable.table[argPos].col);
				}

				int idxTI = lex.lexTable.table[argPos].idxTI;

				if (idxTI >= 0 && idxTI < lex.idTable.size) {
					IT::Entry& entry = lex.idTable.table[idxTI];

					if (entry.iddatatype == IT::STR ||
						entry.iddatatype == IT::SYMB ||
						entry.iddatatype == IT::TORF) {
						throw ERROR_THROW_IN(712, lex.lexTable.table[argPos].sn, lex.lexTable.table[argPos].col);
					}

					if (argType == LEX_LITERAL) {
						if (entry.id[0] == '"' || entry.id[0] == '\'' ||
							strcmp(entry.id, "true") == 0 || strcmp(entry.id, "false") == 0) {
							throw ERROR_THROW_IN(712, lex.lexTable.table[argPos].sn, lex.lexTable.table[argPos].col);
						}

						if (entry.iddatatype == IT::INT) {
							if (entry.value.vint < -127 || entry.value.vint > 128) {
								throw ERROR_THROW_IN(127, lex.lexTable.table[argPos].sn, lex.lexTable.table[argPos].col);
							}
						}
					}
				}

				if (i + 4 >= lex.lexTable.size) continue;

				if (lex.lexTable.table[i + 4].lexema[0] != LEX_LEFTBRACE) {
					continue; 
				}

				int j = i + 5; 
				int braceBalance = 1;
				bool hasCase = false;
				bool hasDefault = false;

				std::vector<int> usedValues; 

				while (j < lex.lexTable.size && braceBalance > 0) {
					char token = lex.lexTable.table[j].lexema[0];

					if (token == LEX_LEFTBRACE) braceBalance++;
					else if (token == LEX_RIGHTBRACE) braceBalance--;

					else if (braceBalance == 1 && token == LEX_CASE) {
						hasCase = true;

						if (j + 1 >= lex.lexTable.size) {
							throw ERROR_THROW_IN(710, lex.lexTable.table[j].sn, lex.lexTable.table[j].col);
						}

						if (lex.lexTable.table[j + 1].lexema[0] != LEX_LITERAL) {
							throw ERROR_THROW_IN(710, lex.lexTable.table[j + 1].sn, lex.lexTable.table[j + 1].col);
						}

						int cIdx = lex.lexTable.table[j + 1].idxTI;
						if (cIdx >= 0 && cIdx < lex.idTable.size) {
							IT::Entry& cEntry = lex.idTable.table[cIdx];

							// Проверка типов в is
							if (cEntry.iddatatype == IT::STR || cEntry.iddatatype == IT::SYMB || cEntry.iddatatype == IT::TORF) {
								throw ERROR_THROW_IN(710, lex.lexTable.table[j + 1].sn, lex.lexTable.table[j + 1].col);
							}

							if (cEntry.iddatatype == IT::INT) {
								if (cEntry.value.vint < 0 || cEntry.value.vint > 255) {
									throw ERROR_THROW_IN(127, lex.lexTable.table[j + 1].sn, lex.lexTable.table[j + 1].col);
								}
							}

							// Проверка дубликатов
							int val = cEntry.value.vint;
							for (int uv : usedValues) {
								if (uv == val) throw ERROR_THROW_IN(711, lex.lexTable.table[j + 1].sn, lex.lexTable.table[j + 1].col);
							}
							usedValues.push_back(val);
						}
					}
					else if (braceBalance == 1 && token == LEX_DEFAULT) {
						if (hasDefault) {
							throw ERROR_THROW_IN(720, lex.lexTable.table[j].sn, lex.lexTable.table[j].col);
						}
						hasDefault = true;
					}

					j++;
				}

				if (braceBalance == 0) {
					if (!hasCase) throw ERROR_THROW_IN(719, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
					if (!hasDefault) throw ERROR_THROW_IN(718, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);

					i = j - 1;
				}
			}
		}
	}
	void checkSwitchSyntax(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_SWITCH) {

				int j = i + 1;
				while (j < lex.lexTable.size) {
					if (lex.lexTable.table[j].lexema[0] == LEX_LEFTBRACE) break;
					if (lex.lexTable.table[j].lexema[0] == LEX_SEMICOLON) break; 
					j++;
				}

				if (j >= lex.lexTable.size || lex.lexTable.table[j].lexema[0] != LEX_LEFTBRACE) continue;

				int braceBalance = 1;
				j++; 

				while (j < lex.lexTable.size && braceBalance > 0) {
					char token = lex.lexTable.table[j].lexema[0];

					if (token == LEX_LEFTBRACE) braceBalance++;
					else if (token == LEX_RIGHTBRACE) braceBalance--;

					if (braceBalance == 1 && token == LEX_LITERAL) {
						if (j + 1 < lex.lexTable.size && lex.lexTable.table[j + 1].lexema[0] == LEX_COLON) {
							if (j > 0 && lex.lexTable.table[j - 1].lexema[0] != LEX_CASE) {
								throw ERROR_THROW_IN(601, lex.lexTable.table[j].sn, lex.lexTable.table[j].col);
							}
						}
					}
					if (braceBalance == 1 && token == LEX_COLON) {
						if (j > 0) {
							char prev = lex.lexTable.table[j - 1].lexema[0];
							if (prev != LEX_LITERAL && prev != LEX_DEFAULT) {
								throw ERROR_THROW_IN(601, lex.lexTable.table[j].sn, lex.lexTable.table[j].col);
							}
						}
					}

					j++;
				}
			}
		}
	}

	void checkSemicolons(LA::LEX lex) {
		for (int i = 0; i < lex.lexTable.size; i++) {
			char currentLex = lex.lexTable.table[i].lexema[0];

			if (currentLex == LEX_OPERATION || currentLex == LEX_COMPARISON) {
				if (i + 1 < lex.lexTable.size) {
					char nextLex = lex.lexTable.table[i + 1].lexema[0];

					if (nextLex == LEX_SEMICOLON ||      // a + ;
						nextLex == LEX_RIGHTTHESIS ||    // ( a + )
						nextLex == LEX_RIGHTBRACE ||     // { a + }
						nextLex == LEX_LEFTBRACE ||      // a + {
						nextLex == LEX_COMMA ||          // func(a+, b)
						nextLex == LEX_EQUAL ||          // a + =
						nextLex == LEX_DECLARE ||        // a + declare
						nextLex == LEX_COUT ||
						nextLex == LEX_RESPONSE ||
						nextLex == LEX_SWITCH ||
						nextLex == LEX_WHEN ||
						nextLex == LEX_CASE ||
						nextLex == LEX_DEFAULT ||
						nextLex == LEX_OTHER)            
					{
						// (нет второго операнда)
						throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
					}
				}
				else {
					throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (lex.lexTable.table[i].lexema[0] == LEX_EQUAL) {

				if (i + 1 < lex.lexTable.size) {
					if (lex.lexTable.table[i + 1].lexema[0] == LEX_EQUAL) {
						continue; 
					}
				}

				if (i > 0) {
					char prevLex = lex.lexTable.table[i - 1].lexema[0];
					if (prevLex == LEX_COMPARISON || prevLex == LEX_OPERATION) {
						continue;
					}
				}
				if (i + 1 < lex.lexTable.size) {
					char nextLex = lex.lexTable.table[i + 1].lexema[0];
					if (nextLex == LEX_SEMICOLON) {
						throw ERROR_THROW_IN(602, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
					}
					if (nextLex == LEX_OPERATION) {
						char op = lex.lexTable.table[i + 1].lexema[1];
						if (op == '+' || op == '*' || op == '/' || op == '%') {
							throw ERROR_THROW_IN(602, lex.lexTable.table[i + 1].sn, lex.lexTable.table[i + 1].col);
						}
					}
				}

				int k = 1;
				bool semicolonFound = false;

				while ((i + k) < lex.lexTable.size) {
					char token = lex.lexTable.table[i + k].lexema[0];

					if (token == LEX_SEMICOLON) {
						semicolonFound = true;
						break;
					}
					if (token == LEX_DECLARE || token == LEX_COUT ||
						token == LEX_RESPONSE || token == LEX_RIGHTBRACE ||
						token == LEX_LEFTBRACE || token == LEX_SWITCH ||
						token == LEX_WHEN || token == LEX_CASE || token == LEX_DEFAULT) {
						break;
					}
					k++;
				}

				if (!semicolonFound) {
					throw ERROR_THROW_IN(608, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (lex.lexTable.table[i].lexema[0] == LEX_RESPONSE ||
				lex.lexTable.table[i].lexema[0] == LEX_COUT) {

				int k = 1;
				bool semicolonFound = false;
				while ((i + k) < lex.lexTable.size) {
					char token = lex.lexTable.table[i + k].lexema[0];
					if (token == LEX_SEMICOLON) {
						semicolonFound = true;
						break;
					}
					if (token == LEX_DECLARE || token == LEX_COUT || token == LEX_RESPONSE ||
						token == LEX_RIGHTBRACE || token == LEX_LEFTBRACE || token == LEX_CASE) {
						break;
					}
					k++;
				}
				if (!semicolonFound) {
					throw ERROR_THROW_IN(608, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
		}
	}
	void checkBraces(LA::LEX lex) {
		int braceBalance = 0;
		bool beginFound = false;

		for (int i = 0; i < lex.lexTable.size; i++) {
			char token = lex.lexTable.table[i].lexema[0];

			if (token == LEX_LEFTBRACE) {
				braceBalance++;
			}
			else if (token == LEX_RIGHTBRACE) {
				braceBalance--;
				if (braceBalance < 0) {
					throw ERROR_THROW_IN(611, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
			if (token == LEX_BEGIN) {
				beginFound = true;

				if (i + 1 >= lex.lexTable.size || lex.lexTable.table[i + 1].lexema[0] != LEX_LEFTBRACE) {
					throw ERROR_THROW_IN(610, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (token == LEX_COUT) {
				if (i + 1 >= lex.lexTable.size) throw ERROR_THROW_IN(607, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				if (lex.lexTable.table[i + 1].lexema[0] != LEX_LEFTTHESIS) {
					throw ERROR_THROW_IN(613, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
				bool closingFound = false;
				int k = 1;
				while (i + k < lex.lexTable.size) {
					char nextTok = lex.lexTable.table[i + k].lexema[0];
					if (nextTok == LEX_SEMICOLON) {
						if (lex.lexTable.table[i + k - 1].lexema[0] == LEX_RIGHTTHESIS) closingFound = true;
						break;
					}
					if (nextTok == LEX_LEFTBRACE || nextTok == LEX_RIGHTBRACE || nextTok == LEX_DECLARE) break;
					k++;
				}
				if (!closingFound) {
					throw ERROR_THROW_IN(613, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (token == LEX_CASE) {
				if (i + 3 >= lex.lexTable.size) throw ERROR_THROW_IN(600, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);

				if (lex.lexTable.table[i + 1].lexema[0] != LEX_LITERAL) throw ERROR_THROW_IN(710, lex.lexTable.table[i + 1].sn, lex.lexTable.table[i + 1].col);
				if (lex.lexTable.table[i + 2].lexema[0] != LEX_COLON) throw ERROR_THROW_IN(722, lex.lexTable.table[i + 2].sn, lex.lexTable.table[i + 2].col);

				if (lex.lexTable.table[i + 3].lexema[0] != LEX_LEFTBRACE) {
					throw ERROR_THROW_IN(610, lex.lexTable.table[i + 3].sn, lex.lexTable.table[i + 3].col);
				}

				if (i + 4 < lex.lexTable.size && lex.lexTable.table[i + 4].lexema[0] == LEX_RIGHTBRACE) {
					throw ERROR_THROW_IN(723, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (token == LEX_DEFAULT) {
				// any : {
				if (i + 2 >= lex.lexTable.size) throw ERROR_THROW_IN(600, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);

				if (lex.lexTable.table[i + 1].lexema[0] != LEX_COLON) throw ERROR_THROW_IN(722, lex.lexTable.table[i + 1].sn, lex.lexTable.table[i + 1].col);

				if (lex.lexTable.table[i + 2].lexema[0] != LEX_LEFTBRACE) {
					throw ERROR_THROW_IN(610, lex.lexTable.table[i + 2].sn, lex.lexTable.table[i + 2].col);
				}

				if (i + 3 < lex.lexTable.size && lex.lexTable.table[i + 3].lexema[0] == LEX_RIGHTBRACE) {
					throw ERROR_THROW_IN(724, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}

			if (token == LEX_BEGIN) {
				if (i + 1 >= lex.lexTable.size || lex.lexTable.table[i + 1].lexema[0] != LEX_LEFTBRACE) {
					throw ERROR_THROW_IN(610, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
			if (token == LEX_OTHER) {
				if (i + 1 >= lex.lexTable.size) throw ERROR_THROW_IN(600, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);

				if (lex.lexTable.table[i + 1].lexema[0] != LEX_LEFTBRACE) {
					throw ERROR_THROW_IN(610, lex.lexTable.table[i + 1].sn, lex.lexTable.table[i + 1].col);
				}

				if (i + 2 < lex.lexTable.size && lex.lexTable.table[i + 2].lexema[0] == LEX_RIGHTBRACE) {
					throw ERROR_THROW_IN(725, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}


			if (token == LEX_SWITCH || token == LEX_WHEN) {
				int k = 1;
				if (i + k < lex.lexTable.size && lex.lexTable.table[i + k].lexema[0] == LEX_LEFTTHESIS) {
					int pBalance = 1;
					k++;
					while ((i + k) < lex.lexTable.size && pBalance > 0) {
						if (lex.lexTable.table[i + k].lexema[0] == LEX_LEFTTHESIS) pBalance++;
						else if (lex.lexTable.table[i + k].lexema[0] == LEX_RIGHTTHESIS) pBalance--;
						k++;
					}

					if ((i + k) >= lex.lexTable.size || lex.lexTable.table[i + k].lexema[0] != LEX_LEFTBRACE) {
						throw ERROR_THROW_IN(610, lex.lexTable.table[i + k - 1].sn, lex.lexTable.table[i + k - 1].col);
					}

					if ((i + k + 1) < lex.lexTable.size && lex.lexTable.table[i + k + 1].lexema[0] == LEX_RIGHTBRACE) {
						if (token == LEX_SWITCH) {
							throw ERROR_THROW_IN(721, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
						}
						else {
							throw ERROR_THROW_IN(725, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
						}
					}
				}
				else {
					// Нет открывающей (
					throw ERROR_THROW_IN(613, lex.lexTable.table[i].sn, lex.lexTable.table[i].col);
				}
			}
		}

		if (braceBalance != 0) {
			throw ERROR_THROW_IN(612, lex.lexTable.table[lex.lexTable.size - 1].sn, lex.lexTable.table[lex.lexTable.size - 1].col);
		}
		if (!beginFound) {
			throw ERROR_THROW(614); // точка входа не найдена
		}
	}
	bool startSA(LA::LEX lex) {
		functions(lex);
		literals(lex);
		operands(lex);
		conditions(lex);
		switches(lex);
		return true;
	};
}