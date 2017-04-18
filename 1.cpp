#include<bits/stdc++.h>
#include <GL/glut.h>
#include<time.h>
#include <IL/il.h>
#include <unistd.h>
#include <pthread.h>
#include<iostream>
#include<string>
#include<string.h>
#include<stdlib.h>
using namespace std;

volatile int stopsound = 0;
volatile int game_menu;
int new_game = 0,load_game = 0,high_score = 0,already_exists = 0, does_not_exist = 0,return_from_nglg = 0,return_from_hs = 0,return_from_qa = 0,option_selected = 1000,help=0,time_ans;
int qlevel = 0,active_option = 0;
int menuw;
GLuint tex[5];
char name[32];
double px[]={-480,-480,-480,-480,-480}, py[]={200,100,0,-100,-200},transp[]={1,0.2,0.2,0.2,0.2};
int minx[5]={0},maxx[5]={0},miny[5]={0},maxy[5]={0};
int firsttime2 = 1;
int fq=0,fq2=0;
int order_of_ques[5] = {0};
int k[4]={0};
int c2x[] = {-400,250,-400,250,-270};
int c2y[] = {-230,-230,-380,-380,-70};
int minx2[] = {-480,200,-480,200,-360};
int miny2[] = {-300,-300,-450,-450,-150};
int maxx2[] = {-200,480,-200,480,360};
int maxy2[] = {-200,-200,-350,-350,0};
int correct = 1;
int score = 0;
int score_array[] = {0,1000,10000,100000,1000000,10000000};
int nq;
int adt;
float dcol[] = {0.282, 0.239, 0.545,1};
float ccol[] = {0.486, 0.988, 0.000,1};
float wcol[] = {0.863, 0.078, 0.235,1};
float col[][4] = {{dcol[0],dcol[1],dcol[2],dcol[3]},{dcol[0],dcol[1],dcol[2],dcol[3]},{dcol[0],dcol[1],dcol[2],dcol[3]},{dcol[0],dcol[1],dcol[2],dcol[3]}};
bool answered = false,ffa=true,apa=true,apu=false;

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
Question Q;

class user
{
	public:           
		string name;
		int level;
		int high_score;

		user()
		{
			name = "";
			level=0;
			high_score=0; 
		}

		bool newuser(string nme)
		{
			
			name = nme;
			ifstream infile1;
			infile1.open("Entry.txt");
			int a,b;
			string n;
			while(!infile1.eof())
			{
				//cout<<"here"<<endl;
				infile1>>n;
				if(infile1.eof()){  break; }
				infile1>>a;
				infile1>>b;
				if(n==name){ return false; }
			}
			infile1.close();

			ofstream infile("Entry.txt",ios::app);  
			infile<<name<<endl;
			infile<<level<<endl;
			infile<<high_score<<endl;
			infile.close();
			return true;   
		} 

		bool loaduser(string nme)
		{	
			//cout<<"here2"<<endl;
			//cout<<nme;
			ifstream infile("Entry.txt");
			int a,b;
			string n;
			while(!infile.eof())
			{
				//cout<<"here3"<<endl;
				infile>>n;
				//cout<<n<<endl;
				if(infile.eof()){  break; }
				infile>>a;
				infile>>b;
				if((n==nme))
				{
					//std::cout<<"Found\n";
					name = nme;
					level=a;
					high_score=b;
					infile.close();
					return true;
				}
			}    
			infile.close();
			return false;            
		} 
		
		bool highscore(string nme)
		{
			ifstream infile("Entry.txt");
			int a,b;
			string n;
			while(!infile.eof())
			{
				infile>>n;
				if(infile.eof()){  break; }
				infile>>a;
				infile>>b;
				if((n==nme))
				{
					//std::cout<<"Found\n";
					name = nme;
					level=a;
					high_score=b;
					infile.close();
					return true;
				}
			}    
			infile.close();
			return false;            
		}
		
