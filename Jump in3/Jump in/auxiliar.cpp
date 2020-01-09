#include<graphics.h>
#include<winbgim.h>
#include<iostream>
#include<fstream>
using namespace std;
int TablaDeJoc[5][5];
int stanga,sus,width,height,latura, numar=5,nrIepuri=0;
ifstream f ("tabl.txt");

void citireTabla()
{
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
        {
            f>>TablaDeJoc[j][i];
        }
}

void desenareTabla()
{
    int i,j;
    numar=5;
    width=400;
    height=400;
    latura=80;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,CYAN);
    for (i=0; i<numar; i++)
        for (j=0; j<numar; j++)
            bar3d(stanga+latura*(i),sus+latura*(j),stanga+latura*(i+1),sus+latura*(j+1),1,1);
}

void desenarePiese()
{
    width=400;
    height=400;
    latura=width/numar;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            if(TablaDeJoc[i][j]==1)
            {
                readimagefile("rabbit.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
                // fillellipse((stanga+latura*(i)+stanga+latura*(i+1))/2,(sus+latura*(j)+sus+latura*(j+1))/2,20,20);
            }
            else if(TablaDeJoc[i][j]==-1)
            {
                readimagefile("mushroom.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            }
            else if(TablaDeJoc[i][j]==-2 && TablaDeJoc[i+1][j]==-2 && i+1<5)
            {
                 readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
              readimagefile("fox.jpg",stanga+latura*(i+1)+2,sus+latura*(j)+2,stanga+latura*(i+2)-2,sus+latura*(j+1)-2);
            }
           else if(TablaDeJoc[i][j]==-2 && TablaDeJoc[i][j+1]==-2)
            {
                readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
                readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j+1)+2,stanga+latura*(i+1)-2,sus+latura*(j+2)-2);
            }
            else if(TablaDeJoc[i][j]==2)
            {
                readimagefile("hole.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            }
            else if(TablaDeJoc[i][j]==3)
                readimagefile("rabbit_in_hole.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
}
main()
{
     initwindow(800,600,"JUMPIN'",true);
      readimagefile("wallpaper2.jpg",0,0,800,600);
      int x,y;
    while(!ismouseclick(WM_LBUTTONDOWN))
        ;
    clearmouseclick(WM_LBUTTONDOWN);
    x=mousex();
    y=mousey();
    cout<<x<<' '<<y;
    /*
    while(1){
    citireTabla();
    desenareTabla();
    desenarePiese();
getch();    }
*/
}
