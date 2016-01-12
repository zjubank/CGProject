#include<iostream>
#include<fstream>
#include<string>

using namespace std;

bool isg=false;
int cnt = 0;
int main()
{
	ifstream fin("kotori_sbk_test.obj");
	ofstream fout("kotori_sbk_test_trans.obj");
	string s;
	while (getline(fin, s))
	{
		if (s[0] == 'u')
		{
			cnt++;
			if (isg == false)
			{
				fout << "g obj" << cnt<< endl;
			}
		}
		//isg = false;
		if (s[0] == 'g')
		{
			isg = true;
		}
		else isg = false;
		fout << s<< endl;
	}
	
	return 0;
}