		bool update()
		{
			fstream infile;
			infile.open("Entry.txt");
			int a,b;
			string n;

			while(!infile.eof())
			{ 
				infile>>n;
				if((n==name))
				{
					int pos = infile.tellp();
					infile>>a;
					infile>>b;	
					char buff[2000]={'\0'};        
					int count=0;
					while(!infile.eof())
					{
						infile.get(buff[count]);
						if(infile.eof()) 
							break;
						count++;
					}
					infile.clear();
					infile.seekp(pos,ios::beg);        

					infile<<'\n'<<level<<endl;
					infile<<high_score;

					for(int i=0;i<count;i++)
						infile.put(buff[i]);
	
					infile.close();
					return true;
				}
			}    
			infile.close();
			return false;        
		}


		void display()
		{
			std::cout<<"Name : "<<name<<endl;
			std::cout<<"Level : "<<level<<endl;
			std::cout<<"High Score : "<<high_score<<endl;
		}
};

user obj;

void init(void);
int load_texture(char * img,int i);
int LoadImage(char *filename);
void key_func(unsigned char key,int x,int y);
void SpecialKey(int key, int x, int y);
void show_menu();
void show_background(int i);
void draw_option(int i);
void draw_font(string s,double x,double y);
void display(void);
void idle(void);
void drawButton1(const char *text, int length ,int x ,int y );
void idle2(void);
void idle_about(void);
void chooseQuestion(Question *q);
void idle3(Question *Q, int opt);
//void idle4(Question *Q, int opt);
void mouseline12(int button, int state,int x1, int y1);
void mouseline11(int button, int state,int x1, int y1);
void generateSetOfNumbers(int arr[], int n,int k);
void audiencePoll(Question *q);
void fiftyFifty(Question *q);
void already(int x);
void does(int x);
void drawButton(const char *text, int length ,int x ,int y );
void drawButton1(const char *text, int length ,int x ,int y );
void drawButton2(const char *text, int length ,int x ,int y );
void drawText(const char *text, int length ,int x ,int y );
void createSubWindow(string msg);
void timer(int );
void idlefunc()
{
	idle3(&Q,option_selected);
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize (1362, 750);
    glutInitWindowPosition (0, 0);
    menuw = glutCreateWindow ("User Details");
    game_menu = 1;
    glutSetWindow(menuw);

    glEnable(GL_TEXTURE_2D);
    ilInit();
    init ();
    glutKeyboardFunc(key_func);
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(display);
    glutMouseFunc(mouseline12);
    glutTimerFunc(0,timer,0);
    //glutIdleFunc(idlefunc);
    glutMainLoop();

    stopsound=1;
    return 0;
}

void init(void)
{       
	glClear(GL_COLOR_BUFFER_BIT);
	int tx =load_texture("kbc1.jpg", 0);
	tx = load_texture("kbc2.jpg",1);
	tx = load_texture("kbc3.jpg",2);

	glEnable(GL_TEXTURE_2D);
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glOrtho (-500,500 ,-500,500,0,100);
	glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window

}

int firsttime = 1;
int load_texture(char * img,int i)
{
	GLuint    image;
	GLuint texid;
	image = LoadImage(img);
	glGenTextures(1, &tex[i]); /* Texture name generation */
	glBindTexture(GL_TEXTURE_2D,tex[i]); /* Binding of texture name */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */
	ilDeleteImages(1, &image);
	firsttime=0;
	//cout<<tex[i]<<"\n";
	return tex[i];
}

int LoadImage(char *filename)
{
	ILuint    image;
	ILboolean success;
	ilGenImages(1, &image);    /* Generation of one image name */
	ilBindImage(image);
	ilLoadImage(filename); /* Binding of image name */
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	return image;
}

