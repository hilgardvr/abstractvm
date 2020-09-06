#include <iostream>
#include <string>
#include <fstream>

#include "OpenRead.hpp"

void	createFile(std::string filename) {
	std::string temp;
	std::ofstream avmFile;
	avmFile.open(filename);
	while (getline(std::cin, temp)) {
		if (temp == "exit" || temp == ";;") {
			avmFile.close();
			break;
		} else {
			avmFile << temp << "\n";
		}
	} 
}


int main(int argc, char** argv) {
	if (argc == 1) {
		std::string filename = "tempavm.avm";
		createFile(filename);
		OpenRead openRead = OpenRead(filename);
		openRead.readFile();
		remove("tempavm.avm");
	} else if (argc == 2) {
		OpenRead openRead = OpenRead(argv[1]);
		openRead.readFile();
		return (0);	
	} else {
		while (argc-- > 1) {
			std::cout << argv[argc] << std::endl;
		}
		std::cout << "Invalid number of parameters\n";
		return (0);
	}
}
