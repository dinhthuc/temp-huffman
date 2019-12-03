#include "NODE.h"

// Huffman coding algorithm
int main()
{

	buildHuffmanTree("text.txt");


	system("pause");
	return 0;
}

//#include <iostream>
//#include <string>
//#include <fstream>
//
//using namespace std;
//int main()
//{
//	ifstream fileInput("text.txt", ios::in);
//
//	if (fileInput.fail())
//	{
//		std::cout << "Failed to open this file!" << std::endl;
//	}
//
//	string s;
//	string file_contents;
//
//	while (getline(fileInput, s))
//	{
//		file_contents += s;
//		file_contents.push_back('\n');
//	}
//
//	cout << s << endl;
//
//	fileInput.close();
//
//
//	system("pause");
//	return 0;
//}