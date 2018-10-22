#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore {
public:
	Semaphore(int count_ = 0)
		: count(count_) {}

	inline void v(){
		std::unique_lock<std::mutex> lock(mtx);
		count++;
		if (count <= 0) {
			cv.notify_one();
		}
	}

	inline void p(){
		std::unique_lock<std::mutex> lock(mtx);
		count--;
		while (count < 0) {
			cv.wait(lock);
		}
	}

private:
	std::mutex mtx;
	std::condition_variable cv;
	int count;
};

map <int, int> Monets;
Semaphore S1;
Semaphore S2;
vector <int> na_razmen = { 1, 2, 5, 10, 25, 50, 100 };
int tasks_num;
bool is_money_in;
int money_in;
int money_out;

void Init_Monets() {
	Monets[na_razmen[0]] = 1000;
	Monets[na_razmen[1]] = 500;
	Monets[na_razmen[2]] = 200;
	Monets[na_razmen[3]] = 80;
	Monets[na_razmen[4]] = 30;
	Monets[na_razmen[5]] = 10;
	Monets[na_razmen[6]] = 0;
}

int find_money_index(int money) {
	int i_end;
	for (unsigned int i = na_razmen.size() - 1; i >= 0; i--) {
		if (money >= na_razmen[i]) {
			i_end = i;
			break;
		}
	}
	return i_end;
}

int Razmen(int dengi, int moneta) {
	int res = dengi / moneta;
	if (Monets[moneta] - res > 0) {
		Monets[moneta] = Monets[moneta] - res;
		if (dengi % moneta != 0) {
			return 0;
		}
		return res;
	}
	else {
		return 0;
	}
}

map <int, int> Neopr_Razmen(int dengi) {
	int i_end = find_money_index(dengi);
	map <int, int> res;
	for (unsigned int i = 0; i < i_end; i++) {
		res[na_razmen[i]] = 0;
	}
	for (unsigned int i = i_end - 1; i >= 0; i--) {
		int moneta = na_razmen[i];
		if (Monets[moneta] - (dengi / moneta) > 0) {
			res[moneta] = dengi / moneta;
			if (dengi % moneta != 0) {
				moneta = dengi % moneta;
				dengi = moneta;
				i = find_money_index(moneta) + 1;
				continue;
			}
			else {
				for (unsigned int i = 0; i < i_end; i++) {
					Monets[na_razmen[i]] -= res[na_razmen[i]];
				}
				return res;
			}
		}
		else {
			if (Monets[moneta] == 0) {
				continue;
			}
			else {
				res[moneta] = Monets[moneta];
				dengi = dengi - res[moneta] * moneta;
				continue;
			}
		}
	}
	res[1] = -1;
	return  res;
}

void RandGenerationMoney() {
	int denga_i = rand() % (na_razmen.size() - 1) + 1;
	money_in = na_razmen[denga_i];
	is_money_in = true;
	money_out = na_razmen[rand() % denga_i];
}

void PrintMonets(map <int, int> Monets) {
	cout << "{";
	for (auto elem : Monets){
		cout << elem.first << "$: " << elem.second << "; ";
	}
	cout << "}" << endl;
}

void procA() {
	int counter = 0;
	while (tasks_num > 0) {
		while (!is_money_in) {
			if (counter % 10000 == 1) {
				PrintMonets(Monets);
				RandGenerationMoney();
				tasks_num--;
			}
			if (counter > 10000000) {
				counter = 0;
			}
			counter++;
		}
		Monets[money_in]++;
		printf("proc A: input %d$ want %d$\n", money_in, money_out);
		S1.v();
		S2.p();
		is_money_in = false;
	}
}

void procB() {
	int res;
	while (tasks_num > 0) {
		S1.p();
		res = Razmen(money_in, money_out);
		if (res == 0) {
			Monets[money_in]--;
			printf("proc B: exchange impossible\n");
		}
		else {
			printf("proc B: get %d to %d$\n", res, money_out);
		}
		S2.v();
	}
}

void procA1() {
	int counter = 0;
	while (tasks_num > 0) {
		while (!is_money_in) {
			if (counter % 10000 == 1) {
				PrintMonets(Monets);
				RandGenerationMoney();
				tasks_num--;
			}
			if (counter > 10000000) {
				counter = 0;
			}
			counter++;
		}
		Monets[money_in]++;
		printf("proc A: input %d$\n", money_in);
		S1.v();
		S2.p();
		is_money_in = false;
	}
}

void procB1() {
	map <int, int> res;
	while (tasks_num > 0) {
		S1.p();
		res = Neopr_Razmen(money_in);
		if (res[1] == -1) {
			Monets[money_in]--;
			printf("proc B: exchange impossible\n");
		}
		else {
			cout << "proc B: get ";
			PrintMonets(res);
		}
		res.clear();
		S2.v();
	}
}

int main() {
	Init_Monets();
	tasks_num = 200;
	is_money_in = false;
	thread t1(procA);
	thread t2(procB);
	t1.join();
	t2.join();
	system("pause");
}