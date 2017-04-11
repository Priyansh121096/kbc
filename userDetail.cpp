#include<bits/stdc++.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

string SESSION_UNAME;

int main()
{
    char *uname;
    scanf("%s", uname);
    SESSION_UNAME = uname;
    
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
		res = stmt->executeQuery("SELECT userName FROM userData WHERE userName = " + SESSION_UNAME);
		if(res->rowsCount()==1)
		{
		    printf("Welcome back %s\n", uname);
		}
		else if(res->rowsCount()==0)
		{
		    res = stmt->executeQuery("INSERT INTO userData(userName) VALUES(" + SESSION_UNAME + ")");
		    printf("Welcome %s\n", uname);
		}
		else
		{
		    printf("Error\n");
		    exit(1);
		}

	    delete res;
	    delete stmt;
	    delete con;
		return 0;
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