void key_func(unsigned char key,int x,int y)
{
	if(key==13) //Enter Key
	{
		if(active_option==0 && game_menu)
		{
			game_menu=0;
			new_game=1;
			std::cout<<"Enter new user name\n";
		}	

		else if(active_option==1 && game_menu)
		{
			std::cout<<"Enter existing user name\n";
			game_menu=0;
			load_game=1;
		}
		
		else if(active_option==2 && game_menu)
		{
			std::cout<<"Enter existing user name\n";
			game_menu=0;
			high_score=1;
		}

		else if(active_option==3 && game_menu)
		{
			std::cout<<"Bye Bye. Visit Again"<<endl;
			exit(1);
		}
        else if (active_option==4 && game_menu){
            game_menu=0;
            help=1;
        }
		else if(game_menu==0 && new_game==1)
		{
			if(strlen(name)==0)
				return;

			if(obj.newuser(name))
			{
				new_game=0;
				return_from_nglg = 1;
			}
			else
			{
				already_exists=1;
				return;
			}

		}

		else if(game_menu==0 && load_game==1)
		{
			if(strlen(name)==0)
			{
				//cout<<"here"<<endl;
				return;
			}

			if(obj.loaduser(name))
			{
				//cout<<"here"<<endl;
				load_game=0;
				return_from_nglg = 1;
			}
			else
			{
				does_not_exist=1;
				return;
			}
		}
		
		else if(game_menu==0 && high_score==1)
		{
			if(strlen(name)==0)
				return;

			if(obj.highscore(name))
			{
				high_score=0;
				return_from_hs = 1;
			}
			else
			{
				does_not_exist=1;
				return;
			}
		}
        	glutPostRedisplay();
	}

	else if(((key>=65 && key<=90) || (key>=65 && key<=122) || key==32 ) && (new_game || load_game || high_score))
	{
		name[strlen(name)]=key;
		glutPostRedisplay();
	}

	else if(key==8 && (new_game || load_game || high_score))
	{
		if(strlen(name)>0)
			name[strlen(name)-1]='\0';
		glutPostRedisplay();
	}
}

void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			if(game_menu)
			{
				transp[active_option]=0.2;
				active_option-=1;
				active_option+=5;
				active_option%=5;
				transp[active_option]=1;
				show_menu();
			}
			break;	
		case GLUT_KEY_DOWN:
			transp[active_option]=0.2;
			active_option+=1;
			active_option%=5;
			transp[active_option]=1;
			show_menu();
			break;

	}
	glutPostRedisplay();
}

void show_menu()
{
   
	show_background(0);
	draw_option(0);
	glColor3f(0.8,0.2,0.2);
	draw_font("New Game",px[0]+125,py[0]-45);
	draw_option(1);
	glColor3f(0.8,0.2,0.2);
	draw_font("Load Game",px[1]+122,py[1]-45);
	draw_option(2);
	glColor3f(0.8,0.2,0.2);
	draw_font("High Score",px[2]+120,py[2]-45);
	draw_option(3);
	glColor3f(0.8,0.2,0.2);
	draw_font("Exit Game",px[3]+120,py[3]-45);
	draw_option(4);
	glColor3f(0.8,0.2,0.2);
	draw_font("About",px[4]+140,py[4]-45);
}

void show_ques(Question *q)
{
	show_background(1);
	draw_option(0);
	glColor3f(0.8,0.2,0.2);
	draw_font(q->ques,px[0]+125,py[0]-45);
	draw_option(1);
	glColor3f(0.8,0.2,0.2);
	draw_font(q->cAns.ans,px[1]+122,py[1]-45);
	draw_option(2);
	glColor3f(0.8,0.2,0.2);
	draw_font(q->wAns[0].ans,px[2]+120,py[2]-45);
	draw_option(3);
	glColor3f(0.8,0.2,0.2);
	draw_font(q->wAns[1].ans,px[3]+120,py[3]-45);
	draw_option(4);
	glColor3f(0.8,0.2,0.2);
	draw_font(q->wAns[2].ans,px[4]+140,py[4]-45);
}

void show_background(int i)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex[i]);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(-500, 500);
		glTexCoord2i(0, 1); glVertex2i(-500, -500);
		glTexCoord2i(1, 1); glVertex2i(500, -500);
		glTexCoord2i(1, 0); glVertex2i(500, 500);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void draw_option(int i)
{
	glBegin(GL_POLYGON);
		glColor4f(0.1,0.5,0.65,transp[i]);
		glVertex2f(px[i],py[i]);
		glVertex2f(px[i]+330,py[i]);
		glVertex2f(px[i]+330,py[i]-70);
		glVertex2f(px[i],py[i]-70);
	glEnd();
	glFlush();
}

