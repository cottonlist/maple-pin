#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

int count_line(char *);
void create_idiom1_instance(int, int);
void create_idiom2_instance_1(int, int, int);
void create_idiom2_instance_2(int, int, int);
void create_idiom3_instance_1(int, int, int, int);
void create_idiom3_instance_2(int, int, int, int);

// class memory_access
// {
// 	string line;	
// };

class memory_access {
public:
	int* ins_addr;
	int* accessed_addr;
	int thd;
};

using namespace std;

// using std::string;
// using std::ifstream;
// using std::cout;
// using std::endl;

int main()
{
	int num_of_ins_th1 = 0;
	int num_of_ins_th2 = 0;

	FILE *fp1;
	FILE *fp2;

	char fname1[] = "memory_accesses_th1.log";
	char fname2[] = "memory_accesses_th2.log";

	num_of_ins_th1 = count_line(fname1);
	num_of_ins_th2 = count_line(fname2);

	// int chr;
	int * i1;
	int * i2;
	int i3;

	char ch1;
	char ch2;

	int index_th1 = 0;
	int index_th2 = 0;

	memory_access access_th1[20];
	memory_access access_th2[20];


	// fp1 = fopen(fname1, "r");
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");

	while((fscanf(fp2, "%p, %p, %d", &i1, &i2, &i3)) != EOF) {
		access_th2[index_th2].ins_addr = i1;
		access_th2[index_th2].accessed_addr = i2;
		access_th2[index_th2].thd = i3;
		index_th2++;
	}

	while((fscanf(fp1, "%p, %p, %d", &i1, &i2, &i3)) != EOF) {
		access_th1[index_th1].ins_addr = i1;
		access_th1[index_th1].accessed_addr = i2;
		access_th1[index_th1].thd = i3;
		index_th1++;
	}

	
	FILE *instance;
	instance = fopen("idiom_1_iRoot", "w");
	fclose(instance);

	for (int i = 0; i < num_of_ins_th1; ++i)
	{
		for (int j = 0; j < num_of_ins_th2; ++j)
		{
			if (access_th1[i].accessed_addr == access_th2[j].accessed_addr)
			{
				create_idiom1_instance(i, j);
				// printf("No.%d ins in th1 and no.%d ins in th2 are accessing the same address.\n", i, j);
			}
		}
	}

	FILE *iRoot2;
	iRoot2 = fopen("idiom_2_iRoot", "w");
	fclose(iRoot2);

	for (int i = 0; i < num_of_ins_th1; ++i)
	{
		for (int j = 0; j < num_of_ins_th2; ++j)
		{
			for (int k = i + 1; k < num_of_ins_th1; ++k) {
				if (access_th1[i].accessed_addr == access_th2[j].accessed_addr 
					&& access_th1[i].accessed_addr == access_th1[k].accessed_addr) {
					create_idiom2_instance_1(i, j, k);
				}
			}
		}
	}

	for (int i = 0; i < num_of_ins_th2; ++i)
	{
		for (int j = 0; j < num_of_ins_th1; ++j)
		{
			for (int k = i + 1; k < num_of_ins_th2; ++k) {
				if (access_th2[i].accessed_addr == access_th1[j].accessed_addr 
					&& access_th2[i].accessed_addr == access_th2[k].accessed_addr) {
					create_idiom2_instance_2(i, j, k);
				}
			}
		}
	}

	FILE *iRoot3;
	iRoot3 = fopen("idiom_3_iRoot", "w");
	fclose(iRoot3);

	for (int i = 0; i < num_of_ins_th1; ++i)
	{
		for (int j = 0; j < num_of_ins_th2; ++j)
		{
			for (int m = i + 1; m < num_of_ins_th1; ++m)
			{
				for (int n = j + 1; n < num_of_ins_th2; ++n)
				{
					if (access_th1[i].accessed_addr == access_th2[j].accessed_addr
						&& access_th1[i].accessed_addr == access_th1[m].accessed_addr
						&& access_th2[j].accessed_addr == access_th2[n].accessed_addr)
					{
						create_idiom3_instance_1(i, j, m, n);
					}
				}
			}
		}
	}

	for (int i = 0; i < num_of_ins_th2; ++i)
	{
		for (int j = 0; j < num_of_ins_th1; ++j)
		{
			for (int m = i + 1; m < num_of_ins_th2; ++m)
			{
				for (int n = j + 1; n < num_of_ins_th1; ++n)
				{
					if (access_th2[i].accessed_addr == access_th1[j].accessed_addr
						&& access_th2[i].accessed_addr == access_th2[m].accessed_addr
						&& access_th1[j].accessed_addr == access_th1[n].accessed_addr)
					{
						create_idiom3_instance_2(i, j, m, n);
					}
				}
			}
		}
	}

	for (int i = 0; i < num_of_ins_th1; ++i)
	{
		printf("%p, %p\n", access_th1[i].ins_addr, access_th1[i].accessed_addr);
	}
	for (int i = 0; i < num_of_ins_th2; ++i)
	{
		printf("%p, %p\n", access_th2[i].ins_addr, access_th2[i].accessed_addr);
	}

	fclose(fp1);
	fclose(fp2);

	printf("Number of ins in th1: %d\n", num_of_ins_th1);
	printf("Number of ins in th2: %d\n", num_of_ins_th2);

	// FILE *fp;
	// int count = 0;
	// char file_name[] = "memory_accesses_th1.log";
	// char c;

	// fp = fopen(file_name, "r");

	// for (c = getc(fp); c != EOF; c = getc(fp))
	// {
	// 	if (c == '\n')
	// 	{
	// 		count++;
	// 	}
	// }

	// char file_name[] = "memory_accesses_th1.log";
	// int count = 0;
	// count = count_line(file_name);

	// printf("Line number of memory_accesses_th1.log is %d\n", count);

	// for (int i = 0; i < num_of_ins_th1-1; ++i)
	// {
	// 	printf("%p, %p, %d\n", 
	// 		access_th1[index_th1].ins_addr, 
	// 		access_th1[index_th1].accessed_addr, 
	// 		access_th1[index_th1].thd);
	// }

	// for (int i = 0; i < num_of_ins_th2-1; ++i)
	// {
	// 	printf("%p, %p, %d\n", 
	// 		access_th2[index_th2].ins_addr, 
	// 		access_th2[index_th2].accessed_addr, 
	// 		access_th2[index_th2].thd);
	// }

	// ofstream file("memory_access.log");
	// file << "Try to write to file.";
	// file.close();
	// string line;
	// ifstream file("memory_accesses_th1.log");
	// // if (file_1.is_open())
	// {
	// 	while(getline(file_1, line)){
	// 		cout << line << endl;
	// 	}
	// 	file_1.close();
	// }

	// else cout << "Unable to open file." << endl;

	return 0;
}

