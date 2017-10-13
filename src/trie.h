//---------------laziness is poison-------------------
#include <bits/stdc++.h>
using namespace std;
const int N = 1234567;
const int mod = 1e9+7;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef priority_queue<pii, vector<pii>, greater<pii>> min_pq;
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define inf 1e18
extern vector<string>leaves;
extern vector<string>generated_item_set;
struct node
{	
	int isLeaf;	// 1 when node is a leaf node
	node* children[11];
	int frequency;
};
struct trie
{
		
	node *root;
	trie();
	void __insert(string s);
	void print(node *root,vector<char>v);
	void get_subtree(node *root,vector<char>v);
	void dfs(node *root,int k,string path);
};
