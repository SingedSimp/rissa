

#include <iostream>
#include <vector>
#include <format>
#include <chrono>
#include <string>
#include <fstream>
#include "youtube.h"
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
		string pprint(string sep = "") { // Prints summary of task
			string about;
			if (desc == "") {
				about = "None";
			} else {
				about = desc;
			}
			string out = format("{0}Task:\n{0}\tName: {1}\n{0}\tStatus: {2}\n{0}\tDescription: {3}\n{0}\tSubtask amount: {4}\n",
					sep, name, status, about, subtasks.size());
			for (unsigned int i = 0; i < subtasks.size(); i++) { // Print each subtask as well, indended with tab
				out.append(subtasks[i].pprint(format("{}\t", sep))); // Uses format to recursively indend nested subtasks
			}
			return out;

		}
};

string trimwhite(string line) { // Trims starting whitespace, to get symbol
	while (line[0] == ' ' || line[0] == '\t') { // Remover chars until content
		line.erase(0,1);
	}
	return line;
}

string trimline(string line) { // Gets text from line
	line = trimwhite(line);
	line.erase(0,1); // Trims content indicator (symbol)
	line = trimwhite(line); // Must be ran twice to trim whitespace on both sides of indicator
	return line;
};

string cleanpurl(string line) { // Assumes @url is in line
	line = trimwhite(line);
	while (line[0] != '@') {
		line.erase(0,1);
	}
	line.erase(line.size()-1,1); // Trim trailing space, just in case
	while (line[line.size()-1] != ' ') {
		line.erase(line.size()-1,1);
	}
	line.erase(line.size()-1,1); // Trim trailing space, not jic
	return line;
}

vector<Task> parsedata(ifstream &input) { // Turn input file into tasks
	string line;
	vector<Task> data;
	bool sub = false; // Changes adding descriptions from tasks to subtasks
	bool sub2 = false; // Possibly could be a number to support unlimited nested subtasks
	while (getline(input, line)) {
		string cleanline = trimwhite(line);
		if (cleanline[0] == '#') { // Comments
			continue; // Special commands with (DISPLAY) not added yet
		} else if (cleanline[0] == '(') { // Cover special commands, only (DISPLAY for now)
			if (cleanline[1] == 'D') {
				string purl = cleanpurl(line); 
				system((format("./wgetchannel.sh {}", purl)).c_str());
				if (getNextLive(purl) == 0) {
					continue;
				}
				data.insert(data.begin(), Task(format("Next livestream - {}", purl), 0));
				string event = format("{}", chrono::sys_seconds{chrono::seconds{getNextLive(purl)}});
				data[0].desc.append(event);
				// Second word is channel id/channel url ending
				// Third word is upcoming/latest (live/video)	
			}
		} else if (cleanline[0] == '*') { // Main tasks
			sub = false;
			sub2 = false;
			data.insert(data.begin(), Task(trimline(line), 0));
		} else if (cleanline[0] == '&') { // Subtasks
			sub = true;
			sub2 = false;
			data[0].subtasks.insert(data[0].subtasks.begin(), Task(trimline(line), 0));
		} else if (cleanline[0] == '$') { // Nested subtasks
			sub = false;
			sub2 = true;
			data[0].subtasks[0].subtasks.insert(data[0].subtasks[0].subtasks.begin(), Task(trimline(line), 0));
		} else if (cleanline[0] == '-') { // Task & subtask descriptions
			if (sub == false && sub2 == false) {
				if (data[0].desc != "") { // Add newlines, if multiple descriptions given
					data[0].desc.append("\n");
				}
				data[0].desc.append(trimline(line));
			} else if (sub) {
				if (data[0].subtasks[0].desc != "") {
					data[0].subtasks[0].desc.append("\n");
				}
				data[0].subtasks[0].desc.append(trimline(line));
			} else if (sub2) {
				if (data[0].subtasks[0].subtasks[0].desc != "") {
					data[0].subtasks[0].subtasks[0].desc.append("\n");
				}
				data[0].subtasks[0].subtasks[0].desc.append(trimline(line));
			}
		}	
	}
	return data;
};

int main(int argc, char** argv) {
	ifstream input;
	input.open(argv[1]);
	vector<Task> data = parsedata(input);
	if (argc == 1) {
		throw invalid_argument("Input file not given");
	} else if (argc == 2) {
		for (unsigned int i = 0; i < data.size(); i++) {
			cout << data[i].pprint();
		}
	} else {
		ofstream output;
		output.open(argv[2]);
		for (unsigned int i = 0; i < data.size(); i++) {
			output << data[i].pprint();
		}
	}

	
}
