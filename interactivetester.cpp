#include "WordLadderSolver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

const char* filename = "/Users/Teddy/Documents/CS32/Project 4/Project 4/wordlist.txt";
  // A Windows user might have the path be "C:/CS32/P4/wordlist.txt"
  // A Mac user might have it be "/Users/fred/CS32/P4/wordlist.txt"

int main()
{
	ifstream wordlistfile(filename);
	if (!wordlistfile)
	{
		cerr << "Cannot open " << filename << "!" << endl;
		return 1;
	}
	vector<string> words;
	string w;
	while (wordlistfile >> w)
		words.push_back(w);
	wordlistfile.close();
		
	WordLadderSolver wls;
	wls.createWordList(words);

	for (;;)
	{
		cout << "Enter start word (empty to quit): ";
		string start;
		getline(cin, start);
		if (start.empty())
			break;
		cout << "Enter end word: ";
		string end;
		getline(cin, end);

		vector<string> result;
		int status = wls.buildLadder(start, end, result);
		cout << "buildLadder returns " << status;
		if (result.empty())
			cout << ", result is empty" << endl;
		else
		{
			cout << ", result contains:" << endl;
			for (string w : result)
				cout << w << endl;
		}
	}
}
