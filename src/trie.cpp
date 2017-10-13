#include "trie.h"
vector<string>leaves;
vector<string>generated_item_set;
trie::trie()
{
	root=new node();
	for(int i=0;i<11;i++)
		root->children[i]=NULL;
	root->isLeaf=0;
	root->frequency=0;
}
void trie::__insert(string s)
{
	node *temp=root;
	// int count=item_set[s];
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='$')
		{
			if(temp->children[10]==NULL)
			{
				temp->children[10]=new node();
			}
			temp=temp->children[10];
		}
		else
		{
			if(temp->children[s[i]-'0']==NULL)
			{
				temp->children[s[i]-'0']=new node();	
			}
			temp=temp->children[s[i]-'0'];
		}
	}
	temp->isLeaf=1;
	// temp->frequency=count;
}
void trie::print(node *root,vector<char>v)
{
	if(root->isLeaf)
	{
		// cout<<"here\n";
		for(char c : v)
			cout<<c;
		cout<<endl;
	}
	for(int i=0;i<11;i++)
	{
		// cout<<i<<" "<<root->children[i]<<endl;
		if(root->children[i]!=NULL)
		{
			// cout<<i<<endl;
			if(i==10)
				v.pb('$');
			else
				v.pb(char('0'+i));
			print(root->children[i],v);
			v.pop_back();
		}
		
	}
}
void trie::get_subtree(node *root,vector<char>v)
{
	if(root->isLeaf)
	{
		string item="";
		for(char c : v)
			item+=c;
		leaves.pb(item);
	}
	for(int i=0;i<11;i++)
	{
		if(root->children[i]!=NULL)
		{
			if(i==10)
				v.pb('$');
			else
				v.pb(char('0'+i));
			get_subtree(root->children[i],v);
			v.pop_back();
		}
		
	}
}


void trie::dfs(node *root,int k,string path)
{
	if(k==0)
	{
		// get all strings below this subtree
		vector<char>v;
		get_subtree(root,v);
		// get all nc2 pair
		string pair="";
		vector<string>pair_content; 
		for(int i=0;i<leaves.size();i++)
		{
			for(int j=i+1;j<leaves.size();j++)
			{
				pair=leaves[i]+"$"+leaves[j];
				pair_content.pb(pair);
			}
		}

		// make k+1 item set
		for(int i=0;i<pair_content.size();i++)
		{
			pair=path+pair_content[i];
			generated_item_set.pb(pair);
		} 
		leaves.clear();
	}
	else
	{
		for(int i=0;i<11;i++)
		{
			if(root->children[i]!=NULL)
			{
				if(i==10)
				{
					path+="$";
					dfs(root->children[i],k-1,path);
					path.pop_back();
				}
				else
				{
					path.pb(char('0'+i));
					dfs(root->children[i],k,path);
					path.pop_back();
				}
			}
		}
	}
}