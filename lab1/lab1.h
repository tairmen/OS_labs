#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

struct task {
	int arr;
	int perf;
	int waiting;
};

vector <task> que;
vector <task> common_task_container;
bool is_proc_free;
bool is_syst_work;
vector <int> avgs_FCFS;
vector <int> avgs_LCFS;

void FCFS();
void LCFS();
void init_task_container(int num);
vector <task> sort_by_arr_time(vector <task> vect, int cmp);