void draw_font(string s,double x,double y)
{
	glRasterPos2f(x, y);
    	for (int i = 0; i < s.size(); i++)
  		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
}

void display(void)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.4,0.5,0.1);
	cout<<"Ans: "<< answered<<endl;
	if (game_menu)
	{
		//cout<<"Yaha hoon7"<<endl;
		show_menu();
	}

	else if(new_game)
	{
		//cout<<"Yaha hoon3"<<endl;
		show_background(2);
		idle();
		if(already_exists)
			already(0);
		else
		{
			return_from_nglg = 1;
			adt=(float)glutGet(GLUT_ELAPSED_TIME)/1000;
		}
	}
	else if(help)
	{
		//cout<<"Yaha hoon3"<<endl;
		show_background(1);
		idle_about();
	}
	else if(load_game)
	{
		//cout<<"Yaha hoon4"<<endl;   
		show_background(2);
		idle();
		if(does_not_exist)
			does(0);
		else
		{
			return_from_nglg = 1;
			adt=(float)glutGet(GLUT_ELAPSED_TIME)/1000;
		}
	}
	
	else if(high_score)
	{
		//cout<<"Yaha hoon5"<<endl;   
		show_background(2);
		idle();
		if(does_not_exist)
			does(0);
		return_from_hs=0;
	}

	else if(return_from_hs)
	{
		//cout<<"Yaha hoon6"<<endl;
		show_background(1);
		idle2();
	}

	else if(answered)
	{
		show_background(1);
		//cout<<"Yaha hoon"<<endl;
		if(option_selected==3)
		{
			for(int i=0;i<4;i++)
				col[k[3]][i] = ccol[i];
		}
		else
		{
			for(int i=0;i<4;i++)
				col[k[3]][i] = ccol[i];
			for(int i=0;i<4;i++)
				col[k[option_selected]][i] = wcol[i];
		}
		idle3(&Q,option_selected);
		sleep(1);
		answered = false;
		//return_from_qa = 0;
	}

	else //if(return_from_qa)
	{
		if(firsttime2)
		{
			for(int i=0;i<3;i++)
				for(int j=0;j<4;j++)
					col[i][j] = dcol[j];
			chooseQuestion(&Q);
			generateSetOfNumbers(k,4,4);
			firsttime2 = 0;
			adt=(float)glutGet(GLUT_ELAPSED_TIME)/1000;
		}
		show_background(1);
		idle3(&Q, option_selected);
		//cout<<"Yaha hoon2"<<endl;
	}
	glFlush();
	glutSwapBuffers();
}
void idle_about(void)
{
	cout<<"idle_about"<<endl;
	char string[20] = "KBC GAME";
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-100, 300);
	for (int i = 0; string[i] != '\0'; i++)
		glutBitmapCharacter(font_style, string[i]);
		
	char string1[30]="This is a simple Quiz Game ";
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-150, 200);
	    for (int i = 0; string1[i] != '\0'; i++)
	    	glutBitmapCharacter(font_style, string1[i]);
	    	
	char string2[30]="Play wisely and get points ";
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-150, 100);
	    for (int i = 0; string2[i] != '\0'; i++)
	    	glutBitmapCharacter(font_style, string2[i]);
	    	
	char string3[30]="You can use LIFE LINE also ";
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-150, 0);
	    for (int i = 0; string3[i] != '\0'; i++)
	    	glutBitmapCharacter(font_style, string3[i]);
	    	
	char string4[20]="GOOD LUCK ";
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-100, -100);
	    for (int i = 0; string4[i] != '\0'; i++)
	    	glutBitmapCharacter(font_style, string4[i]);

	std::string text = "Back";  
	drawButton(text.data(),text.size(),-70,-200);
	glutMouseFunc(mouseline12);  		    		    	
	glFlush();
}


