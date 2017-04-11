#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>

using namespace std;

struct corrAns
{
    string ans;
    int prob;
};

struct wrongAns
{
    string ans;
    int prob;
    bool flag;
};

struct question
{
	string ques;
	corrAns cAns;
	wrongAns wAns[3];
};

class Question
{
	question q;
	
	void audiencePoll()
    	{
		int sum = 0;
		q.cAns.prob = rand()%100 + 50;
		sum += q.cAns.prob;
		q.wAns[0].prob = rand()%(100 - sum);
		sum += q.wAns[0].prob;
		q.wAns[1].prob = rand()%(100 - sum);
		sum += q.wAns[1].prob;
		q.wAns[2].prob = rand()%(100 - sum);
    	}
    
  	void fiftyFifty()
    	{
		int rand1 = rand()%3;
		int rand2 = rand()%3;
		while(rand2==rand1)
		    rand2 = rand()%3;
		q.wAns[rand1].flag = q.wAns[rand2].flag = false;
	}

	public:
		Question()
		{
			audiencePoll();
			fiftyFifty();
		}
		
		void getdata(void)
		{
			cout<<"Enter question: "; getline(cin, q.ques);
			cout<<"Enter correct answer: "; getline(cin,q.cAns.ans);
			cout<<"Enter wrong answer(1): "; getline(cin,q.wAns[0].ans);
			cout<<"Enter wrong answer(2): "; getline(cin,q.wAns[1].ans);
			cout<<"Enter wrong answer(3): "; getline(cin,q.wAns[2].ans);
		}

		void putdata(void)
		{
			cout<< setw(10) << q.ques
			    << endl << q.cAns.ans
			    << "\t\t" << q.wAns[0].ans
			    << endl << q.wAns[1].ans
			    << "\t\t" << q.wAns[2].ans<<endl;
		}
};

int main()
{
	Question q;
	fstream inoutfile;
	inoutfile.open("ques1.txt", ios::ate | ios::out | ios::in);
	inoutfile.seekg(0,ios::beg);
	inoutfile.clear();

	cout<<"Add question"<<endl;
	q.getdata();
	char ch;
	cin.get(ch);
	
	inoutfile.write((char *) &q , sizeof q);
	inoutfile.seekg(0);

	cout<<"Updated Questions"<<endl;
	while(inoutfile.read((char *) &q, sizeof q))
	{
		q.putdata();
	}
	inoutfile.seekg(0,ios::end);
	int last = inoutfile.tellg();
	cout<<"last: "<<last<<endl;
	cout<<"sizeof(q): "<<sizeof(q)<<endl;
	int n = last/sizeof(q);
	cout<<"Number of objects = "<< n << endl;
}
