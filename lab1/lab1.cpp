#include "lab1.h"

int main() {
	int num_of_tests = 1;
	int num_of_tasks = 10;
	for (int i = 0; i < num_of_tests; i++) {
		init_task_container(num_of_tasks);
		FCFS();
		LCFS();
		common_task_container.clear();
	}
	double buf = 0;
	for (int i = 0; i < num_of_tests; i++) {
		buf += avgs_FCFS[i];
	}
	cout << "\n\nglobal waiting avg for FCFS = " << buf / num_of_tests << endl;
	buf = 0;
	for (int i = 0; i < num_of_tests; i++) {
		buf += avgs_LCFS[i];
	}
	cout << "global waiting avg for LCFS = " << buf / num_of_tests << endl;
	system("pause");
	return 0;
}

void FCFS() {
	int timer = 0;
	vector <task> task_container = common_task_container;
	is_proc_free = true;
	is_syst_work = true;
	task proc_task;
	int proc_working = 0;
	vector <task> report;
	 while (is_syst_work) {
		while ((task_container.size() != 0) && (task_container[task_container.size() - 1].arr == timer)) {
			que.push_back(task_container[task_container.size() - 1]);
			task_container.pop_back();
		}
		if ((is_proc_free) && (que.size() > 0)) {
			proc_task = que[0];
			que.erase(que.begin());
			proc_working = proc_task.perf;
			is_proc_free = false;
		}
		if (proc_working > 0) {
			proc_working--;
		}
		else {
			if (!is_proc_free) {
				is_proc_free = true;
				report.push_back(proc_task);
			}
		}
		for (int i = 0; i < que.size(); i++) {
			que[i].waiting++;
		}
		if ((task_container.size() == 0) && (que.size() == 0) && (is_proc_free)) {
			is_syst_work = false;
		}
		timer++;
	}
	report = sort_by_arr_time(report, 1);
	cout << setw(25) << "FCFS\n";
	cout << setw(10) << "N" << setw(12) << "arr" << setw(14) << "perf" << setw(14) << "wait\n";
	for (int i = 0; i < report.size(); i++) {
		cout << setw(10) << i + 1 << " | " << setw(10) << report[i].arr << " | "
			<< setw(10) << report[i].perf << " | " << setw(10) << report[i].waiting << endl;
	}
	double avg_wait = 0;
	for (int i = 0; i < report.size(); i++) {
		avg_wait += report[i].waiting;
	}
	avg_wait = avg_wait / report.size();
	avgs_FCFS.push_back(avg_wait);
	cout << "avg waiting time = " << avg_wait << endl;
	task_container.clear();
	que.clear();
	cout << "\n";
}

void LCFS() {
	int timer = 0;
	vector <task> task_container = common_task_container;
	is_proc_free = true;
	is_syst_work = true;
	task proc_task;
	int proc_working = 0;
	vector <task> report;
	while (is_syst_work) {
		while ((task_container.size() != 0) && (task_container[task_container.size() - 1].arr == timer)) {
			que.push_back(task_container[task_container.size() - 1]);
			task_container.pop_back();
		}
		if ((is_proc_free) && (que.size() > 0)) {
			proc_task = que[que.size() - 1];
			que.pop_back();
			proc_working = proc_task.perf;
			is_proc_free = false;
		}
		if (proc_working > 0) {
			proc_working--;
		}
		else {
			if (!is_proc_free) {
				is_proc_free = true;
				report.push_back(proc_task);
			}
		}
		for (int i = 0; i < que.size(); i++) {
			que[i].waiting++;
		}
		if ((task_container.size() == 0) && (que.size() == 0) && (is_proc_free)) {
			is_syst_work = false;
		}
		timer++;
	}
	report = sort_by_arr_time(report, 1);
	cout << setw(25) << "LCFS\n";
	cout << setw(10) << "N" << setw(12) << "arr" << setw(14) << "perf" << setw(14) << "wait\n";
	for (int i = 0; i < report.size(); i++) {
		cout << setw(10) << i + 1 << " | " << setw(10) << report[i].arr << " | "
			<< setw(10) << report[i].perf << " | " << setw(10) << report[i].waiting << endl;
	}
	double avg_wait = 0;
	for (int i = 0; i < report.size(); i++) {
		avg_wait += report[i].waiting;
	}
	avg_wait = avg_wait / report.size();
	avgs_LCFS.push_back(avg_wait);
	cout << "avg waiting time = " << avg_wait << endl;
	task_container.clear();
	que.clear();
	cout << "\n";
}

void init_task_container(int num) {
	task buf;
	for (int i = 0; i < num; i++) {
		buf.arr = 1;
		buf.perf = num - i + 1;
		buf.waiting = 0;
		common_task_container.push_back(buf);
	}
	cout << endl;
	common_task_container = sort_by_arr_time(common_task_container, 0);
}

vector <task> sort_by_arr_time(vector <task> vect, int cmp) {
	task buf;
	if (cmp == 1) {
		for (int i = 1; i < vect.size(); i++) {
			buf = vect[i];
			int j = i;
			while ((j > 0) && (vect[j - 1].arr > buf.arr)) {
				vect[j] = vect[j - 1];
				j--;
			}
			vect[j] = buf;
		}
	}
	else {
		for (int i = 1; i < vect.size(); i++) {
			buf = vect[i];
			int j = i;
			while ((j > 0) && (vect[j - 1].arr < buf.arr)) {
				vect[j] = vect[j - 1];
				j--;
			}
			vect[j] = buf;
		}
	}
	return vect;
}