#include <iostream>
#include <string>
#include <format>
#include <regex>
#include <fstream>
#include <chrono>
using namespace std;

// TODO: Get upcoming livestream start time with startTime on youtube html page
// TODO: Get latest video from youtube rss

string url2id (string url) {
	string::difference_type count = std::count(url.begin(), url.end(), '/');
	while (count != 0) {
		if (url[0] == '/') {
			count--;
		}
		url.erase(0,1);
	}
	return url;
}

string getChannelId (string customurl) {
	// Filter URL with ID from file
	string query = "(\"https://www.youtube.com/channel/)(.*?)([\"?])";
	ifstream channel;
	channel.open(format("/tmp/rissa/{}", customurl));
	string line;
	string id;
	while (getline(channel, line) && id.empty()) {
		// Make regex query
		regex rgx(query);
		// Hold matches
		smatch url;
		// Search file
		regex_search(line, url, rgx);
		// Get url id from query
		if (url[0] != "") {
			id = url[0];
			id.erase(0,1);
			id.erase(id.size() - 1, 1);
		}
	}
	return url2id(id);
}

int getStartTime(string fullTime) {
	string startTime = "";
	for (unsigned int i = 0; i < fullTime.size(); i++) {
		if (isdigit(fullTime[i])) {
			startTime.push_back(fullTime[i]);
		}
	}
	return stoi(startTime);
}

int getNextLive(string fileName) {
	string query = "(\"startTime\")(.*?)(\",\")";
	ifstream file;
	file.open(format("/tmp/rissa/{}", fileName));
	string line;
	string startTime;
	while (getline(file, line) && startTime.empty()) {
		regex rgx(query);
		smatch utime;
		regex_search(line, utime, rgx);
		if (utime[0] != "") {
			startTime = utime[0];
		}
	}
	int TZ = -14400; // SET UTC TIMEZONE OFFSET HERE
	return getStartTime(startTime)+TZ;

}


int main (int argc, char** argv) {
	if (argc == 1) {
		throw std::invalid_argument("No ID Given");
	} else { // Turn into function later
		if (argv[1][0] == '@') { // If personalized URL is provided
			string purl = argv[1];
			// Not sure how to guarentee script is in $PATH
			system((format("./wgetchannel.sh {}", purl)).c_str());
			cout << getChannelId(purl) << endl;
			cout << chrono::sys_seconds{chrono::seconds(getNextLive(purl))} << endl;
		}
	}
}
