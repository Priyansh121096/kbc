#include<bits/stdc++.h>
#include <GL/glut.h>
#include<time.h>
#include <IL/il.h>
#include <unistd.h>
#include <pthread.h>
#include<iostream>
#include<string>
using namespace std;
#define num_particles 20
#define num_bullets 15
int load_texture(char *img, int i);
void exit_prog(int);
pthread_t pth;
volatile int stopsound=0,game_menu,pause_game=0;
int freq = 100,fq=0,fq2,bl=0,bpos=0,active_particles=num_particles,mainw,menuw;
int active_option=0;
double px[]={-480,-480,-480,-480}, py[]={200,100,0,-100},transp[]={1,0.2,0.2,0.2};
GLuint tex[10];





int new_game=0;
int load_game=0;
int already_exist=0;
int does_not_exist=0;


class user{

public:
            
    string name;
    int level;
    int high_score;
    
    user(){
    name = "";
    level=0;high_score=0; 
    }
    
    bool newuser(string nme){
      name = nme;  
      
      ifstream infile1("Entry.txt");
        int a,b;
        string n;
        while(!infile1.eof()){
        //std::cout<<"Infinite\n";
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
    
    bool loaduser(string nme){
        ifstream infile("Entry.txt");
        int a,b;
        string n;
        while(!infile.eof()){
        //std::cout<<"Infinite\n";
        infile>>n;
        
        if(infile.eof()){  break; }
        infile>>a;
        infile>>b;
        //std::cout<<n<<" "<<a<<" "<<b<<"\n";
        
        if((n==nme)){ std::cout<<"Found\n"; name = nme;level=a;high_score=b; infile.close(); return true; }
        
        }    
        infile.close();
    return false;            
    } 
    
    
    bool update(){
        
        fstream infile;
        infile.open("Entry.txt");
        int a,b;
        string n;
        
        while(!infile.eof()){ 
        infile>>n;
        if((n==name)){
        int pos = infile.tellp();
        infile>>a;
        infile>>b;
        
        char buff[2000]={'\0'};        
        int count=0;
        while(!infile.eof()){  infile.get(buff[count]); if(infile.eof()) break;  count++; }
                infile.clear();
        infile.seekp(pos,ios::beg);        
        
        infile<<'\n'<<level<<endl;
        infile<<high_score;
        
        for(int i=0;i<count;i++)
        infile.put(buff[i]);

        infile.close(); return true; }
        
        
        }    
        
        infile.close();
    return false;        
        
    }
    
    
    void display(){
    
        std::cout<<"Name : "<<name<<endl;
        std::cout<<"Level : "<<level<<endl;
        std::cout<<"High Score : "<<high_score<<endl;
    
    }          
    
    
};


user obj;
char name[40];


void idle(int x){

static int y=0;
if(new_game==0 && load_game==0)
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

//std::cout<<"user prog "<<game_user<<"\n";

//glutTimerFunc(100,idle,x^1);

}

void already(int x){

static int y=0;
if(new_game==0 && load_game==0)
return ;

char string[40] = "Name is Already Exist ,Try Another";
   

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        glColor4f(1.0, 0, 0,1);
        glRasterPos2f (-100, -100);
         for (int i = 0; string[i] != '\0'; i++)
        glutBitmapCharacter(font_style, string[i]);


}


void does(int x){

static int y=0;
if(new_game==0 && load_game==0)
return ;

char string[40] = "This name Doensn't Exist";
   

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        glColor4f(1.0, 0, 0,1);
        glRasterPos2f (-100, -100);
         for (int i = 0; string[i] != '\0'; i++)
        glutBitmapCharacter(font_style, string[i]);


}














class Particle{
public:
    int active;
    double x,y,width,height,m;
    double mass, vel;
    Particle(){
        active=1;width=30;height=50;
        x=0;    y=0; m=(rand()%500-250)/250.0;
        mass=1; vel=1;
    }
    Particle(double a, double b){
        active=1;
        x=a; y=b;
    }
    void reset(){
        active=0;
        active_particles--;
    }
    void move(double dt){
        y = y - vel*dt;
        if(m!=0) x = x-1/m;
    }
    void draw(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex[2]);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        glColor4f(1,1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x,y-height);
        glTexCoord2i(0, 1); glVertex2i(x-width, y-height);
        glTexCoord2i(1, 1); glVertex2i(x-width, y);
        glTexCoord2i(1, 0); glVertex2i(x, y);
        glEnd();
        glFlush();
        glDisable(GL_TEXTURE_2D);
    }
};
class SpaceShip{
public:
    double x,y,width,height;
    SpaceShip(){
        x=-40;y=-350;width=50;height=100;
    }
    void draw(){
//  load_texture("spaceship.png");
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex[1]);
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glColor4f(1,1,1,1);
  /* Draw a fullscreen mapped quad */
  glBegin(GL_QUADS);

