#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct STUDENT_DATA {
	std::string firstName;
	std::string lastName;
#ifdef PRE_RELEASE
	std::string email;
#endif
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
#ifdef PRE_RELEASE
	std::cout << "Running pre-release source code.\n";
	const char* inputFileName = "StudentData_Emails.txt";
#else
	std::cout << "Running standard source code.\n";
	const char* inputFileName = "StudentData.txt";
#endif

	std::ifstream inputFile(inputFileName);
	if (!inputFile) {
		std::cerr << "Unable to open " << inputFileName << '\n';
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

		// Both files store the last name before the first name.
		STUDENT_DATA student;
		student.lastName = trim(line.substr(0, comma));
#ifdef PRE_RELEASE
		const auto emailComma = line.find(',', comma + 1);
		if (emailComma == std::string::npos) {
			std::cerr << "Missing email in student record: " << line << '\n';
			return 1;
		}
		student.firstName = trim(line.substr(comma + 1, emailComma - comma - 1));
		student.email = trim(line.substr(emailComma + 1));
		if (student.email.empty()) {
			std::cerr << "Missing email in student record: " << line << '\n';
			return 1;
		}
#else
		student.firstName = trim(line.substr(comma + 1));
#endif
		if (student.firstName.empty() || student.lastName.empty()) {
			std::cerr << "Invalid student record: " << line << '\n';
			return 1;
		}
		students.push_back(student);
	}

	if (inputFile.bad()) {
		std::cerr << "Error reading " << inputFileName << '\n';
		return 1;
	}

#ifdef _DEBUG
	std::cout << "Loaded " << students.size() << " students.\n";
	for (const STUDENT_DATA& student : students) {
		std::cout << student.firstName << ' ' << student.lastName;
#ifdef PRE_RELEASE
		std::cout << ", " << student.email;
#endif
		std::cout << '\n';
	}
#endif

	return 0;
}
