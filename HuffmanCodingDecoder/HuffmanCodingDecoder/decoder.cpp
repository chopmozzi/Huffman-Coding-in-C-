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
	Node() //생성자
	{
		pLeft = NULL;
		pRight = NULL;
		ascii = 0;
	}
	//GET 함수
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
	//SET 함수
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
	HTree() //생성자
	{
		root = NULL;
	}
	~HTree() //소멸자
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
	//MakeTree function(Huffman table을 받아서)
	void MakeTree(map<char, string> huftable)
	{	//New Node
		Node* pNew = new Node; //dynamic allocation
		pNew->setAsc(0);
		pNew->setLeft(NULL);
		pNew->setRight(NULL);
		pNew->setCodw("");
		root = pNew; //root로 삽입
		//codeword에 따라 Left, Right로 이동하면서 노드를 생성함
		auto it = huftable.begin();
		for (int i = 0; i < huftable.size(); i++)
		{
			pNew = root;
			for (int j = 0; j < it->second.length(); j++)
			{	//0이면 왼쪽으로
				if (it->second[j] == '0')
				{	//없으면 생성
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
				else//1이면 오른쪽으로
				{	//없으면 생성
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
			}//다 이동하면 해당 노드에 ascii와 codeword를 지정해줌
			pNew->setAsc(it->first);
			pNew->setCodw(it->second);
			it++;
		}

	}

	void levelorder() //level order로 순회하면서 값이 잘 들어갔는지 확인
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
		//tree가 존재하지 않을 때
		if (root == NULL)
		{
			cout << "No data" << endl;
			return "";
		}
		//encoding이전의 문자열(decoding의 결과)
		string origin = "";

		Node* pCur = root;
		for (const char& c : code)
		{	//codeword에 따라 이동
			if (c == '0')
			{
				pCur = pCur->getLeft();
			}
			else
			{
				pCur = pCur->getRight();
			}
			//해당 ascii code를 찾았으면
			if (pCur->getLeft() == NULL && pCur->getRight() == NULL)
			{
				if (pCur->getAsc() == 127) //EOD를 만나면 종료
					break;
				origin += pCur->getAsc(); //해당 문자를 붙여줌
				pCur = root;
			}
		}
		return origin; //원본 문자열 반환

	}

};

char hufcode[100000000] = { 0 }; //문자열(huffman_code.hbs에 받은)
char convtable[10000] = { 0 };   //파일을 읽을
char decodetable[10000] = { 0 }; //huffman table 변환

int main(void)
{
	FILE* fp = fopen("huffman_table.hbs", "rb");
	FILE* fc = fopen("huffman_code.hbs", "rb");
	map<char, string> huftable;
	int i = 0;
	while (feof(fp) == 0)
	{	//huffman_table.hbs를 끝까지 읽음
		convtable[i] = fgetc(fp); //1byte 데이터를 읽어옴
		int dec = (int)convtable[i]; //10진수로
		bitset<8> bs(dec); //다시 이진수로
		string str = bs.to_string(); //string을 변환
		const char* binarystr = str.c_str();// 1,0으로 이루어진 값들이 들어가게 됨
		strcat(decodetable, binarystr); //문자열들을 이어 붙여줌
		i++;
	}
	i = 0;
	memset(convtable, 0, sizeof(convtable));
	while (feof(fc) == 0)
	{	//huffman_code.hbs를 끝까지 읽음
		char ch = 0;//1byte단위 ch
		ch = fgetc(fc);
		//huffman_table.hbs를 읽을 때와 똑같은 과정
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
	//decodetable의 값을 통해 huffman tree를 만듦
	while (1)
	{
		char sep[30] = { 0 };//분리해서 입력할 문자열
		strncpy(sep, decodetable + index, 8);//ascii code
		sep[8] = '\0'; //마지막에 NULL
		asci = strtol(sep, NULL, 2); //변환
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
	t.MakeTree(huftable); //huffman table을 통해 tree 생성
	t.levelorder(); //level order를 통해 tree 확인
	//output.txt 생성
	ofstream output("output.txt");
	output << t.decode(hcode);
	return 0;

}