      glTexCoord2i(0, 0); glVertex2i(x,y);
      glTexCoord2i(0, 1); glVertex2i(x, y-height);
      glTexCoord2i(1, 1); glVertex2i(x-width, y-height);
      glTexCoord2i(1, 0); glVertex2i(x-width, y);
  glEnd();
      /*  glBegin(GL_POLYGON);
        glVertex2f(x+25,y+60);
        glVertex2f(x+50,y);
        glVertex2f(x+30,y+15);
        glVertex2f(x+20,y+15);
        glVertex2f(x,y);*/
        //glVertex2f(x+30,y+40);
        //glVertex2f(x+20,y+40);
        //glVertex2f(x,y);
      //  glEnd();
        glFlush();
        glDisable(GL_TEXTURE_2D);
    }
};
SpaceShip ship;
class Bullet:public SpaceShip{
public:
    double x,y,width,height;
    int active;
    //SpaceShip sp;
    Bullet(){
        active=1;width=15;height=30;
        x = ship.x-18;
        y = ship.y+10;
    }
    void advance(double a){
        y+=a;

    }
    void reset(){
        active=0;
    }
    void draw(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex[4]);
      glClearColor (1.0, 1.0, 1.0, 1.0);
       glColor4f(1,1,1,1);
        /* Draw a fullscreen mapped quad */
        glBegin(GL_QUADS);

            glTexCoord2i(0, 0); glVertex2i(x,y);
            glTexCoord2i(0, 1); glVertex2i(x, y-height);
            glTexCoord2i(1, 1); glVertex2i(x-width, y-height);
            glTexCoord2i(1, 0); glVertex2i(x-width, y);
        glEnd();/*
        glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+5,y);
        glVertex2f(x+5,y+10);
        glVertex2f(x,y+10);
        glEnd();*/
        glFlush();
    }
};
//Bullet bullet[num_bullets];
vector< Bullet > bullet;
vector< Particle > particles;

//Particle particles[num_particles];
void *playfunc(void *arg){
    //while(!stopsound)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    system("cvlc shoot.wav");
}
void timer(int val){
    int i,j,yc=35;
    if(!game_menu&&!(new_game==1||load_game==1)){

    for( i=0;i<particles.size();i++)
        if(particles[i].active)
            particles[i].move(4);
            if(particles[i].y>500 || particles[i].y<-500)
            {
                cout<<particles.size()<<endl;
                particles.erase(particles.begin()+i);
            }// bullet[i].advance(25);
            else if(particles[i].x>500 || particles[i].x<-500){
                particles.erase(particles.begin()+i);
            }

    bl = min(bl+1,num_bullets);
    for( i=0;i<bullet.size();i++){
        bullet[i].advance(yc);
        if(bullet[i].y>500 || bullet[i].y<-500)
        {
            bullet.erase(bullet.begin()+i);
        }// bullet[i].advance(25);
        }
       // if(!active_particles) {printf("You Won The Game\n");exit(0);}
    }
    //pthread_create(&pth,NULL,collision,NULL);
    //collision();

    glutTimerFunc(30, timer, 0);
}
void fire_bullet(){
Bullet blt;
bullet.push_back(blt);
}
void new_particle(){
    //cout<<"\nin new";
    Particle prt;
    particles.push_back(prt);
}
void collision(int val){

    int i,j;
    double miny=-400;
    for(i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++){
        if(!bullet[i].active) continue;
        for(j=0;j<particles.size();j++){
            if(!particles[j].active) continue;
            if(bullet[i].x-bullet[i].width<particles[j].x &&bullet[i].x-bullet[i].width > \
                    particles[j].x-particles[j].width && bullet[i].y>particles[j].y-particles[j].height && \
                bullet[i].y< particles[j].y)
                {
                    //bullet[i].reset();
                    bullet.erase(bullet.begin()+i);
                    particles.erase(particles.begin()+j);
                //    particles[j].reset();
                }
        else if(bullet[i].x<particles[j].x &&bullet[i].x > \
            particles[j].x-particles[j].width && bullet[i].y>particles[j].y-particles[j].height && \
        bullet[i].y< particles[j].y)
        {

        //    bullet[i].reset();
        //    particles[j].reset();
            bullet.erase(bullet.begin()+i);
            particles.erase(particles.begin()+j);
        }
        //if(particles[j].active) miny = min(miny, particles[j].y-10);
        }
        }
        for(j=0;j<particles.size();j++){
            if(!particles[j].active) continue;
                if(ship.x-ship.width<particles[j].x &&ship.x-ship.width > \
                    particles[j].x-particles[j].width && ship.y>particles[j].y-particles[j].height && \
                   ship.y<particles[j].y)
                {
                    exit_prog(0);
                }
        else if(ship.x<particles[j].x &&ship.x > \
            particles[j].x-particles[j].width && ship.y>particles[j].y-particles[j].height && \
           ship.y<particles[j].y)
        {
            exit_prog(0);
        }
        //if(particles[j].active) miny = min(miny, particles[j].y-10);
        }


    glutTimerFunc(30, collision, 0);
}


