#pragma once
#include "GRB.h"
#include"Error.h"
#define GRB_ERROR_SERIES 600
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),
		8,
		// S - Стартовый символ
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			4,
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(9, TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(6, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')), 
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))                    
		),

		// N - Операторы 
		Rule(NS('N'), GRB_ERROR_SERIES + 1,
			19,
			Rule::Chain(12, TS('s'), TS('('), NS('B'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')), 
			Rule::Chain(11, TS('s'), TS('('), NS('B'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),         
			Rule::Chain(8, TS('s'), TS('('), NS('B'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),                                     
			Rule::Chain(7, TS('s'), TS('('), NS('B'), TS(')'), TS('{'), NS('N'), TS('}')),                                             
			Rule::Chain(7, TS('s'), TS('('), NS('i'), TS(')'), TS('{'), NS('N'), TS('}')),                                        

			// Switch
			Rule::Chain(8, TS('h'), TS('('), TS('i'), TS(')'), TS('{'), NS('K'), TS('}'), NS('N')), 
			Rule::Chain(7, TS('h'), TS('('), TS('i'), TS(')'), TS('{'), NS('K'), TS('}')),         

			Rule::Chain(8, TS('h'), TS('('), TS('l'), TS(')'), TS('{'), NS('K'), TS('}'), NS('N')), 
			Rule::Chain(7, TS('h'), TS('('), TS('l'), TS(')'), TS('{'), NS('K'), TS('}')),

			Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')), 
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),                  
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),                           
			Rule::Chain(3, TS('d'), TS('t'), NS('N')),                                    

			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),      

			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),

			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 2,
			15, 

			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')), 
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),          

			// Стандартные функции 
			Rule::Chain(5, TS('C'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('C'), TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('T'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('T'), TS('('), NS('W'), TS(')')),

			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')), 
			Rule::Chain(3, TS('('), NS('E'), TS(')')),        

			//  Унарные операции
			Rule::Chain(2, TS('['), NS('E')), 
			Rule::Chain(2, TS(']'), NS('E')), 
			Rule::Chain(2, TS('~'), NS('E')), 

			Rule::Chain(2, TS('i'), NS('M')), 
			Rule::Chain(2, TS('l'), NS('M')), 

			Rule::Chain(1, TS('i')), 
			Rule::Chain(1, TS('l'))  
		),

		// W - Параметры функций
		Rule(NS('W'), GRB_ERROR_SERIES + 3,
			4,
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 4,
			2,
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),
			Rule::Chain(2, TS('t'), TS('i'))
		),

		Rule(NS('M'), GRB_ERROR_SERIES + 5,
			8,
			Rule::Chain(3, TS('v'), NS('E'), NS('M')), 
			Rule::Chain(2, TS('v'), NS('E')),         

			Rule::Chain(6, TS('('), NS('E'), TS(')'), TS('v'), TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), TS('v')),

			Rule::Chain(2, TS('^'), NS('M')), 
			Rule::Chain(1, TS('^')),          // a++
			Rule::Chain(2, TS('_'), NS('M')), 
			Rule::Chain(1, TS('_'))           // a--
		),

		Rule(NS('B'), GRB_ERROR_SERIES + 6,
			4,
			Rule::Chain(3, TS('i'), TS('b'), TS('i')),
			Rule::Chain(3, TS('i'), TS('b'), TS('l')),
			Rule::Chain(3, TS('l'), TS('b'), TS('i')),
			Rule::Chain(3, TS('l'), TS('b'), TS('l'))
		),

		Rule(NS('K'), GRB_ERROR_SERIES + 7,
			4,
			Rule::Chain(7, TS('c'), TS('l'), TS(':'), TS('{'), NS('N'), TS('}'), NS('K')), 
			Rule::Chain(6, TS('c'), TS('l'), TS(':'), TS('{'), NS('N'), TS('}')),          
			Rule::Chain(6, TS('a'), TS(':'), TS('{'), NS('N'), TS('}'), NS('K')),          
			Rule::Chain(5, TS('a'), TS(':'), TS('{'), NS('N'), TS('}'))                    
		)
	);
}