#include "CodeGeneration.h"

namespace CodeGeneration {

	static int ifs = 1;

	void Head(Out::OUT out) {
		*out.stream << ".586P\n";
		*out.stream << ".model flat, stdcall\n";
		*out.stream << "includelib libucrt.lib\n";
		*out.stream << "includelib kernel32.lib\n";
		*out.stream << "includelib \"D:\\bstu\\3sem\\lpa2\\lpaa\\lpa\\LPA-2025\\Debug\\LIB.lib\"\n";
		*out.stream << "ExitProcess PROTO : DWORD\n\n";
		*out.stream << "SetConsoleCP PROTO : DWORD\n\n";
		*out.stream << "SetConsoleOutputCP PROTO : DWORD\n\n";
		*out.stream << "strcpr PROTO : DWORD, : DWORD \n\n";
		*out.stream << "atoli PROTO : DWORD \n\n";
		*out.stream << "writestr PROTO : DWORD \n\n";
		*out.stream << "writeint PROTO : SDWORD \n\n";
		*out.stream << "writebool PROTO : BYTE \n\n";
		*out.stream << "writechar PROTO : BYTE \n\n";
		*out.stream << ".stack 4096\n\n";
	}

	void Const(Out::OUT out, LA::LEX lex) {
		*out.stream << ".const\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::L) {
				*out.stream << lex.idTable.table[i].id;
				switch (lex.idTable.table[i].iddatatype)
				{
				case IT::SYMB: {
					if (lex.idTable.table[i].value.vsymb == 0) {
						*out.stream << " BYTE 0";
					}
					else {
						*out.stream << " BYTE " << (int)lex.idTable.table[i].value.vsymb;
					}
					*out.stream << " ; symb";
					break;
				}
				case IT::STR: {
					bool isEmpty = lex.idTable.table[i].value.vstr->len == 0;

					if (!isEmpty && lex.idTable.table[i].value.vstr->str[0] == '\"' && lex.idTable.table[i].value.vstr->str[1] == '\"' && lex.idTable.table[i].value.vstr->str[2] == '\0') {
						isEmpty = true;
					}

					if (isEmpty) {
						*out.stream << " DB 0"; 
					}
					else {
						*out.stream << " DB " << lex.idTable.table[i].value.vstr->str << ", 0";
					}
					*out.stream << " ; str";
					break;
				}
				case IT::TORF: {
					*out.stream << " BYTE " << lex.idTable.table[i].value.vtorf << " ; torf";
					break;
				}
				case IT::BYTE:
				case IT::INT: {
					*out.stream << " SDWORD " << lex.idTable.table[i].value.vint << " ; int (4 bytes)";
					break;
				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	void Data(Out::OUT out, LA::LEX lex) {
		*out.stream << ".data\n";
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype == IT::V) {
				*out.stream << lex.idTable.table[i].id;
				switch (lex.idTable.table[i].iddatatype) {
				case IT::BYTE:
				case IT::INT: {
					*out.stream << " SDWORD 0 ; int";
					break;
				}
				case IT::STR: {
					*out.stream << " DWORD 0 ; str";
					break;
				}
				case IT::SYMB: {
					*out.stream << " BYTE 0 ; symb";
					break;
				}
				case IT::TORF: {
					*out.stream << " BYTE 0 ; boolean";
					break;
				}
				}
				*out.stream << "\n";
			}
		}
		*out.stream << "\n";
	}

	void Expression(Out::OUT out, LA::LEX lex, int startPos, int endPos) {
		for (int i = startPos; i < endPos; i++) {
			switch (lex.lexTable.table[i].lexema[0]) {
			case LEX_OPERATION: {

				char operation = lex.lexTable.table[i].lexema[1];

				IT::Entry* leftOperand = nullptr;
				IT::Entry* rightOperand = nullptr;
				IT::Entry* result = nullptr;
				int iterator = 1;
				while (!leftOperand || !rightOperand || !result) {
					if (i - iterator < startPos) { 
						break;
					}

					const auto& currentLex = lex.lexTable.table[i - iterator];

					if (currentLex.lexema[0] == '#' || currentLex.lexema[0] == ';' || currentLex.lexema[0] == LEX_OPERATION) {
						iterator++;
						continue;
					}

					if (currentLex.idxTI == -1) {
						iterator++;
						continue;
					}

					if (!leftOperand) {
						leftOperand = &lex.idTable.table[currentLex.idxTI];
					}
					else if (!rightOperand) {
						rightOperand = &lex.idTable.table[currentLex.idxTI];
					}
					else if (!result) {
						result = &lex.idTable.table[currentLex.idxTI];
					}
					iterator++;
				}
				if (operation == MINUS && leftOperand && !rightOperand && result) {
					IT::Entry* number = leftOperand;
					IT::Entry* target = result;

					*out.stream << "; Unary Minus (-" << number->value.vint << ")\n";

					if (target->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << number->id << "\n";
						*out.stream << "neg al\n";
						*out.stream << "mov " << target->id << ", al\n";
					}
					else {
						*out.stream << "mov eax, " << number->id << "\n";
						*out.stream << "neg eax\n";
						*out.stream << "mov " << target->id << ", eax\n";
					}
					break; 
				}

				if (!leftOperand || !rightOperand || !result) {
					throw ERROR_THROW(113);
				}

				switch (operation) {
				case PLUS: {
					*out.stream << "; Addition\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";
						*out.stream << "add al, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", al\n";
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "add eax, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", eax\n";
					}

					break;
				}
				case MINUS: {
					*out.stream << "; Subtraction\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";
						*out.stream << "sub al, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", al\n";
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "sub eax, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", eax\n";
					}

					break;
				}
				case STAR: {
					*out.stream << "; Multiplication\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						
						*out.stream << "movsx eax, " << leftOperand->id << "\n";
						*out.stream << "movsx ebx, " << rightOperand->id << "\n";
						*out.stream << "imul eax, ebx\n";
						*out.stream << "mov " << result->id << ", eax\n"; 
					
						// *out.stream << "mov " << result->id << ", al\n";
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "imul eax, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", eax\n";
					}
					break;
				}

				case AMPERSAND: {
					*out.stream << "; Logical AND\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";
						*out.stream << "and al, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", al\n";
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "and eax, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", eax\n";
					}
					break;
				}
				case PIPE: {
					*out.stream << "; Logical OR\n";

					if (leftOperand->iddatatype == IT::BYTE && rightOperand->iddatatype == IT::BYTE) {
						*out.stream << "mov al, " << leftOperand->id << "\n";
						*out.stream << "or al, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", al\n";
					}
					else if (leftOperand->iddatatype == IT::INT && rightOperand->iddatatype == IT::INT) {
						*out.stream << "mov eax, " << leftOperand->id << "\n";
						*out.stream << "or eax, " << rightOperand->id << "\n";
						*out.stream << "mov " << result->id << ", eax\n";
					}
					break;
				}
				default:
					*out.stream << "; Unsupported operation\n";
					break;
				}
				break;
			}
			case TILDE: {
				*out.stream << "; Logical NOT\n";
				IT::Entry* operand = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				IT::Entry* result = &lex.idTable.table[lex.lexTable.table[i - 3].idxTI];

				if (operand->iddatatype == IT::BYTE) {
					*out.stream << "mov al, " << operand->id << "\n";
					*out.stream << "not al\n";
					*out.stream << "mov " << result->id << ", al\n";
				}
				else if (operand->iddatatype == IT::INT) {
					*out.stream << "mov eax, " << operand->id << "\n";
					*out.stream << "not eax\n";
					*out.stream << "mov " << result->id << ", eax\n";
				}
				break;
			}