void idle(void)
{
	cout<<"idle"<<endl;
	static int y=0;
	if(new_game==0 && load_game==0 && high_score==0)
		return ;

	char string[40] = "Enter Your Name";
	glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
		glColor4f(1.0, 1.0, 1.0,1);
		glVertex2f(-120,135);
		glVertex2f(50,135);
		glVertex2f(50,80);
		glVertex2f(-120,80);
		glVertex2f(-120,135);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor4f(1.0, 1.0, 1.0,1);
		glVertex2f(-120,35);
		glVertex2f(50,35);
		glVertex2f(50,-20);
		glVertex2f(-120,-20);
		glVertex2f(-120,35);
	glEnd();


	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-100, 100);
	for (int i = 0; string[i] != '\0'; i++)
		glutBitmapCharacter(font_style, string[i]);

	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f(-100,0);
	for (int i = 0; name[i] != '\0'; i++)
		glutBitmapCharacter(font_style, name[i]);

	glColor4f(1.0, 1.0, 1.0,0);
	if(y%20==0){ glutBitmapCharacter(font_style, '|'); y=0; }
	y++;
	glFlush();
}

void idle2(void)
{
	cout<<"idle2"<<endl;
	if(return_from_hs==0)
		return ;
	glLineWidth(1.0);
	char string1[40] = "High Score";
	int hs = obj.high_score;
	string h = to_string(hs);

	glBegin(GL_LINE_STRIP);
		glColor4f(1.0, 1.0, 1.0,1);
		glVertex2f(-120,135);
		glVertex2f(50,135);
		glVertex2f(50,80);
		glVertex2f(-120,80);
		glVertex2f(-120,135);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor4f(1.0, 1.0, 1.0,1);
		glVertex2f(-120,35);
		glVertex2f(50,35);
		glVertex2f(50,-20);
		glVertex2f(-120,-20);
		glVertex2f(-120,35);
	glEnd();


	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-100, 100);
	for (int i = 0; string1[i] != '\0'; i++)
		glutBitmapCharacter(font_style, string1[i]);

	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f(-100,0);
	for (int i = 0; h[i] != '\0'; i++)
		glutBitmapCharacter(font_style, h[i]);

	glFlush();
	std::string text = "Back";  
	drawButton(text.data(),text.size(),-70,-200);
	glutMouseFunc(mouseline12);  		    		    	
	glFlush();
}

int firsttime3 = 1;
void chooseQuestion(Question *q)
{
	cout<<"chooseQuestion"<<endl;
	ifstream fp;
	fp.open("ques1.txt");
	if(firsttime3)
	{
		firsttime3 = 0;
		fp >> nq;
		cout<<nq<<endl;
		generateSetOfNumbers(order_of_ques,5,nq);
		for(int i=0;i<5;i++)
			cout<<order_of_ques[i]<<" ";
		cout<<endl;
	}
	int i = order_of_ques[qlevel];
	string buf;
	for(int j=0;j<i;j++)
	{
		getline(fp,buf);
		getline(fp,buf);
		getline(fp,buf);
		getline(fp,buf);
		getline(fp,buf);
	}
	getline(fp,buf);
 
	getline(fp,q->ques);
	getline(fp,q->cAns.ans);
	getline(fp,q->wAns[0].ans);
	getline(fp,q->wAns[1].ans);
	getline(fp,q->wAns[2].ans);
	audiencePoll(q);
	for(int i=0;i<3;i++)
		q->wAns[i].flag = true;
	//fiftyFifty(q);
}