void init(void)
{       glClear(GL_COLOR_BUFFER_BIT);
         int tx =load_texture("kbc1.jpg", 0);
        tx = load_texture("spaceship.png", 1);
         tx = load_texture("a11.png", 2);
         tx = load_texture("aest.jpg", 3);
         tx=load_texture("lasergun.png",4);

         glEnable(GL_TEXTURE_2D);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        glOrtho (-500,500 ,-500,500,0,100);
        int i,base=20;
        glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
         glLoadIdentity();
         glEnable(GL_BLEND);
           glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glOrtho(0.0, 500, 500, 0.0, 0.0, 100.0);

         glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
         glClearDepth(0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
        for(i=0;i<num_particles;i++){
            new_particle();
            particles[i].x=base+(i%10)*60 - 400;
            particles[i].y=particles[i].y - (i/10)*70 + 700;
        }

        timer(0);
        collision(0);

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
void show_background(int i){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex[i]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-500, -500);
        glTexCoord2i(0, 1); glVertex2i(-500, 500);
        glTexCoord2i(1, 1); glVertex2i(500, 500);
        glTexCoord2i(1, 0); glVertex2i(500, -500);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}
void draw_option(int i){
    glBegin(GL_POLYGON);
    glColor4f(0.1,0.5,0.65,transp[i]);
    glVertex2f(px[i],py[i]);
    glVertex2f(px[i]+330,py[i]);
    glVertex2f(px[i]+330,py[i]-70);
    glVertex2f(px[i],py[i]-70);
    glEnd();

    glFlush();
}
void draw_font(string s,double x,double y){

    glRasterPos2f(x, y);
    for (int i = 0; i < s.size(); i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
  }
}
void show_menu(){
    show_background(3);
    draw_option(0);
    glColor3f(0.8,0.2,0.2);
    draw_font("New Game",px[0]+125,py[0]-45);
    draw_option(1);
    glColor3f(0.8,0.2,0.2);
    draw_font("Load Game",px[1]+122,py[1]-45);
    draw_option(2);
    glColor3f(0.8,0.2,0.2);
    draw_font("Quit Game",px[2]+120,py[2]-45);
    draw_option(3);
    glColor3f(0.8,0.2,0.2);
    draw_font("Help",px[3]+150,py[3]-45);
}

void display(void){
    int i,width=-1000,height=1000;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.4,0.5,0.1);
    if (game_menu) {
        show_menu();
    }
    
    else if(new_game){
         //glClear(GL_COLOR_BUFFER_BIT);   
         show_background(0);
         idle(0);
         if(already_exist){
            already(0);
         }
    }
    else if(load_game){
         glClear(GL_COLOR_BUFFER_BIT);   
         show_background(0);
         idle(0);
         if(does_not_exist){
            does(0);
    }
    }
    else{

        show_background(0);
    }
    GLuint tx1,tx2;

    if(!game_menu&&!(new_game==1||load_game==1))
    ship.draw();
    for(i=0;i<particles.size()&&!game_menu&&!(new_game==1||load_game==1);i++){
        if(particles[i].active)
        {
            particles[i].draw();
        }
        //glVertex2f(particles[i].x, particles[i].y);
    }
    glColor3f(0.4,0.5,0.1);
    for(i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++){
        if(bullet[i].active)
            bullet[i].draw();
    }
    glutSwapBuffers();
    //glFlush();
}
void Idle(void){
    fq++;
    fq2++;
    int yc=35;
    if(fq==1000){
        fq=0;
        if(!pause_game)
        glutPostRedisplay();
    }
    if(fq2==10e4){
        fq2=0;
        if(!game_menu&&!(new_game==1||load_game==1)){
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
}
void SpecialKey(int key, int x, int y)
{

switch (key) {
    case GLUT_KEY_LEFT:
        ship.x-=25;
        break;
    case GLUT_KEY_RIGHT:
        ship.x+=25;
        break;
    case GLUT_KEY_UP:
        if(game_menu){
        transp[active_option]=0.2;
        active_option-=1;
        active_option+=4;
        active_option%=4;
        transp[active_option]=1;
        show_menu();
        }
        else{
            fire_bullet();
        }
        break;
    case GLUT_KEY_DOWN:
        transp[active_option]=0.2;
        active_option+=1;
        active_option%=4;
        transp[active_option]=1;
        show_menu();
        break;

    }
    if(ship.x+25<-500) ship.x+=1000;
    else if(ship.x+25>500) ship.x-=1000;
    /*for(int i=0;i<num_bullets;i++){
        bullet[i].x=ship.x+20+2.5;
    }*/
}
int firsttime=1;
int load_texture(char * img,int i){
    GLuint    image;
      GLuint texid;
      //cout<<img;
  image = LoadImage(img);
     glGenTextures(1, &tex[i]); /* Texture name generation */
     glBindTexture(GL_TEXTURE_2D,tex[i]); /* Binding of texture name */
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */

     glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
      0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */
       ilDeleteImages(1, &image);
       firsttime=0;
       cout<<tex[i]<<"\n";
       return tex[i];
}
void key_func(unsigned char key,int x,int y){



if(key==13){  // on Enter key

//std::cout<<"Enter"<<endl;

if(active_option==0 && game_menu){
    game_menu=0;
    new_game=1;
    std::cout<<"enteres new\n";
        
}

else if(active_option==1 && game_menu){
    std::cout<<"enteres load\n";
    game_menu=0;
    load_game=1;
}

else if(game_menu==0 && new_game==1 ){

if(strlen(name)==0)
return ;


if(obj.newuser(name))
new_game=0;
else{already_exist=1; return ;}

}

else if(game_menu==0 && load_game==1){

if(strlen(name)==0)
return ;

//std::cout<<"In load Game\n";
//std::cout<<name<<"\n";
if(obj.loaduser(name))
load_game=0;
else{does_not_exist=1; return ;}

}

//glutPostRedisplay();
}

else if(((key>=65 && key<=90) || (key>=65 && key<=122) || key==32 ) && (new_game || load_game) ){    
    name[strlen(name)]=key;
}

else if(key==8 && (new_game || load_game)){

if(strlen(name)>0)
name[strlen(name)-1]='\0';

}



}

void exit_prog(int flag)
{   char cntr[]="543210";
	pause_game=1;
    if(!flag){
	    glColor3f(1,0,0);
        draw_font("You Lose the game..",-100,100);
        glutSwapBuffers();
        sleep(2);
    }
    else{
   	    glColor3f(0,1,0);
        draw_font("You win the game..",-100,100);
         for(int i=5;i>=0;i--) {
			glColor3f(1,0,0);
			draw_font(to_string(i),0,-100);
			glFlush();glutSwapBuffers();
			sleep(1);
			}
    }



	if(!flag)    exit(0);
}
int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize (1362, 750);
    glutInitWindowPosition (0, 0);
    menuw = glutCreateWindow ("Game Menu");
    game_menu=1;
  //  mainw =  glutCreateWindow ("Play Game");
    glutSetWindow(menuw);
    glEnable( GL_TEXTURE_2D );
    ilInit();
    init ();
    glutKeyboardFunc(key_func);
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(display);
    glutIdleFunc(Idle);

    //collision(0);
    //timer(0);



    glutMainLoop();
    stopsound=1;
    //pthread_cancel(pth);
    return 0;
}
