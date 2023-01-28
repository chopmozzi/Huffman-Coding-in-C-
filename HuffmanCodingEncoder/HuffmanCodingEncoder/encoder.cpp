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
	uint32_t count; //������� �ش� ĳ���� ��
	char ascii; //�ƽ�Ű �ڵ�
public:
	Node() //��� ������
	{
		pLeft = NULL;
		pRight = NULL;
		count = 0;
		ascii = 0;
	}

	Node(char ch, int num) //������2
	{
		pLeft = NULL;
		pRight = NULL;
		count = num;
		ascii = ch;
	}
	//������3
	Node(Node* left, Node* right) : pLeft(left), pRight(right), count(pLeft->count + pRight->count), ascii(0) {}
	//GET �Լ�
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
	//SET �Լ�
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
	//Ʈ���� ���� operator
	bool operator()(const Node* lhs, const Node* rhs) const {
		return lhs->count > rhs->count;
	}
};

//Huffman Tree
class HTree {
private:
	Node* root; //root ���
	string code; //codeword
	void Huffcode(const string& input); //encoding�� ������ �������� ���� function

public:
	map<char, pair<int, string>> table; //ascii, �󵵼�, codeword ���� table
	HTree() //������
	{
		root = NULL;
		code = "";
	}
	~HTree() //�Ҹ���
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
		map<char, int> hufd; //ascii, �󵵼� data
		for (auto& asc : input)
		{
			hufd[asc]++; //�󵵼� ī��Ʈ
		}
		hufd[127]++; //EOD�� 127, EOD count
		for (auto& it : hufd)
		{
			table[it.first].first = it.second; //huffman table�� ���� �� ����ϱ� ���� ����
			cout << it.first << "\t" << it.second << endl;
		}
		//Huffman tree�� ���� �� �켱������ ���(�� ��)
		priority_queue<Node*, vector<Node*>, Node> pq;
		//Huffman tree�����
		for (auto& it : hufd)
		{	//priorty queue�� ������ push
			pq.push(new Node(it.first, it.second));
		}
		//priorty queue�� �������� Huffman Tree�� ����
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
		Huffcode(input); //�Է� ���� ���ڿ��� ���ڵ��ϱ� ����

		return;
	}
	//encodeing�� ������ ������
	string getCode() const {
		return code;
	}
	//Huffman table�� ����� function
	void maketable(char* huftable)
	{
		char str[100] = { 0 };
		auto it = table.begin();
		for (int i = 0; i < table.size(); i++)
		{
			vector<int>bit(8, 0);
			vector<int>bit2(8, 0);
			memset(str, 0, sizeof(str));
			bitset<8> bs1((int)it->first); //�������� ��ȯ
			int codesize = it->second.second.length(); //codeword size ����
			bitset<8> bs2(codesize);
			for (int i = 0; i < 8; i++)
			{
				bit[i] = bs1[i];
			}
			for (int i = 0; i < 8; i++)
			{
				bit2[i] = bs2[i];
			}
			//bitset�� �ϸ� �������� ������ �ٲ�Ƿ�
			reverse(bit.begin(), bit.end());
			reverse(bit2.begin(), bit2.end());
			for (int j = 0; j < (16 + it->second.second.length()); j++)
			{
				if (j == 0)
				{
					if (codesize % 8 != 0)
					{	//8bit�� A���ڸ� �־� ä����
						for (int k = 0; k < 8 * (codesize / 8 + 1) - codesize; k++)
						{
							it->second.second += "A";
						}
					}
				}
				if (j < 8) //�ƽ�Ű �ڵ� ��Ʈ
					str[j] = bit[j] + 48;
				else if (j < 16) //�ڵ� ������ ��Ʈ
					str[j] = bit2[j - 8] + 48;
				else //codeword��Ʈ
					str[j] = it->second.second[j - 16];
			}
			//��� �̾� �ٿ���
			strcat(huftable, str);
			//next table
			it++;
		}
		int num = strlen(huftable); // huftable�� ����
		int ncount = 0;
		//A�� �����ִ� ����
		for (int i = 0; i < num; i++)
		{	//A�� �� ���ڿ��� ����
			if (huftable[i] == 'A')
				ncount++;
		}
		for (int i = 0; i < num; i++)
		{	//A�� �����ϴ� ��ġ���� A�� ������ ��ġ�� ���ڸ� ����!
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
		//�������� NULL���ڸ� ���ؼ� ������ ���� ����
		huftable[num] = '\0';
	}
};
//������ ���ڵ�
void HTree::Huffcode(const string& input)
{
	//codeword
	string codew = "";
	map<char, string> hufd; //ascii�� ���� codeword

	stack<pair<Node*, string>> st;
	st.push({ root,"" }); //�ʱ�ȭ
	//�󵵼��� ������ Ʈ���� ���鼭 1���� 0���� ����
	while (!st.empty())
	{
		Node* pCur = st.top().first;
		string temp = st.top().second;
		st.pop();

		if (pCur->getRight() != NULL)
		{
			st.push({ pCur->getRight(), temp + "1" }); //���������δ� 1
		}
		if (pCur->getLeft() != NULL)
		{
			st.push({ pCur->getLeft(), temp + "0" }); //�������δ� 0
		}
		if (pCur->getLeft() == NULL && pCur->getRight() == NULL)
		{
			hufd[pCur->getAsc()] = temp;
		}
	}

	for (const char& asci : input)
	{
		code += hufd[asci]; //code�� input ������ asci�� �ش��ϴ� codeword�� �ٿ���
	}
	this->code = code;
	for (auto& it : hufd)
	{
		table[it.first].second = it.second;//table�� codeword����
		cout << it.first << "\t" << it.second << endl;
	}
	return;
}
char ch[100000] = { 0 }; //huffman table �� ����
//���Ͽ��� �о�� ���ڿ�
char* input_data = new char[100000];
int main(void)
{
	//huffman�� ���� ������ �մ� table -> �ƽ�Ű�ڵ�, �󵵼�(�Ŀ� codesize�� ��ȯ), string
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
	//������ �о��
	for (; !fin.eof(); len++)
	{
		fin.get(input_data[len]);
	}
	input_data[--len] = '\0';//�������� null
	fin.close();
	string str(input_data);//string ��ȯ
	HTree h;
	h.encode(str);//���� ���� encoding!
	string hfcode;//huffman code
	hfcode = h.getCode();

	for (int i = 0; i < h.table[127].second.length(); i++)
	{
		hfcode = hfcode + h.table[127].second[i];//�������� EOD�� �ٿ���
	}
	h.maketable(ch); //Making Huffman table
	//Huffman_table.hbs write
	for (int i = 0; i < strlen(ch) / 8 + 1; i++)
	{
		char sep[9] = { 0 }; //8bit ���� ���ڿ�
		strncpy(sep, ch + i * 8, 8);
		unsigned int data = strtol(sep, NULL, 2);
		if (i >= strlen(ch) / 8)
		{	//�������� 8bit�� ������ ������ 8 bit�� ä���ֱ� ���� shift
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