void idle3(Question *Q,int opt)
{	
	cout<<"idle3"<<endl;
	glBegin(GL_POLYGON);
		glColor4f(0.294, 0.000, 0.510,1);
		glVertex2f(minx2[4],maxy2[4]);
		glVertex2f(maxx2[4],maxy2[4]);
		glVertex2f(maxx2[4],miny2[4]);
		glVertex2f(minx2[4],miny2[4]);
	glEnd();
	minx[4] = minx2[4];miny[4] = miny2[4];maxx[4] = maxx2[4];maxy[4] = maxy2[4];

	for(int i=0;i<4;i++)
	{
		glBegin(GL_POLYGON);
			glColor4f(col[i][0], col[i][1], col[i][2],col[i][3]);
			glVertex2f(minx2[i],maxy2[i]);
			glVertex2f(maxx2[i],maxy2[i]);
			glVertex2f(maxx2[i],miny2[i]);
			glVertex2f(minx2[i],miny2[i]);
		glEnd();
	}
	
	if(ffa)
	{
		string text = "Fifty-Fifty";  
		drawButton2(text.data(),text.size(),-350,350);
		glutMouseFunc(mouseline12);
	}

	if(apa)
	{
		string text = "Audience Poll";  
		drawButton2(text.data(),text.size(),-350,220);
		glutMouseFunc(mouseline12);
	}

	if(apu)
	{	
		string opt[4] = {"A","B","C","D"};
		for(int i=0;i<4;i++)
		{
			if(k[i]!=3 && Q->wAns[k[i]].flag)
			{
				string st = opt[i];
				GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
				glColor4f(1.0, 1.0, 1.0,1);
				glRasterPos2f (-350+i*20,20);
				for (int i = 0; st[i] != '\0'; i++)
					glutBitmapCharacter(font_style, st[i]);
				glBegin(GL_POLYGON);
					glColor4f(dcol[0], dcol[1], dcol[2],dcol[3]);
					glVertex2f(-350+i*20,60+Q->wAns[k[i]].prob);
					glVertex2f(-340+i*20,60+Q->wAns[k[i]].prob);
					glVertex2f(-340+i*20,60);
					glVertex2f(-350+i*20,60);
				glEnd();
			}
			else if(k[i]==3)
			{
				string st = opt[i];
				GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
				glColor4f(1.0, 1.0, 1.0,1);
				glRasterPos2f (-350+i*20,20);
				for (int i = 0; st[i] != '\0'; i++)
					glutBitmapCharacter(font_style, st[i]);
				glBegin(GL_POLYGON);
					glColor4f(dcol[0], dcol[1], dcol[2],dcol[3]);
					glVertex2f(-350+i*20,60+Q->cAns.prob);
					glVertex2f(-340+i*20,60+Q->cAns.prob);
					glVertex2f(-340+i*20,60);
					glVertex2f(-350+i*20,60);
				glEnd();
			}
		}
	}

    string text = "Forfeit";  
	drawButton1(text.data(),text.size(),-50,-400);
	glutMouseFunc(mouseline12);
	
	char ques[140];
	strcpy(ques, Q->ques.c_str());
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (c2x[4], c2y[4]);
	for (int i = 0; ques[i] != '\0'; i++)
		glutBitmapCharacter(font_style, ques[i]);

	char ans[40];	
	/*if(firsttime2 && !answered)
	{
		generateSetOfNumbers(k,4,4);	
	}*/
	for(int i=0;i<4;i++)
	{
		if(k[i]==3)
		{
			strcpy(ans, Q->cAns.ans.c_str());
			minx[3] = minx2[i];
			miny[3] = miny2[i];
			maxx[3] = maxx2[i];
			maxy[3] = maxy2[i];
		}
		else
		{
			strcpy(ans, Q->wAns[k[i]].ans.c_str());
			minx[k[i]] = minx2[i];
			miny[k[i]] = miny2[i];
			maxx[k[i]] = maxx2[i];
			maxy[k[i]] = maxy2[i];
		}
		if(k[i]==3 || Q->wAns[k[i]].flag)
		{
			glColor4f(1.0, 1.0, 1.0,1);
			glRasterPos2f (c2x[i], c2y[i]);
			for (int i = 0; ans[i] != '\0'; i++)
				glutBitmapCharacter(font_style, ans[i]);
		}
	}
	string st = "TIMER";
	glRasterPos2f (303,350);
	for (int i = 0; st[i] != '\0'; i++)
			glutBitmapCharacter(font_style, st[i]);
	int ms= glutGet(GLUT_ELAPSED_TIME);
	time_ans=31+adt-(float)ms/1000;
	string s1;
	stringstream co1;
	co1 << time_ans;
	s1=co1.str();
	drawText(s1.data(),s1.size(),323,309);
	if(time_ans==0)
	{
	    glutCreateSubWindow(1,400,200,400,200);
		glClearColor(0,0,1,1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0,200,0.0,200,-1.0,1.0);
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glFlush();
		glColor3f(1.0,0.4,0.2);
		string text = "You Lost!";
		drawText(text.data(),text.size(),85,150);
		text = "Exit";
		drawButton(text.data(),text.size(),85,100);
		glutMouseFunc(mouseline11);
		glutSwapBuffers();
	}
	//answered = false;
}

