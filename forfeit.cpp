#include<bits/stdc++.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
using namespace std;
extern string SESSION_UNAME;
class Forfeit
{
    public:       
        void game_forfeit(unsigned int curr_score)
        {
            Highscore hs;
            unsigned int high_score = hs.getHighScore(SESSION_UNAME);
            if(curr_score > high_score)
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
				    string str;          //The string
                    ostringstream temp;  //temp as in temporary
                    temp<<curr_score;
                    str=temp.str(); 
				    res = stmt->executeQuery("UPDATE userData SET highScore = " + str +"where userName = "+ SESSION_UNAME);
			
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
        }
};
