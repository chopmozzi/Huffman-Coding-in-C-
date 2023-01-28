#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
#include <stack>
#include <queue>
#pragma warning(disable:4996)
using namespace std;
//Huffman tree node
class Node {
private:
	Node* pLeft; //Left Node
	Node* pRight;//Right Node
	char ascii; //ascii code
	string codw; //code word
public:
	Node() //������
	{
		pLeft = NULL;
		pRight = NULL;
		ascii = 0;
	}
	//GET �Լ�
	Node* getLeft()
	{
		return pLeft;
	}
	Node* getRight()
	{
		return pRight;
	}
	char getAsc()
	{
		return ascii;
	}
	string getCodw()
	{
		return codw;
	}
	//SET �Լ�
	void setLeft(Node* node)
	{
		pLeft = node;
	}
	void setRight(Node* node)
	{
		pRight = node;
	}
	void setAsc(char ch)
	{
		ascii = ch;
	}
	void setCodw(string word)
	{
		codw = word;
	}
};

class HTree {
private:
	Node* root; //root node
public:
	HTree() //������
	{
		root = NULL;
	}
	~HTree() //�Ҹ���
	{
		stack<Node*> st;
		st.push(root);
		while (!st.empty())
		{
			Node* cur = st.top();
			st.pop();

			if (cur->getLeft())
				st.push(cur->getLeft());
			if (cur->getRight())
				st.push(cur->getRight());

			delete cur;
		}
		root = NULL;
	}
	//MakeTree function(Huffman table�� �޾Ƽ�)
	void MakeTree(map<char, string> huftable)
	{	//New Node
		Node* pNew = new Node; //dynamic allocation
		pNew->setAsc(0);
		pNew->setLeft(NULL);
		pNew->setRight(NULL);
		pNew->setCodw("");
		root = pNew; //root�� ����
		//codeword�� ���� Left, Right�� �̵��ϸ鼭 ��带 ������
		auto it = huftable.begin();
		for (int i = 0; i < huftable.size(); i++)
		{
			pNew = root;
			for (int j = 0; j < it->second.length(); j++)
			{	//0�̸� ��������
				if (it->second[j] == '0')
				{	//������ ����
					if (pNew->getLeft() == NULL)
					{
						Node* ppNew = new Node;
						ppNew->setAsc(0);
						ppNew->setLeft(NULL);
						ppNew->setRight(NULL);
						ppNew->setCodw("");
						pNew->setLeft(ppNew);
					}
					pNew = pNew->getLeft();
				}
				else//1�̸� ����������
				{	//������ ����
					if (pNew->getRight() == NULL)
					{
						Node* ppNew = new Node;
						ppNew->setAsc(0);
						ppNew->setLeft(NULL);
						ppNew->setRight(NULL);
						ppNew->setCodw("");
						pNew->setRight(ppNew);
					}
					pNew = pNew->getRight();
				}
			}//�� �̵��ϸ� �ش� ��忡 ascii�� codeword�� ��������
			pNew->setAsc(it->first);
			pNew->setCodw(it->second);
			it++;
		}

	}

	void levelorder() //level order�� ��ȸ�ϸ鼭 ���� �� ������ Ȯ��
	{
		queue<Node*> q;
		Node* item;
		q.push(root);
		while (!q.empty())
		{
			item = q.front();
			cout << item->getAsc() << "\t" << item->getCodw() << endl;
			if (item->getLeft() != NULL)
				q.push(item->getLeft());
			if (item->getRight() != NULL)
				q.push(item->getRight());
			q.pop();
		}
	}
	//decoding function
	string decode(string code) const {
		//tree�� �������� ���� ��
		if (root == NULL)
		{
			cout << "No data" << endl;
			return "";
		}
		//encoding������ ���ڿ�(decoding�� ���)
		string origin = "";

		Node* pCur = root;
		for (const char& c : code)
		{	//codeword�� ���� �̵�
			if (c == '0')
			{
				pCur = pCur->getLeft();
			}
			else
			{
				pCur = pCur->getRight();
			}
			//�ش� ascii code�� ã������
			if (pCur->getLeft() == NULL && pCur->getRight() == NULL)
			{
				if (pCur->getAsc() == 127) //EOD�� ������ ����
					break;
				origin += pCur->getAsc(); //�ش� ���ڸ� �ٿ���
				pCur = root;
			}
		}
		return origin; //���� ���ڿ� ��ȯ

	}

};

char hufcode[100000000] = { 0 }; //���ڿ�(huffman_code.hbs�� ����)
char convtable[10000] = { 0 };   //������ ����
char decodetable[10000] = { 0 }; //huffman table ��ȯ

int main(void)
{
	FILE* fp = fopen("huffman_table.hbs", "rb");
	FILE* fc = fopen("huffman_code.hbs", "rb");
	map<char, string> huftable;
	int i = 0;
	while (feof(fp) == 0)
	{	//huffman_table.hbs�� ������ ����
		convtable[i] = fgetc(fp); //1byte �����͸� �о��
		int dec = (int)convtable[i]; //10������
		bitset<8> bs(dec); //�ٽ� ��������
		string str = bs.to_string(); //string�� ��ȯ
		const char* binarystr = str.c_str();// 1,0���� �̷���� ������ ���� ��
		strcat(decodetable, binarystr); //���ڿ����� �̾� �ٿ���
		i++;
	}
	i = 0;
	memset(convtable, 0, sizeof(convtable));
	while (feof(fc) == 0)
	{	//huffman_code.hbs�� ������ ����
		char ch = 0;//1byte���� ch
		ch = fgetc(fc);
		//huffman_table.hbs�� ���� ���� �Ȱ��� ����
		int dec = (int)ch;
		bitset<8> bs(dec);
		string str = bs.to_string();
		const char* binarystr = str.c_str();
		strcat(hufcode, binarystr);
		i++;
	}
	string hcode(hufcode);
	int asci = 0; //ascii code
	int cods = 0; //codesize
	int codw = 0; //codeword
	int index = 0; //index number
	//decodetable�� ���� ���� huffman tree�� ����
	while (1)
	{
		char sep[30] = { 0 };//�и��ؼ� �Է��� ���ڿ�
		strncpy(sep, decodetable + index, 8);//ascii code
		sep[8] = '\0'; //�������� NULL
		asci = strtol(sep, NULL, 2); //��ȯ
		memset(sep, 0, sizeof(sep));
		index += 8;
		strncpy(sep, decodetable + index, 8); //codesize
		sep[8] = '\0';
		cods = strtol(sep, NULL, 2);
		memset(sep, 0, sizeof(sep));
		index += 8;
		int k = index;
		strncpy(sep, decodetable + k, cods);
		sep[cods] = '\0';
		index = index + cods;
		string codeword(sep); //codeword
		huftable[(char)asci] = codeword;
		if (asci == 0x7f)
			break;
	}
	HTree t;
	t.MakeTree(huftable); //huffman table�� ���� tree ����
	t.levelorder(); //level order�� ���� tree Ȯ��
	//output.txt ����
	ofstream output("output.txt");
	output << t.decode(hcode);
	return 0;

}