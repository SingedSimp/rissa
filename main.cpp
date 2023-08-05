#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <fstream>
using namespace std;

class Task {
	public:
		string name;
		int status;
		string desc;
		vector<Task> subtasks;
		Task(string a, int b, string c = "", vector<Task> d = {}) {
			name = a;
			status = b;
			desc = c;
			subtasks = d;
		}
		string pprint() {
			return std::format("Task:\n\tName: {}\n\tStatus: {}\n\tDescription: {}\n\tSubtask number: {}\n",
					name, status, desc, subtasks.size());
		}
};

int main(int argc, char** argv) {
	ifstream input;
	vector<Task> data;
	string line;
	input.open(argv[1]);
	bool sub = false;
	while (getline(input, line)) {
		if (line[0] == '#' || line[0] == '(') {
			continue; // Special commands with (DISPLAY) not added yet
		} else if (line[0] == '*' || line[0] == '&' || line[0] == '-') {
			data.push_back(Task(line, 0));
		}
	}

	for (unsigned int i = 0; i < data.size(); i++) {
		cout << data[i].pprint();
	}
	
}
