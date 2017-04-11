#include<bits/stdc++.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#define NDIFFICULTY 5
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

struct Question
{
	string ques;
	corrAns cAns;
	wrongAns wAns[3];
};

class PlayGame
{
	static const string *difficultyTable;
	public:
		void askQuestion(Question *Q, int difficulty)
		{
			try 
			{
				sql::Driver *driver;
				sql::Connection *con;
				sql::Statement *stmt;
				sql::ResultSet *res;

				driver = get_driver_instance();
				con = driver->connect("tcp://127.0.0.1:3306", "root", "John1234");
				con->setSchema("kbcdb");

				stmt = con->createStatement();
				res = stmt->executeQuery("SELECT COUNT(*) FROM " + difficultyTable[difficulty - 1]);
				int noOfQues = res->getInt("COUNT(*)");
				
				int quesNo = rand()%noOfQues;
				string str;          //The string
				ostringstream temp;  //temp as in temporary
				temp<<quesNo;
				str=temp.str(); 
				res = stmt->executeQuery("SELECT * FROM " + difficultyTable[difficulty - 1] + "WHERE quesId = " + str);
				
				Question q;
				q.ques = res->getString("ques");
				q.cAns.ans = res->getString("corrAns");
				q.wAns[0].ans = res->getString("wrongAns1");
				q.wAns[1].ans = res->getString("wrongAns2");
				q.wAns[2].ans = res->getString("wrongAns3");
				audiencePoll(q);
				fiftyFifty(q);

			    	delete res;
			    	delete stmt;
			    	delete con;
                		Q = &q;
				return;
			}
			catch (sql::SQLException &e)
			{
				cout << "# ERR: SQLException in " << __FILE__;
				cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
				cout << "# ERR: " << e.what();
				cout << " (MySQL error code: " << e.getErrorCode();
				cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			}
		}
};

static const string difficultyTableData[5] = {"quesLevel1","quesLevel2","quesLevel3","quesLevel4","quesLevel5"};
const string* PlayGame::difficultyTable = difficultyTableData;

int main()
{
    cout<<"Hello World";
}
