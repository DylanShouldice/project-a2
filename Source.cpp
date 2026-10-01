#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct STUDENT_DATA {
	std::string firstName;
	std::string lastName;
};

std::string trim(const std::string& name) {
	const auto first = name.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) {
		return "";
	}
	const auto last = name.find_last_not_of(" \t\r\n");
	return name.substr(first, last - first + 1);
}

int main(void) {
	std::ifstream inputFile("StudentData.txt");
	if (!inputFile) {
		std::cerr << "Unable to open StudentData.txt\n";
		return 1;
	}

	std::vector<STUDENT_DATA> students;
	std::string line;
	while (std::getline(inputFile, line)) {
		if (trim(line).empty()) {
			continue;
		}

		const auto comma = line.find(',');
		if (comma == std::string::npos) {
			std::cerr << "Invalid student record: " << line << '\n';
			return 1;
		}

		// StudentData.txt stores each record as LastName, FirstName.
		STUDENT_DATA student;
		student.lastName = trim(line.substr(0, comma));
		student.firstName = trim(line.substr(comma + 1));
		if (student.firstName.empty() || student.lastName.empty()) {
			std::cerr << "Invalid student record: " << line << '\n';
			return 1;
		}
		students.push_back(student);
	}

	if (inputFile.bad()) {
		std::cerr << "Error reading StudentData.txt\n";
		return 1;
	}

	std::cout << "Loaded " << students.size() << " students.\n";
	for (const STUDENT_DATA& student : students) {
		std::cout << student.firstName << ' ' << student.lastName << '\n';
	}

	return 0;
}
