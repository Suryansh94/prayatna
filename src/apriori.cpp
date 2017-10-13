//---------------laziness is poison-------------------
#include "trie.h"
string input_file,output_file;
float support,confidence;
int flag,min_support_value,total_unique_item=0;
vector<set<string>>transaction;
map<string,int>item_set;
map<string,string>original_string;
map<string,string>get_original_string;
set<pair<string,string>>association_rules;
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string>v;
    while (getline(ss, item, delim)) {
        v.pb(item);
    }
    return v;
}

void read_input(string file_name)
{
	ifstream input_file(file_name);
	string line;
	int total_transaction=0;
	int item_id=1;
	stringstream ss;
	string encoded_item_id;
	ss<<item_id;
	encoded_item_id=ss.str();
	while (getline(input_file, line))
	{	
		vector<string> result = split(line,',');
		set<string>items;
		for(string val : result)
		{
			if(original_string.find(val)==original_string.end())
			{
				total_unique_item++;
				ss.str(std::string());
				original_string[val]=encoded_item_id;
				get_original_string[encoded_item_id]=val;
				item_id++;
				ss<<item_id;
				encoded_item_id=ss.str();
			}
			items.insert(original_string[val]);
		}	
		transaction.pb(items);
		total_transaction++;
	}
	// cout<<total_unique_item<<endl;
	min_support_value=ceil(support*total_transaction);
}


void print(map<string,int>&m)
{
	cout<<m.size()<<endl;
	for(auto val : m)
	{
		vector<string>item =split(val.fi,'$');
		for(int i=0;i<item.size();i++)
			cout<<get_original_string[item[i]]<<(i==item.size()-1?"":",");
		cout<<endl;
	}
}
void generate_frequent_itemset(int size)
{
	if(size==1)
	{
		for(auto item : transaction)
		{
			for(auto val : item)
			{
				item_set[val]++;
			}
		}
	}
	else if(size==2)
	{
		vector<string>items,paired_item;
		for(auto val : item_set)
			items.pb(val.fi);
		for(int i=0;i<items.size();i++)
		{
			for(int j=i+1;j<items.size();j++)
			{
				string two = items[i]+"$"+items[j];
				paired_item.pb(two);
			}
		}
		for(int i=0;i<paired_item.size();i++)
		{
			vector<string>pair_item = split(paired_item[i],'$');
			for(int j=0;j<transaction.size();j++)
			{
				if(transaction[j].find(pair_item[0])!=transaction[j].end() and  transaction[j].find(pair_item[1])!=transaction[j].end())
					item_set[paired_item[i]]++;
			}
		}
	}
}


void prune(map<string,int>&item_set)
{
	// mistake with auto and size decrement simultaneouly
	map<string,int>::iterator it;
	for(auto item : item_set)
	{
		if(item.se<min_support_value)
		{
			map<string,int>::iterator it;
			it=item_set.find(item.fi);
			item_set.erase (it); 
		}
	}
}

vector<vector<string>> get_subset(vector<string> &v)
{

	int size = v.size();
	int len = (1<<size)-1;
	vector<vector<string>>res;
	for(int i=1;i<len;i++)
	{
		int j=0,temp=i;
		vector<string>items;	
		while(j<size)
		{
			if(temp&1)
			{
				items.pb(v[j]);
			}
			temp>>=1;
			j++;
		}
		res.pb(items);
	}
	return res;
}

