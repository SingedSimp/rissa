#include <iostream>
#include <string>
#include <format>
#include <regex>
#include <fstream>
using namespace std;

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

int main (int argc, char** argv) {
	if (argc == 1) {
		throw std::invalid_argument("No ID Given");
	} else { // Turn into function later
		if (argv[1][0] == '@') { // If personalized URL is provided
			// Filter URL with ID from file
			string query = "(\"https://www.youtube.com/channel/)(.*?)([\"?])";
			ifstream channel;
			channel.open("./tmp/@NerissaRavenCroft"); // Temp testing file, will be removed later
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
			cout << url2id(id) << endl;
		} else {
			cout << "norun" << endl;
		}
	}
}
