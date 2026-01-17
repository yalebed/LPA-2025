#include "stdafx.h"
#include "MFST.h"
#include "PolishNotation.h"
#include "SA.h"
#include "CodeGeneration.h"

int _tmain(int argc, _TCHAR* argv[]) {

    setlocale(LC_ALL, "rus");

    Log::LOG log = Log::INITLOG;
    Out::OUT out = Out::INITOUT;
    In::IN in;

    try {
        Parm::PARM parm = Parm::getparm(argc, argv);

        in = In::getin(parm.in);

        cout << in.text << endl;
        cout << "Всего символов: " << in.size << endl;
        cout << "Всего строк: " << in.lines << endl;
        cout << "Пропущено: " << in.ignore << endl;

        log = Log::getlog(parm.log);
        out = Out::getout(parm.out);

        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        cout << "Путь к выходному файлу: ";
        wcout << parm.out << endl;

        Out::WriteOut(in, parm.out);

        Log::WriteIn(log, in);
        Log::WriteForbidden(log, in);

        cout << "\nЗапуск Лексического анализа..." << endl;
        LA::LEX lexTables = LA::LA(parm, in);
        cout << "Лексический анализ завершен." << endl;

        SA::checkSemicolons(lexTables);
        SA::checkBraces(lexTables);
        SA::checkSwitchSyntax(lexTables);
        SA::conditions(lexTables);

        cout << "\nЗапуск Синтаксического анализа..." << endl;
        MFST_TRACE_START(*log.stream)
            MFST::Mfst mfst(lexTables.lexTable, GRB::getGreibach());

        if (!mfst.start(log)) {
            throw Error::geterror(600);
        }
        mfst.savededucation();
        mfst.printrules(log);
        cout << "Синтаксический анализ завершен." << endl;

        cout << "\nЗапуск Семантического анализа..." << endl;
        if (SA::startSA(lexTables)) {
            cout << "Семантический анализ завершен." << endl;
        }

        cout << "\nЗапуск построения Польской записи..." << endl;
        if (PN::startPolish(lexTables)) {
            cout << "Польская запись построена." << endl;
        }
        else {
            cout << "Польская запись не построена." << endl;
        }

        LT::PrintLT(lexTables.lexTable);

        cout << "\nЗапуск Генерации кода..." << endl;
        CodeGeneration::GenerateCode(lexTables, out);
        cout << "Генерация кода завершена." << endl;

        cout << endl << "Программа выполнена успешно!" << endl;

        Log::Close(log);
        Out::CloseOut(out);
        In::deleteIn(in);

        system(R"(D:\bstu\3sem\lpa2\lpaa\lpa\LPA-2025\compile_debug.bat)");

    }
    catch (Error::ERROR e)
    {
        cout << "Ошибка " << e.id << ": " << e.message << endl;
        if (e.inext.line != -1) {
            cout << "Строка: " << e.inext.line << ", позиция: " << e.inext.col << endl;
        }
        cout << endl;

        if (log.stream != nullptr) {
            *log.stream << "Ошибка " << e.id << ':' << e.message << endl;
            if (e.inext.line != -1) {
                *log.stream << "Строка: " << e.inext.line << ", позиция: " << e.inext.col << endl;
            }
            Log::WriteError(log, e);
            Log::Close(log);
        }

        if (out.stream != nullptr) {
            Out::WriteError(out, e);
            Out::CloseOut(out);
        }
    }
    catch (...)
    {
        cout << "Неизвестная ошибка (системный сбой)" << endl;
    }

    system("pause");
    return 0;
}