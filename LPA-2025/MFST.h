#pragma once
#include "GRB.h"
#include "stdafx.h"

//макросы для трассировки работы синтаксического анализатора 
#define MFST_TRACE_START(stream) stream<< std::setw(4)<<std::left<<"Шаг"<<":"\
								  << std::setw(20)<<std::left<<" Правило"\
								  << std::setw(30)<<std::left<<" Входная лента"\
								  << std::setw(20)<<std::left<<" Стек"\
								  << std::endl;

//вывод шага, ПРАВИЛО перехода, текущего содержимого входной ленты и стека
#define MFST_TRACE1(stream)		 stream<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;
//вывод шага, текущего содержимого входной ленты и стека: 
//после определения нового правила перехода или восстановления состояния
#define MFST_TRACE2(stream)	 stream<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

//вывод шага, текущего содержимого входной ленты и стека: обычный шаг трассировки
#define MFST_TRACE3(stream)		 stream<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

//вывод сообщения с увеличением шага: ошибка в результате трассировки
#define MFST_TRACE4(stream, c)		stream<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
//вывод сообщения без увеличения шага: восстановление состояния автомата
#define MFST_TRACE5(stream, c)		stream<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
//сохранение текущего состояния автомата
#define MFST_TRACE6(stream, c,k)	stream<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;
//вывод правил грамматики
#define MFST_TRACE7(stream)		stream<<std::setw(4)<<std::left << state.lenta_position << ": "\
							<<std::setw(20)<< std::left << rule.getCRule(rbuf,state.nrulechain)\
							<<std::endl;
#define MFST_DIAGN_MAXSIZE 2* ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

class MFSTSTSTACK :public std::stack<short> {
public:
	using std::stack<short>::c; 
};

static int FST_TRACE_n = -1;	
static char rbuf[205], sbuf[205], lbuf[1024];	

namespace MFST
{
	struct MfstState					//состояние автомата для сохранения
	{
		short lenta_position;			
		short nrule;					
		short nrulechain;			
		MFSTSTSTACK st;				
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);	
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
	};

	class MFSTSTSTATE :public std::stack<MfstState> {
	public:
		using std::stack<MfstState>::c;
	};
	struct Mfst							//магазинный автомат
	{
		enum RC_STEP {					
			NS_OK,					
			NS_NORULE,				
			NS_NORULECHAIN,			
			NS_ERROR,				
			TS_OK,					
			TS_NOK,					
			LENTA_END,				
			SURPRISE,			
		};
		struct MfstDiagnosis		
		{
			short lenta_position;		
			RC_STEP rc_step;			
			short nrule;				
			short nrule_chain;			
			MfstDiagnosis();			
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);

		} diagnosis[MFST_DIAGN_NUMBER]; 	

		GRBALPHABET* lenta;					
		short lenta_position;				
		short nrule;						
		short nrulechain;					
		short lenta_size;					
		GRB::Greibach grebach;				
		LT::lexTable lex;				
		MFSTSTSTACK st;				
		MFSTSTSTATE storestate;		
		Mfst();
		Mfst(LT::lexTable& plex, GRB::Greibach pgrebach);
		char* getCSt(char* buf);			
		char* getCLenta(char* buf, short pos, short n = 25);	
		char* getDiagnosis(short n, char* buf);					
		bool savestate(Log::LOG& log);					
		bool reststate(Log::LOG& log);					
		bool push_chain(GRB::Rule::Chain chain);		
		RC_STEP step(Log::LOG& log);						
		bool start(Log::LOG& log);						
		bool savediagnosis(RC_STEP pprc_step);		
		void printrules(Log::LOG& log);					

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation()
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;
		bool savededucation();
	};
}