void generate_rules(string s)
{
	vector<string> items  = split(s,'$');
	vector<vector<string>> ans = get_subset(items);
	for(int i=0;i<ans.size();i++)
	{
		for(int j=0;j<ans.size();j++)
		{	
			if(i==j)
				continue;
			int flag=1;
			for(int k=0;k<ans[i].size();k++)
			{
				for(int l=0;l<ans[j].size();l++)
				{
					if(ans[i][k]==ans[j][l])
					{
						flag=0;
						break;
					}
				}
				if(flag==0)
					break;
			}
			if(flag)
			{
				vector<string> left,right,actual_right;
				for(int k=0;k<ans[i].size();k++)
				{
					left.pb(ans[i][k]);
					right.pb(ans[i][k]);
				}
				for(int k=0;k<ans[j].size();k++)
				{
					actual_right.pb(ans[j][k]);
					right.pb(ans[j][k]);
				}
				sort(all(actual_right));
				sort(all(left));
				sort(all(right));
				string left_value="",store_right_value="",store_left_value="";
				for(int i=0;i<left.size();i++)
				{
					left_value+=left[i];
					store_left_value+=get_original_string[left[i]];
					store_left_value+=",";
					if(i!=left.size()-1)left_value+="$";
				}
				store_left_value.pop_back();
				string right_value="";
				for(int i=0;i<right.size();i++)
				{
					right_value+=right[i];
					if(i!=right.size()-1)right_value+="$";
				}
				for(int i=0;i<actual_right.size();i++)
				{
					store_right_value+=get_original_string[actual_right[i]];
					store_right_value+=",";
				}
				store_right_value.pop_back();
				double val = 1.0*item_set[right_value]/(item_set[left_value]);
				// cout<<val<<endl;
				if(val>=confidence)
					association_rules.insert({store_left_value,store_right_value});
			}

		}
	}
	return ;
}
int main()
{
	ios_base:: sync_with_stdio(false); cin.tie(0);
	// freopen ("input.csv","r",stdin);
	// freopen ("output.csv","w",stdout);
	trie tree;
	ifstream config_file("config.csv");
	string file_content;
	while (getline(config_file, file_content))
	{
		vector<string> s = split(file_content,',');
		if(s[0]=="input")
			input_file=s[1];
		if(s[0]=="output")
			output_file=s[1];
		if(s[0]=="support")
		{
			stringstream ss(s[1]);
			ss>>support;
		}
		if(s[0]=="confidence")
		{
			stringstream ss(s[1]);
			ss>>confidence;
		}
		if(s[0]=="flag")
		{
			stringstream ss(s[1]);
			ss>>flag;	
		}
	}
	freopen (output_file.c_str(),"w",stdout);
	read_input(input_file);
	int size=1;
	generate_frequent_itemset(size);

	// got item_set :: map<string,int> item_set
	prune(item_set);
	// got the single candidate item set which is frequent  present in item_set
	size++;
	generate_frequent_itemset(size);
	prune(item_set);
	// inserting of one and two frequen set item in trie
	for(auto item : item_set)
	{
		tree.__insert(item.fi);
	}

	// now generation of more than 2 candidate set starts
	int depth=1;
	while(1)
	{
		tree.dfs(tree.root,depth,"");
		depth++;
		// generated_item_set is vector<string> containing k th item set

		// prune the generated item set
		int previous_item_set_size = item_set.size();
		vector<string>entries_to_insert;
		for(int i=0;i<generated_item_set.size();i++)
		{
			vector<string>pair_item = split(generated_item_set[i],'$');
			
			for(int j=0;j<transaction.size();j++)
			{
				int flag=0;
				for(string value : pair_item)
				{
					if(transaction[j].find(value)==transaction[j].end())
					{
						flag=1;
						break;
					}
				}
				if(!flag)
				{
					item_set[generated_item_set[i]]++;
				}
			}
		}
		prune(item_set);
		for(int i=0;i<generated_item_set.size();i++)
		{
			if(item_set.find(generated_item_set[i])!=item_set.end())
				entries_to_insert.pb(generated_item_set[i]);
		}
		if(previous_item_set_size==item_set.size())
		{
			// no change in size 
			break;
		}
		// clear generated item set
		generated_item_set.clear();

		// insert new entres to tree
		for(int i=0;i<entries_to_insert.size();i++)
			tree.__insert(entries_to_insert[i]);
	}
	// cout<<"print item set\n";
	print(item_set);
	if(flag)
	{
		for(auto item : item_set)
		{
			if(item.fi.size()==1)
			{
				continue;
			}

			//  generate rules
			generate_rules(item.fi);
		}
		cout<<association_rules.size()<<endl;
		for(auto rule : association_rules)
		{
			cout<<rule.fi<<",=>>,"<<rule.se<<endl;
		}
	}

	return 0;
}