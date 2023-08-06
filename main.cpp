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

string trimline(string line) {
	line.erase(0,1);
	while (line[0] == ' ' || line[0] == '\t') {
		line.erase(0,1);
	}
	return line;
};

vector<Task> parsedata(ifstream &input) { // Turn input file into tasks
	string line;
	vector<Task> data;
	bool sub = false; // Changes adding descriptions from tasks to subtasks
	while (getline(input, line)) {
		if (line[0] == '#' || line[0] == '(') { // Comments
			continue; // Special commands with (DISPLAY) not added yet
		} else if (line[0] == '*') { // Main tasks
			sub = false;
			data.insert(data.begin(), Task(trimline(line), 0));
		} else if (line[0] == '&') { // Subtasks
			sub = true;
			data[0].subtasks.insert(data[0].subtasks.begin(), Task(trimline(line), 0));
		} else if (line[0] == '-') { // Task & subtask descriptions
			if (sub == false) {
				if (data[0].desc != "") { // Add newlines, if multiple descriptions given
					data[0].desc.append("\n");
				}
				data[0].desc.append(trimline(line));
			} else {
				if (data[0].desc != "") {
					data[0].subtasks[0].desc.append("\n");
				}
				data[0].subtasks[0].desc.append(trimline(line));
			}
		}
	}
	return data;
};

int main(int argc, char** argv) {
	ifstream input;
	input.open(argv[1]);
	vector<Task> data = parsedata(input);
	for (unsigned int i = 0; i < data.size(); i++) {
		cout << data[i].pprint();
	}
	
}
