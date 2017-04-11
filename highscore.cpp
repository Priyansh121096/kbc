#include<bits/stdc++.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class HighScore
{		
	public:
		unsigned int getHighScore(string uname)
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
				res = stmt->executeQuery("SELECT highScore from userData where userName=" + uname);
				if(res->rowsCount() == 1)
				{
					highScore = res->getInt("highScore");
				}
			
			delete res;
			delete stmt;
			delete con;

			}
			catch (sql::SQLException &e)
			{
				cout << "# ERR: SQLException in " << __FILE__;
				cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
				cout << "# ERR: " << e.what();
				cout << " (MySQL error code: " << e.getErrorCode();
				cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			}
			return highScore;
		
		}
		void updateHighScore(string uname, unsigned int hs)
		{
			highScore = hs;
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
                string str;          //The string
                ostringstream temp;  //temp as in temporary
                temp<<highScore;
                str=temp.str(); 
                
				res = stmt->executeQuery("UPDATE userData SET highScore=" + str + "where userName=" + uname);
			
			delete res;
			delete stmt;
			delete con;

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