void generateSetOfNumbers(int arr[], int n, int k)
{
	int p[k];
	for(int i=0;i<k;i++)
		p[i] = i;
	for (int i=k-1; i>0; --i)
	{
		int j = rand()%i;
		int temp = p[i];
		p[i] = p[j];
		p[j] = temp;
	}
	for (int i=0; i<n; ++i)
		arr[i] = p[i];
}

void mouseline12(int button, int state,int x1, int y1)
{
	cout<<"mouseline12"<<endl;
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		x1 = ((float)1000/1300)*x1 - 500;
		y1 = 500 - ((float)1000/750)*y1;
		cout<<x1<<" "<<y1<<endl;
		apu = false;
		if(x1>minx[3] && x1<maxx[3] && y1>miny[3] && y1<maxy[3])
		{
			answered = true;
			option_selected = 3;
			time_ans = 30;
			if(qlevel<4)
			{
				cout<<"qlevel: "<<qlevel<<endl;
				qlevel++;
				score = score_array[qlevel];
				obj.high_score = score;
				obj.update();
				load_game=0;new_game=0;game_menu=0;firsttime2 = 1;
				return_from_nglg = 0;return_from_qa=1;
				display();
			}
			else
			{
				//sleep(1);
				qlevel++;
				score = score_array[qlevel];
				obj.high_score = score;
				obj.update();
				createSubWindow("You Won!");
			}
		}

		else if(x1>-68 && x1<-30 && y1>-192 && y1<-157)
		{
		    game_menu=1;
			help = 0;
			high_score = 0;
			for(int i=0;i<strlen(name);i++)
				name[i] = '\0';
		}
	
		else if(x1>-28 && x1<75 && y1>-400 && y1<-300)
		{
		    game_menu=1;
			for(int i=0;i<strlen(name);i++)
				name[i] = '\0';
		}

		else if(x1>-350 && x1<-200 && y1>350 && y1<450)
		{
			ffa = false;
			fiftyFifty(&Q);		
		}

		else if(x1>-350 && x1<-200 && y1>220 && y1<320)
		{
			apa = false;
			apu = true;
		}		
		
		else if(x1>minx[0] && x1<maxx[0] && y1>miny[0] && y1<maxy[0])
		{	
			answered = true;
			option_selected = 0;
			return_from_nglg = 0;
			return_from_qa = 1;
			score = score_array[qlevel];
			obj.high_score = score;
			obj.update();
			createSubWindow("You Lost!");
		}

		else if(x1>minx[1] && x1<maxx[1] && y1>miny[1] && y1<maxy[1])
		{	
			answered = true;
			option_selected = 1;
			return_from_nglg = 0;
			return_from_qa = 1;
			score = score_array[qlevel];
			obj.high_score = score;
			obj.update();
			createSubWindow("You Lost!");
		}

		else if(x1>minx[2] && x1<maxx[2] && y1>miny[2] && y1<maxy[2])
		{	
			answered = true;
			option_selected = 2;
			return_from_nglg = 0;
			return_from_qa = 1;
			score = score_array[qlevel];
			obj.high_score = score;
			obj.update();
			createSubWindow("You Lost!");
		}
	}	
	glutPostRedisplay();
}