int count_line(char *file_name) {
	FILE *fp;
	int count = 0;
	char c;
	fp = fopen(file_name, "r");
	for(c = getc(fp); c != EOF; c = getc(fp)) {
		if (c == '\n')
		{
			count++;
		}
	}
	fclose(fp);
	return count;
}

void create_idiom1_instance(int ins1, int ins2){
	FILE *fp;
	fp = fopen("idiom_1_iRoot", "a");
	fprintf(fp, "%d, %d, %d\n", 1, ins1, ins2);
	fprintf(fp, "%d, %d, %d\n", 2, ins2, ins1);
	fclose(fp);
}

void create_idiom2_instance_1(int ins1, int ins2, int ins3) {
	FILE *fp;
	fp = fopen("idiom_2_iRoot", "a");
	fprintf(fp, "%d, %d, %d, %d\n", 1, ins1, ins2, ins3);
	fclose(fp);
}

void create_idiom2_instance_2(int ins1, int ins2, int ins3) {
	FILE *fp;
	fp = fopen("idiom_2_iRoot", "a");
	fprintf(fp, "%d, %d, %d, %d\n", 2, ins1, ins2, ins3);
	fclose(fp);
}

void create_idiom3_instance_1(int ins1, int ins2, int ins3, int ins4)
{
	FILE *fp;
	fp = fopen("idiom_3_iRoot", "a");
	fprintf(fp, "%d, %d, %d, %d, %d\n", 1, ins1, ins2, ins3, ins4);
	fclose(fp);
}

void create_idiom3_instance_2(int ins1, int ins2, int ins3, int ins4)
{
	FILE *fp;
	fp = fopen("idiom_3_iRoot", "a");
	fprintf(fp, "%d, %d, %d, %d, %d\n", 2, ins1, ins2, ins3, ins4);
	fclose(fp);
}