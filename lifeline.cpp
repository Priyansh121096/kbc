#include<bits/stdc++.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
using namespace std;
class Lifeline
{
    public:
    void swap(Question *Q, int difficulty)
    {
       PlayGame p;
       p.askQuestion(Q,difficulty);
    }
    
    void audiencePoll(Question *q)
    {
        int sum = 0;
        q->cAns.prob = rand()%100 + 50;
        sum += q->cAns.prob;
        q->wAns[0].prob = rand()%(100 - sum);
        sum += q->wAns[0].prob;
        q->wAns[1].prob = rand()%(100 - sum);
        sum += q->wAns[1].prob;
        q->wAns[2].prob = rand()%(100 - sum);
    }
    
    void fiftyFifty(Question *q)
    {
        int rand1 = rand()%3;
        int rand2 = rand()%3;
        while(rand2==rand1)
            rand2 = rand()%3;
        q->wAns[rand1].flag = q->wAns[rand2].flag = false;
    }
}
