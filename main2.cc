#include <set>
#include <iostream>
#include <string>
using namespace std;






// C++ program to print all
// possible strings of length k
#include <bits/stdc++.h>
using namespace std;


// The main recursive method
// to print all possible
// strings of length k
void printAllKLengthRec(vector<string> solutions, char set[], string prefix,
									int n, int k)
{

	// Base case: k is 0,
	// print prefix
	if (k == 0)
	{
    solutions.push_back(prefix);
		cout << (prefix) << endl;
		return;
	}

	// One by one add all characters
	// from set and recursively
	// call for k equals to k-1
	for (int i = 0; i < n; i++)
	{
		string newPrefix;

		// Next character of input added
		newPrefix = prefix + set[i];

		// k is decreased, because
		// we have added a new character
		printAllKLengthRec(solutions,set, newPrefix, n, k - 1);
	}

}

void printAllKLength(vector<string> solutions, char set[], int k,int n)
{
	printAllKLengthRec(solutions,set, "", n, k);
}

// Driver Code
int main()
{
  vector<string> solutions;
	int n,k;
  cin>>n,k;
	char set1[] = {'b', 's'};
	printAllKLength(solutions,set1, k, 2);
  for(int i = 0; i < solutions.size(); ++i){
    cout<<solutions<<endl;
  }
}
