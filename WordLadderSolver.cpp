// WordLadderSolver.cpp

#include "WordLadderSolver.h"
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

void removeNonLetters(string& s);

//hash_table

//this is a hash_table ,and the size is 100000.
const int TableSize = 100000;

//hash table node
struct Node
{
    int key;
    int value;
    int in_use;
    Node* multis;
};

//hashtable
struct hash_table
{
    int table_size;
    int size;
    Node* table;
};

//table head
hash_table* table_head = nullptr;

//table class
class MyList
{
public:
    //hash table
    hash_table* m;
    hash_table* CreateHashTable();
    int GetPosition(hash_table* m, int key);
    bool IsInUse(hash_table* m, int key);
    int KeyExists(hash_table* m, int key);
    void SetKey(int key, int val);
    void DisEntry(hash_table* m);
    void DeleteKey(int key);
    int SearchKey(int key);
    int GetTableSize();
    void ShowInfo(hash_table* m);
};

//create table
hash_table* MyList::CreateHashTable()
{
    m = new hash_table();
    m->table = new Node[TableSize];
    for(int i = 0; i < TableSize; i++)
    {
        m->table[i].key = 0;
        m->table[i].value = 0;
        m->table[i].in_use = 0;
        m->table[i].multis = 0;
    }
    m->table_size = TableSize;
    m->size = 0;
    
    return m;
}

//get key in the table's pos
int MyList::GetPosition(hash_table* m, int key)
{
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key << 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);
    key = (key >> 3) * 2654435761;
    key %= m->table_size;
    if(key < 0)
        key = -key;
    return key;
}

//judge if the key's place in used
bool MyList::IsInUse(hash_table* m, int key)
{
    int curr;
    curr = GetPosition(m, key);
    
    if(m->table[curr].multis == nullptr)
        return false;
    else
        return true;
}

//if this key exits
int MyList::KeyExists(hash_table* m, int key)
{
    int pos;
    int flag = 0;
    int record = 1;
    Node* e;
    pos = GetPosition(m, key);
    
    if(!IsInUse(m, key))
    {
        return 0;
    }
    else
    {
        if(m->table[pos].multis->key == key)
            return record;
        else
        {
            if((e = m->table[pos].multis->multis) != nullptr)
            {
                do
                {
                    ++record;
                    if(e->key == key)
                    {
                        flag = 1;
                        break;
                    }
                }while((e = e->multis) != nullptr);
            }
            else
                return 0;
            
            if(flag == 1)
                return record;
            else
                return 0;
        }
    }
}

//insert a key in table
void MyList::SetKey(int key, int val)
{
    hash_table* m;
    Node*e,*add;
    int pos;
    add = new Node();
    add->key = key;
    add->value = val;
    add->in_use = 1;
    add->multis = nullptr;
    m = table_head;
    pos = GetPosition(m, key);
    
    if(m->table != nullptr)
    {
        if(!KeyExists(table_head, key))
        {
            e = m->table[pos].multis;
            m->size++;
            if(!IsInUse(table_head, key))
            {
                m->table[pos].multis = add;
            }
            else
            {
                while(e->multis != nullptr)
                {
                    e = e->multis;
                }
                e->multis = add;
            }
        }
    }
}

//print
void MyList::DisEntry(hash_table* m)
{
    Node* e1,* e2;
    e1 = m->table;
    for(int i = 0; i < m->table_size; i++)
    {
        if((e1 + i)->multis != nullptr)
        {
            e2 = (e1 + i)->multis;
            do
            {
                cout << e2->key<<"-->" << e2->value << endl;
                e2 = e2->multis;
            }while(e2 != nullptr);
        }
    }
}

//delete a key
void MyList::DeleteKey(int key)
{
    int record;
    int pos;
    Node* e,* efree;
    hash_table*m;
    m = table_head;
    
    record = KeyExists(m, key);
    pos = GetPosition(m, key);
    if(record)
    {
        e = &(m->table[pos]);
        for(int i = 1; i <= record -1; i++)
        {
            e = e->multis;
        }
        efree = e->multis->multis;
        e->multis = nullptr;
        --(m->size);
        delete [] efree;
    }
}

