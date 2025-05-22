#include <bits/stdc++.h>
using namespace std;
#define nl '\n'
class Node
{
private:
    int freq;
    char c;
    Node *left;
    Node *right;
    int time;

public:
    Node(const int &f = 0, const char &ch = '~', Node *l = nullptr, Node *r = nullptr, const int &t = 0) : freq(f), c(ch), left(l), right(r), time(t) {}
    char getChar(void)
    {
        return c;
    }
    int getFreq(void)
    {
        return freq;
    }
    int getTime(void)
    {
        return time;
    }
    Node *getLeft(void)
    {
        return left;
    }
    Node *getRight(void)
    {
        return right;
    }
    void setLeft(Node *l)
    {
        left = l;
    }
    void setRight(Node *r)
    {
        right = r;
    }
    void setC(char _c)
    {
        c = _c;
    }
};

class Comparator
{
public:
    bool operator()(Node *n1, Node *n2)
    {
        if (n1->getFreq() != n2->getFreq())
            return (n1->getFreq()) > (n2->getFreq());
        if (n1->getChar() != n2->getChar())
            return (n1->getChar()) > (n2->getChar());
        return (n1->getTime()) > (n2->getTime());
    }
};

void print(Node *root)
{
    if (root->getLeft() == nullptr and root->getRight() == nullptr)
    {
        cout << (root->getChar());
        return;
    }
    cout << '(';
    print(root->getLeft());
    cout << ',';
    print(root->getRight());
    cout << ')';
}

class CodeBook
{
protected:
    vector<int> freqTable;

public:
    CodeBook(string s = "")
    {
        freqTable.assign(63, 0);
        for (char c : s)
        {
            if (c >= '0' and c <= '9')
                freqTable[c - '0']++;
            else if (c >= 'A' and c <= 'Z')
                freqTable[c - 'A' + 10]++;
            else if (c == '_')
                freqTable[36]++;
            else
                freqTable[62 + c - 'z']++;
        }
    }
    CodeBook(vector<int> &arr)
    {
        freqTable = arr;
    }
    void printBook()
    {
        for (int i = 0; i < 63; i++)
        {
            if (freqTable[i] != 0)
            {
                if (i < 10)
                    cout << i;
                else if (i <= 35)
                    cout << (char)('A' + i - 10);
                else if (i == 36)
                    cout << '_';
                else
                    cout << (char)('a' + i - 37);
                cout << " " << freqTable[i] << '\n';
            }
        }
    }
};
class HuffMann : public CodeBook
{
private:
    Node *root;

public:
    HuffMann() {}
    HuffMann(vector<int> &fre) : CodeBook(fre)
    {
        root = nullptr;
    }
    void constructTree(void)
    {
        int t = 0;
        priority_queue<Node *, vector<Node *>, Comparator> pq;
        for (int i = 0; i < 63; i++)
        {
            if (freqTable[i] == 0)
                continue;
            char c;
            if (i < 10)
                c = i + '0';
            else if (i <= 35)
                c = (char)('A' + i - 10);
            else if (i == 36)
                c = '_';
            else
                c = (char)('a' + i - 37);
            Node *n = new Node(freqTable[i], c, nullptr, nullptr, t);
            t++;
            pq.push(n);
        }
        while (!pq.empty())
        {
            Node *upar1 = pq.top();
            pq.pop();
            if (pq.empty())
            {
                root = upar1;
                break;
            }
            Node *upar2 = pq.top();
            pq.pop();
            Node *next = new Node(upar1->getFreq() + upar2->getFreq(), '~', upar1, upar2, t);
            t++;
            pq.push(next);
            if (pq.empty())
            {
                root = next;
                break;
            }
        }
    }
    void printTree()
    {
        print(root);
        cout << ';' << nl;
    }
    void buildTree(int n)
    {
        root = new Node();
        for (int i = 0; i < n; i++)
        {
            char C;
            cin >> C;
            string s;
            cin >> s;
            Node *cur = root;
            for (char c : s)
            {
                if (c == '0')
                {
                    if (cur->getLeft() == nullptr)
                        cur->setLeft(new Node);
                    cur = cur->getLeft();
                }
                else
                {
                    if (cur->getRight() == nullptr)
                        cur->setRight(new Node);
                    cur = cur->getRight();
                }
            }
            cur->setC(C);
        }
        printTree();
    }
    void buildTree(string s)
    {
        stack<Node *> nodes;
        stack<char> chars;
        for (char c : s)
        {
            if (c == '(')
                chars.push(c);
            else if (c == ')')
            {
                Node *r = nodes.top();
                nodes.pop();
                Node *l = nodes.top();
                nodes.pop();
                chars.pop();
                nodes.push(new Node(0, '~', l, r, 0));
                root = nodes.top();
            }
            else if (c != ',')
            {
                nodes.push(new Node(0, c, nullptr, nullptr, 0));
            }
        }
    }
    string decode(string msg)
    {
        Node *cur = root;
        string ans;
        for (char c : msg)
        {
            if (cur->getChar() != '~')
            {
                ans += cur->getChar();
                cur = root;
            }
            if (c == '0')
                cur = cur->getLeft();
            if (c == '1')
                cur = cur->getRight();
        }
        if (cur->getChar() != '~')
        {
            ans += cur->getChar();
            cur = root;
        }
        return ans;
    }
};
int main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string command;
    cin >> command;
    while (command != "Quit")
    {
        if (command == "BuildFrequencyTable")
        {
            string s;
            cin >> s;
            CodeBook c(s);
            c.printBook();
        }
        else if (command == "BuildTreeFromFrequencyTable")
        {
            int n;
            cin >> n;
            vector<int> freqTable(63);
            for (int i = 0; i < n; i++)
            {
                char c;
                cin >> c;
                int f;
                cin >> f;
                if (c >= '0' && c <= '9')
                    freqTable[c - '0'] = f;
                else if (c >= 'A' && c <= 'Z')
                    freqTable[c - 'A' + 10] = f;
                else if (c == '_')
                    freqTable[36] = f;
                else
                    freqTable[62 + c - 'z'] = f;
            }
            HuffMann hf(freqTable);
            hf.constructTree();
            hf.printTree();
        }
        else if (command == "DeriveHuffmanTree")
        {
            int n;
            cin >> n;
            HuffMann hf;
            hf.buildTree(n);
        }
        else if (command == "DecodeMessage")
        {
            string s;
            cin >> s;
            string msg;
            cin >> msg;
            HuffMann hf;
            hf.buildTree(s);
            cout << hf.decode(msg) << nl;
        }
        cin >> command;
    }
    return 0;
}