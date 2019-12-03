#include "NODE.h"

Node * getNode(char ch, int freq, Node * left, Node * right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

void encode(Node * root, string str, unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	// là node lá
	if (!root->left && !root->right) 
	{
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode); // qua trái thì thêm 0
	encode(root->right, str + "1", huffmanCode); // qua phải thêm 1
}

void decode(Node * root, int & index, string str)
{
	fstream Decode;
	Decode.open("huffman(Giainen).txt", ios::app);

	if (Decode.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}
	else
	{
		if (root == nullptr) 
		{
			return;
		}

		// là node lá
		if (!root->left && !root->right)
		{
			cout << root->ch;
			Decode << root->ch;
			return;
		}

		index++;

		if (str[index] == '0') // nếu 0 thì qua trái
			decode(root->left, index, str);
		else // nếu 1 thì qua phải
			decode(root->right, index, str);
	}
	Decode.close();

}

void WriteToFile(string FileName, unordered_map<char, string> huffmanCode, string s,int &pattern)
{
	ofstream fileOutput;
	fileOutput.open(FileName);

	if (fileOutput.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}

	// Ghi mã vào file
	string str = "";

	for (char ch : s) // cuyệt chuỗi s
	{
		str += huffmanCode[ch];
	}

	//fileOutput << "\nEncoded string is :\n" << str << '\n';
	cout << "\nEncoded string is :\n" << str << '\n';


	while ((int)str.length() >= 8) //tách 8 bit 
	{
		string byte = str.substr(0, 8);
		istringstream in(byte);
		bitset<8> bs;
		while (in >> bs)
			fileOutput << char(bs.to_ulong());
		str = str.substr(8, str.length() - 8); // xóa 8 bit vừa chuyển
	}
	
	pattern = 8 - str.length();
	while (str.size() < 8 && str.size() > 0)
	{
		str.insert(str.begin(), '0');
	}


		istringstream in(str);
		bitset<8> bs;
		while (in >> bs)
			fileOutput << char(bs.to_ulong());
	cout << "asv: " << (int)str.length() << endl;


	fileOutput.close();

}

void buildHuffmanTree(string fileName)
{
	
	ifstream fileInput;
	fileInput.open(fileName);

	string s;
	string file_contents;

	if (fileInput.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}
	while (getline(fileInput, s))
	{
		file_contents += s;
		file_contents.push_back('\n');
	}

	fileInput.close();

	// đếm tần suất mỗi ký tự
	unordered_map<char, int> freq;
	for (char ch : file_contents)
	{
		freq[ch]++;
	}

	// Tạo hàng đợi ưu tiên để lưu trực tiếp các node của cây Huffman
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Tạo node lá cho mỗi ký tự và thêm nó vào hàng đợi ưu tiên 
	for (auto pair : freq) 
	{
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// làm cho đến khi có nhiều hơn một nút trong hàng đợi
	while (pq.size() != 1)
	{
		// Xóa 2 nút có độ ưu tiên cao nhất (tần suất thấp nhất) từ hàng đợi
		Node *left = pq.top();
		pq.pop();

		Node *right = pq.top();	
		pq.pop();

		// Tạo một nút nội bộ mới với hai nút này như 1 nút con
		// và có tần số bằng tổng tần số của hai nút.
		// Thêm nút mới vào hàng đợi ưu tiên.

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root lưu trữ con trỏ đến root của Huffman Tree
	Node* root = pq.top();

	// duyệt cây Huffman và lưu trữ mã Huffman
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	cout << "Huffman Codes are :\n" << '\n';

	for (auto pair : huffmanCode) // xuất bảng mã hóa
	{
		cout << pair.first << " " << pair.second << '\n';
	}

	//cout << "\nOriginal string was :\n" << s << '\n';

	int pattern;
	WriteToFile("huffman.txt", huffmanCode, file_contents ,pattern); // ghi vào file


	ifstream fileOutput2;
	fileOutput2.open("huffman.txt");

	string s2;
	string file_contents_2;

	if (fileOutput2.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}
	while (getline(fileOutput2, s2))
	{
		file_contents_2 += s2;
		file_contents_2.push_back('\n');
	}
	

	cout << "thuc1: " << s2 << endl;
	fileOutput2.close();

	string str2 = "";
	
	char c;
	c = s2[s2.length()-1];

	s2.pop_back();


	for (char& _char : s2)
	{
		
		str2 += bitset<8>(_char).to_string(); // chuyển string về nhị phân
	}
	

	string str3;
	str3 = bitset<8>(c).to_string();
	str2.append(str3.substr(pattern , 8 - pattern));
	cout << "cc123:" <<str3 << endl;
	cout << "\nthuc2: \n" << str2 << endl;
	

	// Duyệt cây và giải mã
	int index = -1;
	cout << "\nDecoded string is: \n";
	while (index < (int)str2.size()- 2)
	{
		decode(root, index, str2);
	}
}
