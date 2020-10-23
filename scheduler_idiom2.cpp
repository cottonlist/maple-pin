#include <iostream>
#include <pthread.h>
#include "pin.H"

using std::cout;
using std::endl;

static PIN_LOCK pinLock;

static PIN_SEMAPHORE pinSemaphore1;
static PIN_SEMAPHORE pinSemaphore2;

static int ins_no_th1 = 0;
static int ins_no_th2 = 0;

// static int idiom = 0;

// static int target_th1;
// static int target_th2;

// static int interleaving[4] = {2, 5, 1, 0};
// static int interleaving[4] = {2, 6, 0, 7};
// static int interleaving[5] = {2, 3, 2, 7, 5};
// static int interleaving[] = {1, 2, 4, 4};
static int interleaving[] = {1, 2, 4, 4};
// static int interleaving[] = {2, 3, 2, 4, 4};

// class idiom_1 {
// public:
// 	int interleaving[4];
// };

static void wait_wrapper_idiom2 (int threadid) {
	if (threadid == 1)
	{
		PIN_GetLock(&pinLock, threadid);
		cout << "Thread " << threadid << " ins " << ins_no_th1 << " starting..." << endl;
		PIN_ReleaseLock(&pinLock);

		if (interleaving[0] == 1 && interleaving[3] == ins_no_th1) 
		{
			PIN_SemaphoreWait(&pinSemaphore2);
		} 

		if (interleaving[0] == 2 && interleaving[2] == ins_no_th1)
		{
			PIN_SemaphoreWait(&pinSemaphore1);
		}
	}
	if (threadid == 2)
	{
		PIN_GetLock(&pinLock, threadid);
		cout << "Thread " << threadid << " ins " << ins_no_th2 << " starting..." << endl;
		PIN_ReleaseLock(&pinLock);

		if (interleaving[0] == 2 && interleaving[3] == ins_no_th2) 
		{
			PIN_SemaphoreWait(&pinSemaphore2);
		} 
		if (interleaving[0] == 1 && interleaving[2] == ins_no_th2)
		{
			PIN_SemaphoreWait(&pinSemaphore1);
		}
	}
}

static void signal_wrapper_idiom2 (int threadid) {
	if (threadid == 1)
	{
		if (interleaving[0] == 1 && interleaving[1] == ins_no_th1)
		{
			PIN_SemaphoreSet(&pinSemaphore1);
		}
		
		if (interleaving[0] == 2 && interleaving[2] == ins_no_th1)
		{
			PIN_SemaphoreSet(&pinSemaphore2);
		}

		PIN_GetLock(&pinLock, threadid);
		cout << "Thread " << threadid << " ins " << ins_no_th1 << " ending..." << endl;
		ins_no_th1++;
		PIN_ReleaseLock(&pinLock);
	}
	if (threadid == 2)
	{
		if (interleaving[0] == 2 && interleaving[1] == ins_no_th2)
		{
			PIN_SemaphoreSet(&pinSemaphore1);
		}

		if (interleaving[0] == 1 && interleaving[2] == ins_no_th2)
		{
			PIN_SemaphoreSet(&pinSemaphore2);
		}

		PIN_GetLock(&pinLock, threadid);
		cout << "Thread " << threadid << " ins " << ins_no_th2 << " ending..." << endl;
		ins_no_th2++;
		PIN_ReleaseLock(&pinLock);
	}
}


static void idiom_2_wrapper(INS ins, void *v) {
	IMG img = IMG_FindByAddress(INS_Address(ins));
	if (!IMG_Valid(img) || !IMG_IsMainExecutable(img))
	{
		return;
	}

	if (INS_IsMemoryRead(ins) || INS_IsMemoryWrite(ins))
	{
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wait_wrapper_idiom2, IARG_THREAD_ID, IARG_END);
		if (INS_IsValidForIpointAfter(ins))
		{
			INS_InsertCall(ins, IPOINT_AFTER, (AFUNPTR)signal_wrapper_idiom2, IARG_THREAD_ID, IARG_END);
		}
	}
}

	

int main(int argc, char *argv[])
{
	// idiom = 3;
	// int index = 0;
	// static idiom_1 iRootIdiom1[10];
	// FILE *fp;
	// char file_name[] = "idiom_1_instance";
	// fp = fopen(file_name, "r");
	// while ((fscanf(fp, "%d, %d; %d, %d;", 
	// 	&iRootIdiom1[index].interleaving[0], 
	// 	&iRootIdiom1[index].interleaving[1], 
	// 	&iRootIdiom1[index].interleaving[2], 
	// 	&iRootIdiom1[index].interleaving[3])) != EOF) {
	// 	index++;
	// }

	// fclose(fp);

	// for (int i = 0; i < index; ++i)
	// {
	// 	printf("interleaving[0~3] = %d, %d, %d, %d\n",  
	// 		iRootIdiom1[i].interleaving[0], 
	// 		iRootIdiom1[i].interleaving[1], 
	// 		iRootIdiom1[i].interleaving[2], 
	// 		iRootIdiom1[i].interleaving[3]
	// 		);
	// }

	// target_th1 = iRootIdiom1[0].interleaving[0];
	// target_th2 = iRootIdiom1[0].interleaving[2];

	PIN_InitSymbols();
	PIN_Init(argc, argv);

	PIN_InitLock(&pinLock);

	PIN_SemaphoreInit(&pinSemaphore1);
	PIN_SemaphoreInit(&pinSemaphore2);


	// if (idiom == 1)
	// 	{
	// 		INS_AddInstrumentFunction(idiom_1_wrapper, NULL);
	// 	}	
	// else if (idiom == 2)
	// {
	// 	INS_AddInstrumentFunction(idiom_2_wrapper, NULL);
	// } 
		// else if (idiom == 3) {
	// 	INS_AddInstrumentFunction(idiom_3_wrapper, NULL);
	// }

		
	INS_AddInstrumentFunction(idiom_2_wrapper, NULL);



	// if (iRootIdiom1[0].interleaving[0] == 1 && iRootIdiom1[0].interleaving[2] == 2)
	// {
	// 	INS_AddInstrumentFunction(idiom1_th1_th2, NULL);
	// }
	// if (iRootIdiom1[0].interleaving[0] == 2 && iRootIdiom1[0].interleaving[2] == 1)
	// {
	// 	INS_AddInstrumentFunction(idiom1_th2_th1, NULL);
	// }

	PIN_StartProgram();

	return 0;
}