			case INC: { // Инкремент ++
				*out.stream << "; Increment\n";

				IT::Entry* operand = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];

				if (operand->idtype == IT::L) {
				}

				if (operand->iddatatype == IT::BYTE) {
					*out.stream << "mov al, " << operand->id << "\n";
					*out.stream << "inc al\n";
					*out.stream << "mov " << operand->id << ", al\n";
				}
				else if (operand->iddatatype == IT::INT) {
					*out.stream << "mov eax, " << operand->id << "\n";
					*out.stream << "inc eax\n";
					*out.stream << "mov " << operand->id << ", eax\n";
				}
				break;
			}

			case DEC: { // Декремент --
				*out.stream << "; Decrement\n";
				IT::Entry* operand = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];

				if (operand->iddatatype == IT::BYTE) {
					*out.stream << "mov al, " << operand->id << "\n";
					*out.stream << "dec al\n";
					*out.stream << "mov " << operand->id << ", al\n";
				}
				else if (operand->iddatatype == IT::INT) {
					*out.stream << "mov eax, " << operand->id << "\n";
					*out.stream << "dec eax\n";
					*out.stream << "mov " << operand->id << ", eax\n";
				}
				break;
			}

			case LEX_EQUAL: {
				*out.stream << "\n" << "; string #" << lex.lexTable.table[i].sn << " : ";
				int parmsAmount = 0;

				int statrt = i + 1;

				while (i < lex.lexTable.size && lex.lexTable.table[i].lexema[0] != LEX_SEMICOLON) i++;

				*out.stream << ";metka\n";

				if (statrt == i - 1)
				{

					if (lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype == IT::INT ||
						lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype == IT::BYTE)
					{
						*out.stream << "mov eax, " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";

					}
					else if (lex.idTable.table[lex.lexTable.table[statrt].idxTI].iddatatype == IT::STR)
					{
						if (lex.idTable.table[lex.lexTable.table[statrt].idxTI].idtype == IT::L) {
							*out.stream << "mov eax, OFFSET " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
						}
						else {
							*out.stream << "mov eax, " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
						}
					}
					else {
						*out.stream << "movsx eax, " << lex.idTable.table[lex.lexTable.table[statrt].idxTI].id << "\n";
					}

					if (lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::INT ||
						lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::BYTE)
					{
						*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << ", eax\n";

					}

					else if (lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].iddatatype == IT::STR) {
						*out.stream << "mov [" << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << "], eax\n";
					}

					else {
						*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 1].idxTI].id << ", al\n";
					}
					break;
				}

				ExpressionHandler(out.stream, lex, statrt, i - 1);
				*out.stream << "pop eax\n";
				*out.stream << "mov " << lex.idTable.table[lex.lexTable.table[statrt - 2].idxTI].id << ", eax\n";
				

				break;
			}

			case LEX_WHEN: {
				int cur = 1;
				int startPos;
				bool isEnd = false;
				bool hasElse = false;
				IT::Entry* first = nullptr, * second = nullptr;
				std::string op = "";

				int currentIf = ifs;
				ifs += 2;


				*out.stream << "\n; --- IF BEGIN " << currentIf << " ---\n";

				while (true) {
					switch (lex.lexTable.table[i + cur].lexema[0]) {
					case LEX_RIGHTTHESIS: {
						startPos = i + 1;
						break;
					}
					case LEX_ID:
					case LEX_LITERAL: {
						if (first == nullptr) first = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						else second = &lex.idTable.table[lex.lexTable.table[i + cur].idxTI];
						break;
					}
					case LEX_COMPARISON: {
						char data = lex.lexTable.table[i + cur].lexema[1];
						if (data == '<') op = "jl";
						else if (data == '>') op = "jg";
						else if (data == 'e') op = "je"; 
						else if (data == 'n') op = "jne"; 
						break;
					}
					case LEX_LEFTBRACE: {
						// Генерация сравнения
						if (second == nullptr) {
							second = new IT::Entry; 
							if (first->iddatatype == IT::SYMB || first->iddatatype == IT::TORF)
								*out.stream << "movzx eax, " << first->id << "\n";
							else
								*out.stream << "mov eax, " << first->id << "\n";

							*out.stream << "cmp eax, 1\n";
							*out.stream << "je If_True_" << currentIf << "\n";
						}
						else {
							// Обычное сравнение двух операндов
							if (first->iddatatype == IT::SYMB || first->iddatatype == IT::TORF)
								*out.stream << "movzx eax, " << first->id << "\n";
							else
								*out.stream << "mov eax, " << first->id << "\n";

							if (second->iddatatype == IT::SYMB || second->iddatatype == IT::TORF)
								*out.stream << "movzx ebx, " << second->id << "\n";
							else
								*out.stream << "mov ebx, " << second->id << "\n";

							*out.stream << "cmp eax, ebx\n";
							*out.stream << op.c_str() << " If_True_" << currentIf << "\n";
						}

						if (lex.lexTable.table[i + cur + 1].lexema[0] == LEX_OTHER || false) {
							*out.stream << "jmp If_Else_" << currentIf << "\n";
						}
						else {
							*out.stream << "jmp If_Else_" << currentIf << "\n";
						}

						break;
					}
					case LEX_RIGHTBRACE: {
						isEnd = true;
						if (i + cur + 1 < lex.lexTable.size && lex.lexTable.table[i + cur + 1].lexema[0] == LEX_OTHER) {
							hasElse = true;
						}
						break;
					}
					}

					if (isEnd) {
						*out.stream << "If_True_" << currentIf << ":\n";
						Expression(out, lex, startPos, i + cur); 

						*out.stream << "jmp If_Exit_" << currentIf << "\n";

						*out.stream << "If_Else_" << currentIf << ":\n";

						if (hasElse) {
							int elseStartIdx = i + cur + 1; 
							int bracesCounter = 0;
							int k = elseStartIdx + 1; 

							while (k < lex.lexTable.size && lex.lexTable.table[k].lexema[0] != LEX_LEFTBRACE) k++;
							int bodyStart = k + 1;

							bracesCounter = 1;
							k++;
							while (k < lex.lexTable.size && bracesCounter > 0) {
								if (lex.lexTable.table[k].lexema[0] == LEX_LEFTBRACE) bracesCounter++;
								if (lex.lexTable.table[k].lexema[0] == LEX_RIGHTBRACE) bracesCounter--;
								k++;
							}
							int bodyEnd = k - 1;

							Expression(out, lex, bodyStart, bodyEnd);

							cur = (k - 1) - i;
						}

						*out.stream << "If_Exit_" << currentIf << ":\n";
						*out.stream << "; --- IF END " << currentIf << " ---\n";

						i += cur;
						break;
					}
					cur++;
				}
				break;
			}

			case LEX_OTHER: {
				break;
			}

			case LEX_RESPONSE: {
				*out.stream << "\n; response\n";
				IT::Entry* returnValue = &lex.idTable.table[lex.lexTable.table[i + 1].idxTI];

				if (returnValue->iddatatype == IT::BYTE) {

					if (returnValue->idtype == IT::L) {
						*out.stream << "movsx eax, " << returnValue->id << "\n";
					}
					else {
						*out.stream << "movsx eax, " << returnValue->id << "\n";
						*out.stream << "mov " << returnValue->id << ", al\n";
					}
				}
				if (returnValue->iddatatype == IT::INT) {

					*out.stream << "mov eax, " << returnValue->id << "\n";
				}
				else if (returnValue->iddatatype == IT::STR) {

					if (returnValue->idtype == IT::L) {
						*out.stream << "mov eax, OFFSET " << returnValue->id << "\n";
					}
					else {
						*out.stream << "mov eax, " << returnValue->id << "\n";
					}
				}
				else {

					*out.stream << "movzx eax, " << returnValue->id << "\n";
				}
				break;
			}
			case LEX_COUT: {
				switch (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].iddatatype)
				{
				case (IT::SYMB): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writechar" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				case (IT::STR): {
					*out.stream << "\npush ";
					if (lex.idTable.table[lex.lexTable.table[i + 2].idxTI].idtype == IT::L)
						*out.stream << "offset " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					else
						*out.stream << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';

					*out.stream << "CALL writestr" << '\n';
					break;
				}
				case IT::BYTE:
				case IT::INT: {
					*out.stream << "\nmov eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << "\n";
					*out.stream << "push eax\n";
					*out.stream << "CALL writeint" << '\n';
					//*out.stream << "pop eax\n";
					break;
				}
				case (IT::TORF): {
					*out.stream << "push eax\n";
					*out.stream << "movzx eax, " << lex.idTable.table[lex.lexTable.table[i + 2].idxTI].id << '\n';
					*out.stream << "push eax\n";
					*out.stream << "CALL writebool" << '\n';
					*out.stream << "pop eax\n" << '\n';
					break;
				}
				}
				break;
			}
			case LEX_ID: {
				if ((lex.lexTable.table[i - 1].lexema[0] == LEX_SEMICOLON || lex.lexTable.table[i - 1].lexema[0] == LEX_LEFTBRACE || lex.lexTable.table[i - 1].lexema[0] == LEX_RIGHTBRACE) &&
					lex.lexTable.table[i + 1].lexema[0] != LEX_EQUAL) {
					stack<IT::Entry> stackForParams;
					IT::Entry* function = &lex.idTable.table[lex.lexTable.table[i].idxTI];
					int iterator = 1;
					while (lex.lexTable.table[i + iterator].lexema[0] != '@') {
						stackForParams.push(lex.idTable.table[lex.lexTable.table[i + iterator].idxTI]);
						iterator += 1;
					}
					while (!stackForParams.empty()) {
						switch (stackForParams.top().iddatatype) {

						case IT::TORF:
						case IT::SYMB: {
							*out.stream << "movsx eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}
						case IT::STR: {
							if (stackForParams.top().idtype == IT::L) {
								*out.stream << "push OFFSET " << stackForParams.top().id << "\n";
							}
							else {
								*out.stream << "push " << stackForParams.top().id << "\n";
							}
							break;
						}
						case IT::BYTE:
						case IT::INT: {
							*out.stream << "mov eax, " << stackForParams.top().id << "\n";
							*out.stream << "push eax\n";
							break;
						}
						}
						stackForParams.pop();
					}
					*out.stream << "CALL F" << function->id << "\n";
				}
				break;
			}

			case LEX_SWITCH: {
				if (lex.lexTable.table[i + 2].idxTI == -1) {
					int k = i + 1;
					int braceBalance = 0;
					while (k < lex.lexTable.size) {
						if (lex.lexTable.table[k].lexema[0] == LEX_LEFTBRACE) braceBalance++;
						if (lex.lexTable.table[k].lexema[0] == LEX_RIGHTBRACE) {
							braceBalance--;
							if (braceBalance == 0) break;
						}
						k++;
					}
					i = k;
					break;
				}
				int currentSwitchId = ifs++; 
				IT::Entry* switchVar = &lex.idTable.table[lex.lexTable.table[i + 2].idxTI];

				*out.stream << "\n; SWITCH START ID: " << currentSwitchId << " ---\n";

				if (switchVar->iddatatype == IT::BYTE)
					*out.stream << "movzx eax, byte ptr [" << switchVar->id << "]\n";
				else
					*out.stream << "mov eax, " << switchVar->id << "\n";

				int k = i + 5;
				int caseCounter = 0;

				while (k < lex.lexTable.size && lex.lexTable.table[k].lexema[0] != LEX_RIGHTBRACE) {

					if (lex.lexTable.table[k].lexema[0] == LEX_CASE) {

						IT::Entry* caseLit = &lex.idTable.table[lex.lexTable.table[k + 1].idxTI];

						*out.stream << "; is Check " << caseLit->value.vint << "\n";
						*out.stream << "cmp eax, " << caseLit->value.vint << "\n";
						*out.stream << "jne Switch_" << currentSwitchId << "_Next_" << caseCounter << "\n";

						int bodyStart = k + 4; 
						int bodyEnd = bodyStart;
						int braceBalance = 1;

						while (braceBalance > 0 && bodyEnd < lex.lexTable.size) {
							bodyEnd++;
							if (lex.lexTable.table[bodyEnd].lexema[0] == LEX_LEFTBRACE) braceBalance++;
							if (lex.lexTable.table[bodyEnd].lexema[0] == LEX_RIGHTBRACE) braceBalance--;
						}

						Expression(out, lex, bodyStart, bodyEnd);

						*out.stream << "jmp Switch_" << currentSwitchId << "_EXIT\n";

						*out.stream << "Switch_" << currentSwitchId << "_Next_" << caseCounter << ":\n";

						caseCounter++;
						k = bodyEnd; 
					}

					else if (lex.lexTable.table[k].lexema[0] == LEX_DEFAULT) {
						*out.stream << "; any is\n";

						int bodyStart = k + 3; 
						int bodyEnd = bodyStart;
						int braceBalance = 1;

						while (braceBalance > 0 && bodyEnd < lex.lexTable.size) {
							bodyEnd++;
							if (lex.lexTable.table[bodyEnd].lexema[0] == LEX_LEFTBRACE) braceBalance++;
							if (lex.lexTable.table[bodyEnd].lexema[0] == LEX_RIGHTBRACE) braceBalance--;
						}

						Expression(out, lex, bodyStart, bodyEnd); 

						*out.stream << "jmp Switch_" << currentSwitchId << "_EXIT\n";

						k = bodyEnd;
					}

					k++;
				}

				*out.stream << "Switch_" << currentSwitchId << "_EXIT:\n";
				*out.stream << "; SWITCH END\n";

				i = k;
				break;
			}
			}

		}
	}

	bool IsIndentifier(const LA::LEX& lex, int i) {
		return lex.lexTable.table[i].lexema[0] == LEX_ID;
	}

	bool IsLexemaFunction(const LA::LEX& lex, int i) {
		return lex.lexTable.table[i].lexema[0] == '@';
	}

	void Operations(std::ofstream* stream, LT::Entry lexTable) {
		switch (lexTable.lexema[1]) {
		case PLUS:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "add eax, ebx\n";
			break;
		}
		case MINUS:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "sub eax, ebx\n";
			break;
		}
		case STAR:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "imul eax, ebx\n";
			break;
		}
		case DIRSLASH:
		{
			*stream << "pop ebx\n";      // Делитель в EBX
			*stream << "pop eax\n";      // Делимое в EAX
			*stream << "cdq\n";          // Расширение знака EAX до EDX:EAX 
			*stream << "idiv ebx\n";     // Деление (результат: частное в EAX, остаток в EDX)
			break;
		}
		case AMPERSAND:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "and eax, ebx\n";
			break;
		}
		case PIPE:
		{
			*stream << "pop ebx\n";
			*stream << "pop eax\n";
			*stream << "or eax, ebx\n";
			break;
		}

		}

		if (lexTable.lexema[0] == TILDE) {
			*stream << "pop eax\n";
			*stream << "not eax\n";
		}
	}

	void ExpressionHandler(std::ofstream* stream, LA::LEX lex, int startpos, int endpos) {
		for (int i = startpos; i <= endpos; i++) {

			if (lex.lexTable.table[i].lexema[0] == '#') {
				continue;
			}

			IT::Entry* entry = nullptr;
			if (lex.lexTable.table[i].idxTI != -1) {
				entry = &lex.idTable.table[lex.lexTable.table[i].idxTI];
			}

			if (entry != nullptr) {
				if (strcmp(entry->id, "atoli") == 0 || strcmp(entry->id, "ato") == 0) {
					*stream << "call atoli\n";
					*stream << "push eax\n"; 
					continue; 
				}
				if (strcmp(entry->id, "strcpr") == 0 || strcmp(entry->id, "strcp") == 0) {
					*stream << "call strcpr\n"; 
					*stream << "push eax\n";        
					continue; 
				}

			}

			bool isUserFunc = (entry && entry->idtype == IT::F);
			bool isCallToken = (lex.lexTable.table[i].lexema[0] == '@');

			if (isUserFunc || isCallToken) {
				if (!entry && lex.lexTable.table[i].idxTI != -1)
					entry = &lex.idTable.table[lex.lexTable.table[i].idxTI];

				if (entry && entry->idtype == IT::F) {
					*stream << "call F" << entry->id << "\n";
					*stream << "push eax\n";
				}
				continue;
			}

			if (lex.lexTable.table[i].lexema[0] == LEX_ID || lex.lexTable.table[i].lexema[0] == LEX_LITERAL) {
				if (entry) {
					if (entry->iddatatype == IT::STR && entry->idtype == IT::L) {
						*stream << "push OFFSET " << entry->id << "\n";
					}
					else {
						*stream << "push " << entry->id << "\n";
					}
				}
			}

			else if (lex.lexTable.table[i].lexema[0] == INC) {
				IT::Entry* var = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				*stream << "pop eax\n";
				*stream << "inc eax\n";
				if (var->idtype == IT::V || var->idtype == IT::P) {
					if (var->iddatatype == IT::BYTE) *stream << "mov byte ptr [" << var->id << "], al\n";
					else *stream << "mov " << var->id << ", eax\n";
				}
				*stream << "push eax\n";
			}
			else if (lex.lexTable.table[i].lexema[0] == DEC) {
				IT::Entry* var = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				*stream << "pop eax\n";
				*stream << "dec eax\n";
				if (var->idtype == IT::V || var->idtype == IT::P) {
					if (var->iddatatype == IT::BYTE) *stream << "mov byte ptr [" << var->id << "], al\n";
					else *stream << "mov " << var->id << ", eax\n";
				}
				*stream << "push eax\n";
			}
			else if (lex.lexTable.table[i].lexema[0] == POST_INC) {
				IT::Entry* var = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				*stream << "pop eax\n";
				*stream << "mov ebx, eax\n";
				*stream << "inc eax\n";
				if (var->idtype == IT::V || var->idtype == IT::P) {
					if (var->iddatatype == IT::BYTE) *stream << "mov byte ptr [" << var->id << "], al\n";
					else *stream << "mov " << var->id << ", eax\n";
				}
				*stream << "mov eax, ebx\n";
				*stream << "push eax\n";
			}
			else if (lex.lexTable.table[i].lexema[0] == POST_DEC) {
				IT::Entry* var = &lex.idTable.table[lex.lexTable.table[i - 1].idxTI];
				*stream << "pop eax\n";
				*stream << "mov ebx, eax\n";
				*stream << "dec eax\n";
				if (var->idtype == IT::V || var->idtype == IT::P) {
					if (var->iddatatype == IT::BYTE) *stream << "mov byte ptr [" << var->id << "], al\n";
					else *stream << "mov " << var->id << ", eax\n";
				}
				*stream << "mov eax, ebx\n";
				*stream << "push eax\n";
			}

			

			else {
				if (lex.lexTable.table[i].lexema[0] == LEX_OPERATION || lex.lexTable.table[i].lexema[0] == TILDE) {
					Operations(stream, lex.lexTable.table[i]);
					*stream << "push eax\n";
				}
			}
		}
	}

	void Functions(Out::OUT out, LA::LEX lex) {
		for (int i = 0; i < lex.idTable.size; i++) {
			if (lex.idTable.table[i].idtype != IT::F) continue;

			*out.stream << "\nF" << lex.idTable.table[i].id << " PROC uses ebx ecx edi esi";

			int cur = 1;
			int firstLexIdx = lex.idTable.table[i].idxfirstLE;

			while (lex.lexTable.table[firstLexIdx + cur].lexema[0] != LEX_RIGHTTHESIS) {
				auto& curLex = lex.lexTable.table[firstLexIdx + cur];

				if (curLex.lexema[0] == LEX_ID) {
					int idxTI = curLex.idxTI;
					auto& curId = lex.idTable.table[idxTI];

					if (curId.idtype == IT::P) {
						*out.stream << ", " << curId.id;
						switch (curId.iddatatype) {
						case IT::BYTE:
							*out.stream << " : SBYTE";
							break;
						case IT::STR:
							*out.stream << " : DWORD";
							break;
						case IT::SYMB:
						case IT::TORF:
							*out.stream << " : BYTE";
							break;
						case IT::INT:
							*out.stream << " : DWORD";
							break;
						}
					}
				}
				cur++;
			}

			int startPos = firstLexIdx + cur;
			while (lex.lexTable.table[firstLexIdx + cur].lexema[0] != LEX_RESPONSE) {
				cur++;
			}
			int endPos = firstLexIdx + cur + 4;

			Expression(out, lex, startPos, endPos);

			*out.stream << "ret\n";
			*out.stream << "F" << lex.idTable.table[i].id << " ENDP\n\n";
		}
	}


	void Code(Out::OUT out, LA::LEX lex) {
		*out.stream << ".code\n";

		Functions(out, lex);
		*out.stream << "main PROC\n";
		*out.stream << "Invoke SetConsoleCP, 1251\n";
		*out.stream << "Invoke SetConsoleOutputCP, 1251\n";
		int mainPos = 0;
		int endPos = 0;
		for (int i = 0; i < lex.lexTable.size; i++) {
			if (lex.lexTable.table[i].lexema[0] == LEX_BEGIN) {
				mainPos = i;
				break;
			}
		}
		endPos = lex.lexTable.size;
		Expression(out, lex, mainPos, endPos);
		*out.stream << "push -1\n";
		*out.stream << "call ExitProcess\n";
		*out.stream << "main ENDP\n";
		*out.stream << "end main\n";

	}

	void GenerateCode(LA::LEX lex, Out::OUT out) {
		Head(out);
		Const(out, lex);
		Data(out, lex);
		Code(out, lex);
	}

}


