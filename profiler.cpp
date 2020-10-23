#include <pthread.h>
#include <iostream>
#include <fstream>
#include "pin.H"

using std::cout;
using std::endl;

typedef struct mem_access {
	int id;
	int* ins_addr;
	int* accessed_mem_addr;
	int thread_id;
} mem_access_t;

FILE * f1;
FILE *f2;

static int i1 = 0;
static int i2 = 0;

// static int finish = 0;

static mem_access_t accesses_t1[10];
static mem_access_t accesses_t2[10];

static void profile(int* ins_addr, int* accessed_addr, THREADID threadid) {
	if (threadid == 0)
	{
		return;
	}

	if (threadid == 1)
	{
		accesses_t1[i1].ins_addr = ins_addr;
		accesses_t1[i1].accessed_mem_addr = accessed_addr;
		accesses_t1[i1].thread_id = threadid;
		cout << accesses_t1[i1].ins_addr << " " << 
		accesses_t1[i1].accessed_mem_addr << " " << accesses_t1[i1].thread_id
		<< endl;
		fprintf(f1, "%p, %p, %d\n", ins_addr, accessed_addr, threadid);
		
	} else if (threadid == 2)
	{
		accesses_t2[i2].ins_addr = ins_addr;
		accesses_t2[i2].accessed_mem_addr = accessed_addr;
		accesses_t2[i2].thread_id = threadid;
		cout << accesses_t2[i2].ins_addr << " " << 
		accesses_t2[i2].accessed_mem_addr << " " << accesses_t2[i2].thread_id
		<< endl;		
		fprintf(f2, "%p, %p, %d\n", ins_addr, accessed_addr, threadid);
	}
}

// static void compare() {
// 	// cout << "compare() is called. \n" << endl;
// 	for (int i = 0; i < i1; ++i)
// 	{
// 		for (int j = 0; j < i2; ++j)
// 		{
// 			if (accesses_t1[i].accessed_mem_addr == accesses_t2[j].accessed_mem_addr)
// 			{
// 				cout << "No. " << i << " in th1 and no. " << j << " in th2 access same address. " << endl;
// 			}
// 		}
// 	}
// 	finish++;
// }

// static void output(INS ins, void *v) {
// 	IMG img = IMG_FindByAddress(INS_Address(ins));
// 	if (!IMG_Valid(img) || !IMG_IsMainExecutable(img))
// 	{
// 		return;
// 	}
// 	if (finish == 1)
// 	{
// 		return;
// 	}
// 	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)compare, IARG_END);
// }

static void instrument_profiler(INS ins, void *v) {
	IMG img = IMG_FindByAddress(INS_Address(ins));
	if (!IMG_Valid(img) || !IMG_IsMainExecutable(img))
	{
		return;
	}
	if (INS_IsMemoryRead(ins))
	{
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)profile, IARG_INST_PTR, 
			IARG_MEMORYREAD_EA, IARG_THREAD_ID, IARG_END);
	} 
	if (INS_IsMemoryWrite(ins))
	{
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)profile, IARG_INST_PTR, 
			IARG_MEMORYWRITE_EA, IARG_THREAD_ID, IARG_END);
	}
}

VOID Fini(INT32 code, VOID *v){
	// fprintf(mem_access, "#eof\n");
	fclose(f1);
	fclose(f2);
}

int main(int argc, char *argv[])
{
	PIN_InitSymbols();
	PIN_Init(argc, argv);

	f1 = fopen("memory_accesses_th1.log", "w");
	f2 = fopen("memory_accesses_th2.log", "w");

	INS_AddInstrumentFunction(instrument_profiler, NULL);
	// INS_AddInstrumentFunction(output, NULL);

	PIN_AddFiniFunction(Fini, 0);

	PIN_StartProgram();
	
	return 0;
}