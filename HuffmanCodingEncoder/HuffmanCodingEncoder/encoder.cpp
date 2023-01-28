#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <vector>
#pragma warning(disable:4996)
using namespace std;

class Node
{
private:
	Node* pLeft; //Left Node
	Node* pRight;//Right Node
	uint32_t count; //문장안의 해당 캐릭터 수
	char ascii; //아스키 코드
public:
	Node() //노드 생성자
	{
		pLeft = NULL;
		pRight = NULL;
		count = 0;
		ascii = 0;
	}

	Node(char ch, int num) //생성자2
	{
		pLeft = NULL;
		pRight = NULL;
		count = num;
		ascii = ch;
	}
	//생성자3
	Node(Node* left, Node* right) : pLeft(left), pRight(right), count(pLeft->count + pRight->count), ascii(0) {}
	//GET 함수
	uint32_t getCount()
	{
		return count;
	}
	char getAsc()
	{
		return ascii;
	}
	Node* getLeft()
	{
		return pLeft;
	}
	Node* getRight()
	{
		return pRight;
	}
	//SET 함수
	void setAsc(char ch)
	{
		ascii = ch;
	}
	void setCount(uint32_t num)
	{
		count += num;
	}
	void setLeft(Node* node)
	{
		pLeft = node;
	}
	void setRight(Node* node)
	{
		pRight = node;
	}
	//트리를 위한 operator
	bool operator()(const Node* lhs, const Node* rhs) const {
		return lhs->count > rhs->count;
	}
};

