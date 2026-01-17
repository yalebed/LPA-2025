#pragma once

#define ID_MAXSIZE 10
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2, BYTE = 3, SYMB = 4, TORF = 5 };
	//V - операция, F - функция, P - параметр, L - лексема, SF - стандартная библиотека
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, SF = 5, M = 6 };

	struct Entry		
	{
		int			idxfirstLE;			
		int			line;				
		char		id[ID_MAXSIZE];		
		IDDATATYPE	iddatatype;			
		IDTYPE		idtype;				
		Entry* scope;
		union
		{
			int		vint;						
			char	vbyte;						
			char	vsymb;						
			bool	vtorf;
			struct
			{
				int len;						
				char str[TI_STR_MAXSIZE - 1];	
			}	vstr[TI_STR_MAXSIZE];			
		} value;	

	};

	struct idTable              
	{
		int maxsize;            
		int size;            
		Entry* table;        
	};

	idTable Create(             
		int size               
	);

	void Add(                   
		idTable& idTable,      
		Entry entry            
	);

	Entry GetEntry(             
		idTable& idTable,      
		int n                  
	);

	int IsId(                   
		idTable& idTable,      
		char id[ID_MAXSIZE]     
	);

	int search(idTable& idTable, Entry& entry); 

	void Delete(idTable& idTable); 
}