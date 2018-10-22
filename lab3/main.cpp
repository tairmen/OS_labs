#include <iostream>
#include <vector>

using namespace std;

struct Proc {
	int size;
	int id;
};

const int Mem_Size = 512;
const int num_of_sections = 5;
const vector <int> Mem_Sections_Size = {16, 48, 64, 128, 256};
Proc Mem_use[num_of_sections];
bool is_section_busy[num_of_sections];
vector <Proc> Ques[num_of_sections];
int Proc_Counter = 0;

void Init_Sections() {
	for (bool sect : is_section_busy) {
		sect = false;
	}
}

int find_proc_in_mem_by_id(int id) {
	for (int i = 0; i < num_of_sections; i++) {
		if (Mem_use[i].id == id) {
			return i;
		}
	}
	return -1;
}

int find_proc_in_que_by_id_and_del(int id, int que_num) {
	for (int i = 0; i < Ques[que_num].size(); i++) {
		if (Ques[que_num][i].id == id) {
			Ques[que_num].erase(Ques[que_num].begin() + i);
		}
	}
	return -1;
}

int del_proc_in_mem_by_id(int id) {
	int check = find_proc_in_mem_by_id(id);
	if (check == -1) {
		return -1;
	}
	is_section_busy[check] = false;
	return 0;
}

void Gener_Proc() {
	Proc buf;
	buf.id = Proc_Counter;
	if (rand() % 6 == 1){
		buf.size = rand() % 128 + 1;
	}
	if (rand() % 6 == 2) {
		buf.size = rand() % 256 + 1;
	}
	else{
		buf.size = rand() % 64 + 1;
	}
	Proc_Counter++;
	for (int i = 0; i < num_of_sections; i++) {
		if (buf.size <= Mem_Sections_Size[i]) {
			Ques[i].push_back(buf);
		}
	}
}

void Check_and_Push() {
	vector <int> free_sections;
	for (int i = 0; i < num_of_sections; i++) {
		if (is_section_busy[i] == false) {
			free_sections.push_back(i);
		}
	}
	Proc buf;
	for (int i : free_sections) {
		if (Ques[i].size() > 0) {
			buf = Ques[i][0];
			Mem_use[i] = buf;
			is_section_busy[i] = true;
			Ques[i].erase(Ques[i].begin());
			for (int j = 0; j < num_of_sections; j++) {
				find_proc_in_que_by_id_and_del(buf.id, j);
			}
		}
	}
}

void Print_Ques() {
	cout << "            Ques\n\n";
	for (int i = 0; i < num_of_sections; i++) {
		cout << i + 1 << " : [ ";
		for (int j = 0; j < Ques[i].size(); j++) {
			cout << "{id: " << Ques[i][j].id << ", size: " << Ques[i][j].size << "} ";
		}
		cout << "]\n";
	}
	cout << "\n\n";
}

void Print_Mem_use() {
	printf("%20s\n\n", "Memory use");
	printf("%6s%18s%22s\n", "num", "addr", "proc");
	int addr = 0;
	for (int i = 0; i < num_of_sections; i++) {
		if (is_section_busy[i] == true) {
			printf("%6d%8d%4s%6d%14s%3d%8s%5d%4s\n", i + 1, addr, "..", addr + Mem_Sections_Size[i] - 1,
				"{ id:", Mem_use[i].id, ", size:", Mem_use[i].size, " }");
			addr += Mem_Sections_Size[i];
		}
		else {
			printf("%6d%8d%4s%6d%20s%14s\n", i + 1, addr, "..", addr + Mem_Sections_Size[i] - 1, "-", " ");
			addr += Mem_Sections_Size[i];
		}
	}
	printf("\n\n");
}

void Print_Menu() {
	cout << "            Menu\n\n";
	cout << "1 - Generate processes\n";
	cout << "2 - Delete process by id\n";
	cout << "3 - Delete all processes\n";
	cout << "4 - Get process physical address\n";
	cout << "5 - Quit\n\n";
}

int main() {
	Init_Sections();
	int menu;
	Print_Menu();
	while (1) {
		cin >> menu;
		if (menu == 1) {
			int nump;
			cout << "num of processes = ";
			cin >> nump;
			for (int i = 0; i < nump; i++) {
				Gener_Proc();
			}
			cout << "\n";
			cout << "Processes generated\n\n";
		}
		if (menu == 2) {
			int id;
			cout << "id = ";
			cin >> id;
			cout << "\n";
			int check = del_proc_in_mem_by_id(id);
			if (check == -1) {
				cout << "Error: No process with id = " << id << " in memory\n\n";
				continue;
			}
		}
		if (menu == 3) {
			for (int i = 0; i < num_of_sections; i++) {
				is_section_busy[i] = false;
			}
			cout << "\n\n";
		}
		if (menu == 4) {
			int id, offset;
			cout << "proc id = ";
			cin >> id;
			int mem_id = find_proc_in_mem_by_id(id);
			if (mem_id == -1) {
				cout << "Error: No process with id = " << id << " in memory\n\n";
				continue;
			}
			cout << "virtual address = ";
			cin >> offset;
			if (Mem_use[mem_id].size <= offset) {
				cout << "Error: virtual adderss = " << offset << " dont found in proc with id = " << id <<"\n\n";
				continue;
			}
			for (int i = 0; i < mem_id; i++) {
				offset += Mem_Sections_Size[i];
			}
			cout << "physical address = " << offset << "\n\n\n";
			continue;
		}
		if (menu == 5) {
			break;
		}
		Check_and_Push();
		Print_Ques();
		Print_Mem_use();
		Print_Menu();
	}
	return 0;
}