//search the key's place in the table
int MyList::SearchKey(int key)
{
    int record;
    int pos;
    Node* e;
    hash_table* m;
    
    m = table_head;
    pos = GetPosition(m, key);
    record = KeyExists(m, key);
    if(!record)
    {
        return 0;
    }
    else
    {
        e = &(m->table[pos]);
        for(int i = 1; i <= record; i++)
        {
            e = e->multis;
        }
        return e->value;
    }
}

//get table size
int MyList::GetTableSize()
{
    return table_head->size;
}

//show table information
void MyList::ShowInfo(hash_table* m)
{
    DisEntry(m);
}

//////////////////////////////////////////////
//hash char[] to int
unsigned int myHash(char* str)
{
    unsigned int h;
    unsigned char* p;
    for(h = 0, p = (unsigned char*)str;* p ; p++)
        h = 31 * h + *p;
    return h;
}

//hash string to int
int myHashString(const string &str)
{
    unsigned int h=0;
    int i=0;
    int len = str.length();
    for(; i < len; i++)
    {
        h = 31*h + str[i];
    }
    return h;
}

string store[100000];
MyList mylist;

//the class to find the ladderlength and the path.
class Solution
{
public:
    queue<string> q1;//the words which take a change from last word.
    queue<int> q2;//path length
    int flag;
    
    //get ladderlength and store the path to store[100000].
    int ladderLength(string start, string end, MyList &dict)
    {
        //if flag==1 means find the end ,then break out.
        flag = 0;
        dict.SetKey(myHashString(end),1);
        q1.push(start);
        q2.push(1);
        while(!q1.empty())
        {
            string s = q1.front();
            int len = q2.front();
            q1.pop();
            q2.pop();
            //foundd it,return path lens.
            if(s == end)
                return len;
            //change one letter,and store new words in neighbors.
            //if new words equal to end,then return
            vector<string> neighbors = findNeighbors(s, dict,end);
            //finded and return
            if(flag == 1)
            {
                store[dict.GetPosition(dict.m,myHashString(end))] = s;
                return len+1;
            }
            //store ladder words and path length
            for(int i = 0; i < neighbors.size(); i++)
            {
                store[dict.GetPosition(dict.m,myHashString(neighbors[i]))] = s;
                q1.push(neighbors[i]);
                q2.push(len+1);
            }
        }
        return 0;
    }
    
    //change a letter of s,and store all the new words int ret,then return ret.
    //if new words equal to end,return ret immediately
    vector<string> findNeighbors(string s, MyList &dict,string end)
    {
        vector<string> ret;
        //delete a letter
        int i;
        for(i = 0; i < s.size(); i++)
        {
            int len = s.length();
            char c[2];
            c[1] = '\0';
            c[0]= s[i];
            string tmp1(c);
            s = s.substr(0,i) + s.substr(i+1,len);
            if(dict.SearchKey(myHashString(s)))
            {
                ret.push_back(s);
                dict.DeleteKey(myHashString(s));
            }
            if(s == end)
            {
                flag = 1;
                return ret;
            }
            s = s.substr(0,i) + tmp1 + s.substr(i,len-1);
        }
        
        //insert a letter
        for(i = 0; i <= s.size(); i++)
        {
            int len = s.length();
            int j = 0;
            char c[2];
            c[1] = '\0';
            c[0]= 'a'+j;
            string tmp1(c);
            s = s.substr(0,i) + tmp1 + s.substr(i,len);
            if(dict.SearchKey(myHashString(s)))
            {
                ret.push_back(s);
                dict.DeleteKey(myHashString(s));
            }
            if(s == end)
            {
                flag = 1;
                return ret;
            }
            for(j = 1; j < 26; j++)
            {
                c[0] = 'a'+j;
                s[i] = c[0];
                if(dict.SearchKey(myHashString(s)))
                {
                    ret.push_back(s);
                    dict.DeleteKey(myHashString(s));
                }
                if(s == end)
                {
                    flag = 1;
                    return ret;
                }
            }
            
            s = s.substr(0,i) + s.substr(i+1,len);
        }
        
        //replace a letter
        for(i = 0; i < s.size(); i++)
        {
            char c = s[i];
            for(int j = 0; j < 26; j++)
            {
                if(c == 'a'+j) continue;
                s[i] = 'a'+j;
                if(dict.SearchKey(myHashString(s)))
                {
                    ret.push_back(s);
                    dict.DeleteKey(myHashString(s));
                }
                if(s == end)
                {
                    flag = 1;
                    return ret;
                }
            }
            
            s[i] = c;
        }
        
        //swap a pair letters
        for(i = 0; i < s.size()-1; i++)
        {
            for(int j = i+1; j < s.size(); j++)
            {
                if(s[i] == s[j])
                    continue;
                char tmp = s[i];
                s[i] = s[j];
                s[j] = tmp;
                if(dict.SearchKey(myHashString(s)))
                {
                    ret.push_back(s);
                    dict.DeleteKey(myHashString(s));
                }
                if(s == end)
                {
                    flag = 1;
                    return ret;
                }
                tmp = s[i];
                s[i] = s[j];
                s[j] = tmp;
            }
        }
        return ret;
    }
};