//Huffman Tree
class HTree {
private:
	Node* root; //root 노드
	string code; //codeword
	void Huffcode(const string& input); //encoding한 문장을 가져오기 위한 function

public:
	map<char, pair<int, string>> table; //ascii, 빈도수, codeword 저장 table
	HTree() //생성자
	{
		root = NULL;
		code = "";
	}
	~HTree() //소멸자
	{
		stack<Node*> st;
		st.push(root);
		while (!st.empty())
		{
			Node* pCur = st.top();
			st.pop();

			if (pCur->getLeft())
				st.push(pCur->getLeft());
			if (pCur->getRight())
				st.push(pCur->getRight());

			delete pCur;
		}

		root = NULL;
	}
	//encode function(Make Huffman tree)
	void encode(const string& input)
	{
		map<char, int> hufd; //ascii, 빈도수 data
		for (auto& asc : input)
		{
			hufd[asc]++; //빈도수 카운트
		}
		hufd[127]++; //EOD는 127, EOD count
		for (auto& it : hufd)
		{
			table[it.first].first = it.second; //huffman table을 만들 때 사용하기 위해 삽입
			cout << it.first << "\t" << it.second << endl;
		}
		//Huffman tree를 만들 때 우선순위로 계산(빈도 수)
		priority_queue<Node*, vector<Node*>, Node> pq;
		//Huffman tree만들기
		for (auto& it : hufd)
		{	//priorty queue에 값들을 push
			pq.push(new Node(it.first, it.second));
		}
		//priorty queue를 바탕으로 Huffman Tree를 만듦
		while (pq.size() > 1)
		{
			Node* left = pq.top();
			pq.pop();
			Node* right = pq.top();
			pq.pop();
			//Node insert
			Node* pNew = new Node(left, right);

			pq.push(pNew);
		}
		root = pq.top();
		pq.pop();
		Huffcode(input); //입력 받은 문자열을 인코딩하기 위해

		return;
	}
	//encodeing한 문장을 가져옴
	string getCode() const {
		return code;
	}
	//Huffman table을 만드는 function
	void maketable(char* huftable)
	{
		char str[100] = { 0 };
		auto it = table.begin();
		for (int i = 0; i < table.size(); i++)
		{
			vector<int>bit(8, 0);
			vector<int>bit2(8, 0);
			memset(str, 0, sizeof(str));
			bitset<8> bs1((int)it->first); //이진수로 변환
			int codesize = it->second.second.length(); //codeword size 측정
			bitset<8> bs2(codesize);
			for (int i = 0; i < 8; i++)
			{
				bit[i] = bs1[i];
			}
			for (int i = 0; i < 8; i++)
			{
				bit2[i] = bs2[i];
			}
			//bitset을 하면 이진수의 순서가 바뀌므로
			reverse(bit.begin(), bit.end());
			reverse(bit2.begin(), bit2.end());
			for (int j = 0; j < (16 + it->second.second.length()); j++)
			{
				if (j == 0)
				{
					if (codesize % 8 != 0)
					{	//8bit를 A문자를 넣어 채워줌
						for (int k = 0; k < 8 * (codesize / 8 + 1) - codesize; k++)
						{
							it->second.second += "A";
						}
					}
				}
				if (j < 8) //아스키 코드 파트
					str[j] = bit[j] + 48;
				else if (j < 16) //코드 사이즈 파트
					str[j] = bit2[j - 8] + 48;
				else //codeword파트
					str[j] = it->second.second[j - 16];
			}
			//계속 이어 붙여줌
			strcat(huftable, str);
			//next table
			it++;
		}
		int num = strlen(huftable); // huftable의 길이
		int ncount = 0;
		//A를 없애주는 과정
		for (int i = 0; i < num; i++)
		{	//A를 뺀 문자열의 길이
			if (huftable[i] == 'A')
				ncount++;
		}
		for (int i = 0; i < num; i++)
		{	//A가 시작하는 위치에서 A가 끝나는 위치의 문자를 복사!
			if (huftable[i] == 'A')
			{
				int count = 0;
				for (int j = i; huftable[j] == 'A'; j++)
				{
					count++;
				}
				strcpy(huftable + i, huftable + i + count);
			}
		}
		num -= ncount;
		//마지막에 NULL문자를 취해서 나머지 값을 없앰
		huftable[num] = '\0';
	}
};
//문장을 인코딩
void HTree::Huffcode(const string& input)
{
	//codeword
	string codew = "";
	map<char, string> hufd; //ascii에 대한 codeword

	stack<pair<Node*, string>> st;
	st.push({ root,"" }); //초기화
	//빈도수에 맞춰진 트리를 들어가면서 1값과 0값을 넣음
	while (!st.empty())
	{
		Node* pCur = st.top().first;
		string temp = st.top().second;
		st.pop();

		if (pCur->getRight() != NULL)
		{
			st.push({ pCur->getRight(), temp + "1" }); //오른쪽으로는 1
		}
		if (pCur->getLeft() != NULL)
		{
			st.push({ pCur->getLeft(), temp + "0" }); //왼쪽으로는 0
		}
		if (pCur->getLeft() == NULL && pCur->getRight() == NULL)
		{
			hufd[pCur->getAsc()] = temp;
		}
	}

	for (const char& asci : input)
	{
		code += hufd[asci]; //code에 input 문장의 asci에 해당하는 codeword를 붙여줌
	}
	this->code = code;
	for (auto& it : hufd)
	{
		table[it.first].second = it.second;//table에 codeword삽입
		cout << it.first << "\t" << it.second << endl;
	}
	return;
}
char ch[100000] = { 0 }; //huffman table 값 저장
//파일에서 읽어올 문자열
char* input_data = new char[100000];
int main(void)
{
	//huffman의 값을 가지고 잇는 table -> 아스키코드, 빈도수(후에 codesize로 변환), string
	map<char, pair<int, string>> table;
	memset(input_data, 0, sizeof(input_data));
	ofstream fout("huffman_table.hbs", ios::binary);
	ofstream fcode("huffman_code.hbs", ios::binary);
	ifstream fin("input.txt");
	if (!fin.is_open())
	{
		cout << "File open error" << endl;
		return 0;
	}
	int len = 0;
	//파일을 읽어옴
	for (; !fin.eof(); len++)
	{
		fin.get(input_data[len]);
	}
	input_data[--len] = '\0';//마지막에 null
	fin.close();
	string str(input_data);//string 변환
	HTree h;
	h.encode(str);//받은 값을 encoding!
	string hfcode;//huffman code
	hfcode = h.getCode();

	for (int i = 0; i < h.table[127].second.length(); i++)
	{
		hfcode = hfcode + h.table[127].second[i];//마지막에 EOD를 붙여줌
	}
	h.maketable(ch); //Making Huffman table
	//Huffman_table.hbs write
	for (int i = 0; i < strlen(ch) / 8 + 1; i++)
	{
		char sep[9] = { 0 }; //8bit 단위 문자열
		strncpy(sep, ch + i * 8, 8);
		unsigned int data = strtol(sep, NULL, 2);
		if (i >= strlen(ch) / 8)
		{	//마지막이 8bit로 끝나지 않으면 8 bit를 채워주기 위해 shift
			int shift = 8 - strlen(sep);
			data = data << shift;
		}
		fout.write((char*)&data, sizeof(char));
	}
	//Huffman_code.hbs write
	for (int i = 0; i < hfcode.size() / 8 + 1; i++)
	{
		char sep[9] = { 0 };
		strncpy(sep, hfcode.c_str() + i * 8, 8);
		unsigned int data = strtol(sep, NULL, 2);
		if (i >= hfcode.size() / 8)
		{
			int shift = 8 - strlen(sep);
			data = data << shift;
		}
		fcode.write((char*)&data, sizeof(char));
	}
	fout.close();
	fcode.close();
	delete input_data;
	return 0;
}