void createSubWindow(string msg)
{
	glutCreateSubWindow(1,400,200,400,200);
	glClearColor(0,0,1,1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,200,0.0,200,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glFlush();
	glColor3f(1.0,0.4,0.2);
	string text = msg;
	drawText(text.data(),text.size(),85,150);
	text = "Final Score:";
	drawText(text.data(),text.size(),85,50);
	string s1;
	stringstream co1;
	co1 << obj.high_score;
	s1=co1.str();
	drawText(s1.data(),s1.size(),150,50);
	text = "Exit";
	drawButton(text.data(),text.size(),85,100);
	glutMouseFunc(mouseline11);
	glutSwapBuffers();
}

void timer(int )
{
	glutPostRedisplay();
	glutTimerFunc(1000,timer,0);
}

void mouseline11(int button, int state,int x1, int y1)
{	cout << x1 << " " <<y1<<endl;
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{  
		if(x1>172 && x1<227 && y1>71 && y1<99)
		{
			int w1=glutGetWindow();
			glutSetWindow(menuw);
			glutDestroyWindow(w1);	
			glutDestroyWindow(menuw);
		}
	}
	glFlush();
}

void audiencePoll(Question *q)
{
	int sum = 0;
	q->cAns.prob = rand()%50 + 50;
	sum += q->cAns.prob;
	q->wAns[0].prob = rand()%(100 - sum);
	sum += q->wAns[0].prob;
	q->wAns[1].prob = rand()%(100 - sum);
	sum += q->wAns[1].prob;
	q->wAns[2].prob = (100 - sum);
}
    
void fiftyFifty(Question *q)
{
	int rand1 = rand()%3;
	int rand2 = rand()%3;
	while(rand2==rand1)
		rand2 = rand()%3;
	q->wAns[rand1].flag = q->wAns[rand2].flag = false;
}


void already(int x)
{
	static int y=0;
	if(new_game==0 && load_game==0)
		return ;

	char string1[40] = "Name already exists, try another";

	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 0, 0,1);
	glRasterPos2f (-100, -100);
	for (int i = 0; string1[i] != '\0'; i++)
		glutBitmapCharacter(font_style, string1[i]);
}

void does(int x)
{
	static int y=0;
	if(new_game==0 && load_game==0 && high_score==0)
		return ;

	char string1[40] = "This name doesn't exist";

	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 0, 0,1);
	glRasterPos2f (-100, -100);
	for (int i = 0; string1[i] != '\0'; i++)
		glutBitmapCharacter(font_style, string1[i]);
	glutPostRedisplay();
}
void drawButton2(const char *text, int length ,int x ,int y )
{	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glLineWidth(5.0);
	glColor3f(1.000, 0.000, 0.000);
	glBegin(GL_QUADS);
        glColor3f(0.373, 0.620, 0.627);
		glVertex2f(x,y);
		glVertex2f(x+150,y);
		glVertex2f(x+150,y+100);
		glVertex2f(x,y+100);
	glEnd();
	glColor3f(0.502, 0.000, 0.502);
	glRasterPos2i(x+30,y+50);
	int i;
	for(i=0;i<length;i++)
		glutBitmapCharacter(font_style,(int)text[i]);
}

void drawButton1(const char *text, int length ,int x ,int y )
{	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glLineWidth(5.0);
	glColor3f(1.000, 0.000, 0.000);
	glBegin(GL_QUADS);
        glColor3f(0.373, 0.620, 0.627);
		glVertex2f(x,y);
		glVertex2f(x+100,y);
		glVertex2f(x+100,y+100);
		glVertex2f(x,y+100);
	glEnd();
	glColor3f(0.502, 0.000, 0.502);
	glRasterPos2i(x+30,y+50);
	int i;
	for(i=0;i<length;i++)
		glutBitmapCharacter(font_style,(int)text[i]);
}
void drawButton(const char *text, int length ,int x ,int y )
{	
	glLineWidth(5.0);
	glColor3f(1.000, 0.000, 0.000);
	glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
		glVertex2f(x,y);
		glVertex2f(x+40,y);
		glVertex2f(x+40,y+40);
		glVertex2f(x,y+40);
	glEnd();
	glColor3f(0.502, 0.000, 0.502);
	glRasterPos2i(x+6,y+12);
	int i;
	for(i=0;i<length;i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
}

void drawText(const char *text, int length ,int x ,int y )
{	
	glColor3f(1.0,0.4,0.2);
	glRasterPos2i(x,y);
	int i;
	for(i=0;i<length;i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)text[i]);
}