class WordLadderSolverImpl
{
public:
    WordLadderSolverImpl() {}
    ~WordLadderSolverImpl() {}
    void createWordList(const vector<string>& words);
    int buildLadder(string start, string end, vector<string>& ladder);
private:
    // You probably want something more sophisticated/efficient than this:
    vector<string> m_words;
};

//temp listword
vector<string> wordtmp;
int flag = 0;
void WordLadderSolverImpl::createWordList(const vector<string>& words)
{
    //creat a hash table which to store the wordlist
    table_head = mylist.CreateHashTable();
    
    //put the vector words into hashtable
    //it's query speed has been greatly improved
    for(int i = 0; i < words.size(); i++)
    {
        mylist.SetKey(myHashString(words[i]),1);
        //stored in a tmp word vector in the first time
        if(flag == 0)
            wordtmp.push_back(words[i]);
    }
}

int WordLadderSolverImpl::buildLadder(string start, string end, vector<string>& ladder)
{
    //flag, to recreateWordList
    if(flag == 1)
    {
        delete [](mylist.m->table);
        delete mylist.m;
        for(int i = 0; i< 100000; i++)
            store[i] = "";
        createWordList(wordtmp);
    }
    flag = 1;
    ladder.clear();
    removeNonLetters(start);
    //start word not in wordlist
    if (!mylist.SearchKey(myHashString(start)))
        return 1;
    removeNonLetters(end);
    //end word not in wordlist
    if (!mylist.SearchKey(myHashString(end)))
        return 2;
    if (start == end)
        return 3;
    
    //my class to find ladder
    Solution s;
    
    //get the step from start to end
    int step = s.ladderLength(start, end, mylist);
    
    //no ladder
    if(step == 0)
        return -1;
    else
    {
        //in store array,stored words and it's pre words,
        //occording to this ,we can find the ladder.
        string tmp,pre = end;
        stack<string> out;
        //hash string to int,the int number is the word's store place.
        int pos = myHashString(pre);
        out.push(pre);
        do
        {	
            tmp = store[mylist.GetPosition(mylist.m,pos)];
            out.push(tmp);
            pre = tmp;
            pos = myHashString(pre);
        }while(tmp != start);
        
        int len = out.size();
        //push path to ladder.
        for(int i = 0; i < len; i++)
        {
            ladder.push_back(out.top());
            out.pop();
        }
        return 0;
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (int i = 0; i < s.size(); i++)
    {
        char ch = s[i];
        if (isalpha(ch))
        {
            *to = tolower(ch);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
} 

//******************** WordLadderSolver functions **********************************

// These functions simply delegate to WordLadderSolverImpl's functions.
// You probably don't want to change any of this code.

WordLadderSolver::WordLadderSolver()
{
    m_impl = new WordLadderSolverImpl;
}

WordLadderSolver::~WordLadderSolver()
{
    delete m_impl;
}

void WordLadderSolver::createWordList(const vector<string>& words)
{
    m_impl->createWordList(words);
}

int WordLadderSolver::buildLadder(string start, string end, vector<string>& ladder)
{
    return m_impl->buildLadder(start, end, ladder);
}
