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
int new_game = 0,load_game = 0,high_score = 0,already_exists = 0, does_not_exist = 0,return_from_nglg = 0,return_from_hs = 0;
int level = 0,active_option = 0;
int menuw;
GLuint tex[5];
char name[32];
double px[]={-480,-480,-480,-480,-480}, py[]={200,100,0,-100,-200},transp[]={1,0.2,0.2,0.2,0.2};
double minx[4],maxx[4],miny[4],maxy[4];

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
			ifstream infile1("Entry.txt");
			int a,b;
			string n;
			while(!infile1.eof())
			{
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
					std::cout<<"Found\n";
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
					std::cout<<"Found\n";
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
//void Idle(void);
void idle2(void);
void idle3(void);
void mouseline12(int button, int state,int x1, int y1);
void generateSetOfNumbers(int arr[], int n);
void audiencePoll(Question *q);
void fiftyFifty(Question *q);
void already(int x);
void does(int x);


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
    glutIdleFunc(idle);
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
	cout<<tex[i]<<"\n";
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

		else if(game_menu==0 && new_game==1)
		{
			if(strlen(name)==0)
				return;

			if(obj.newuser(name))
			{
				//cout<<"here"<<endl;
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
				return;

			if(obj.loaduser(name))
			{
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
	draw_font("Quit Game",px[3]+120,py[3]-45);
	draw_option(4);
	glColor3f(0.8,0.2,0.2);
	draw_font("Help",px[4]+140,py[4]-45);
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
	int i,width=-1000,height=1000;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.4,0.5,0.1);
	if (game_menu)
		show_menu();

	else if(new_game)
	{
		show_background(2);
		idle();
		if(already_exists)
			already(0);
		else
			return_from_nglg = 1;
	}

	else if(load_game)
	{
		glClear(GL_COLOR_BUFFER_BIT);   
		show_background(2);
		idle();
		if(does_not_exist)
			does(0);
		else
			return_from_nglg = 1;
	}
	
	else if(high_score)
	{
		glClear(GL_COLOR_BUFFER_BIT);   
		show_background(2);
		idle();
		if(does_not_exist)
			does(0);
	}

	else if(return_from_hs)
	{
		show_background(1);
		idle2();
		return_from_hs=0;
	}

	else
	{
		show_background(1);
		idle3();
	}
	GLuint tx1,tx2;

	/*if(!game_menu&&!(new_game==1||load_game==1))
		ship.draw();
	
	for(i=0;i<particles.size()&&!game_menu&&!(new_game==1||load_game==1);i++)
	{
		if(particles[i].active)
			particles[i].draw();
	}
	glColor3f(0.4,0.5,0.1);
	for(i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++)
	{
		if(bullet[i].active)
			bullet[i].draw();
	}*/
	glutSwapBuffers();
}

void idle(void)
{
	static int y=0;
	if(new_game==0 && load_game==0 && high_score==0)
		return ;

	char string[40] = "Enter Your Name";

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

/*void Idle(void)
{
	fq++;
	fq2++;
	int yc=35;
	if(fq==1000)
	{
		fq=0;
		if(!pause_game)
			glutPostRedisplay();
	}
	if(fq2==10e4)
	{
		fq2=0;
		if(!game_menu&&!(new_game==1||load_game==1))
		{
			new_particle();
			int i=particles.size()-1;
			particles[i].x=rand()%1000-500;
			particles[i].y=rand()%400+100;
			if(ship.x==particles[i].x) particles[i].m=10e10;
			else
				particles[i].m = (ship.y-ship.height/2-particles[i].y+particles[i].height/2) \
				/(ship.x-ship.width/2-particles[i].x+particles[i].width/2);
		}
	}
}*/

void idle2(void)
{
	if(return_from_hs==0)
		return ;

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
}

void idle3(void)
{
	cout<<"here"<<endl;
	ifstream fp;
	fp.open("ques1.txt");
	int nq;
	fp >> nq;
	cout<<nq<<endl;
	int i = rand()%nq;
	cout<<i<<endl;
	Question q;
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
 
	getline(fp,q.ques);
	getline(fp,q.cAns.ans);
	getline(fp,q.wAns[0].ans);
	getline(fp,q.wAns[1].ans);
	getline(fp,q.wAns[2].ans);
	audiencePoll(&q);
	fiftyFifty(&q);

	glBegin(GL_POLYGON);
		glColor4f(0.294, 0.000, 0.510,1);
		glVertex2f(-360,0);
		glVertex2f(360,0);
		glVertex2f(360,-150);
		glVertex2f(-360,-150);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.282, 0.239, 0.545,1);
		glVertex2f(-480,-200);
		glVertex2f(-200,-200);
		glVertex2f(-200,-300);
		glVertex2f(-480,-300);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.282, 0.239, 0.545,1);
		glVertex2f(200,-200);
		glVertex2f(480,-200);
		glVertex2f(480,-300);
		glVertex2f(200,-300);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4f(0.282, 0.239, 0.545,1);
		glVertex2f(-480,-400);
		glVertex2f(-200,-400);
		glVertex2f(-200,-500);
		glVertex2f(-480,-500);
	glEnd();
	
	glBegin(GL_POLYGON);
		glColor4f(0.282, 0.239, 0.545,1);
		glVertex2f(200,-400);
		glVertex2f(480,-400);
		glVertex2f(480,-500);
		glVertex2f(200,-500);
	glEnd();

	char ques[140];
	strcpy(ques, q.ques.c_str());
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor4f(1.0, 1.0, 1.0,1);
	glRasterPos2f (-270, -70);
	for (int i = 0; ques[i] != '\0'; i++)
		glutBitmapCharacter(font_style, ques[i]);

	char ans[40];	
	double c2x[] = {-400,250,-400,250};
	double c2y[] = {-230,-230,-440,-440};
	double minx2[] = {-480,200,-480,200};
	double miny2[] = {-300,-300,-500,-500};
	double maxx2[] = {-200,480,-200,480};
	double maxy2[] = {-200,-200,-400,-400};
	int k[4];
	generateSetOfNumbers(k,4);	
	for(i=0;i<4;i++)
	{
		if(k[i]==3)
		{
			strcpy(ans, q.cAns.ans.c_str());
			minx[3] = minx2[i];
			miny[3] = miny2[i];
			maxx[3] = maxx2[i];
			maxy[3] = maxy2[i];
		}
		else
		{
			strcpy(ans, q.wAns[k[i]].ans.c_str());
			minx[k[i]] = minx2[i];
			miny[k[i]] = miny2[i];
			maxx[k[i]] = maxx2[i];
			maxy[k[i]] = maxy2[i];
		}
		glColor4f(1.0, 1.0, 1.0,1);
		glRasterPos2f (c2x[i], c2y[i]);
		for (int i = 0; ans[i] != '\0'; i++)
			glutBitmapCharacter(font_style, ans[i]);
	}
}

void generateSetOfNumbers(int arr[], int n)
{
	int p[4] = {0,1,2,3};
	for (int i=3; i>0; --i)
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
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		if(x1>minx[3] && x1<maxx[3] && y1>miny[3] && y1<maxy[3])
		{
			glBegin(GL_POLYGON);
				glColor4f(0.486, 0.988, 0.000,1);
				glVertex2f(minx[3],miny[3]);
				glVertex2f(maxx[3],miny[3]);
				glVertex2f(maxx[3],maxy[3]);
				glVertex2f(minx[3],maxy[3]);
			glEnd();
		}
	}
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
