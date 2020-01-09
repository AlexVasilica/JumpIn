/* DE ADAUGAT:
_____________________________
 -> vulpe si mutareVulpe()
 -> buton undo
 -> buton reset puzzle
 X-> meniu
 -> muzica
 -> buton hint
 X-> customizare marime tabla
 -> timp
 -> clasament
 -> buton home
_____________________________
*/
#include<iostream>
#include<fstream>
#include<graphics.h>
#include<winbgim.h>
#include<windows.h>
#include<mmsystem.h>
using namespace std;

ifstream f ("tabla.txt");

unsigned DIM;
int TablaDeJoc[10][10];
int stanga,sus,width,height,latura,numar,nrIepuri=0;
int di[]= {-1,1,0,0};
int dj[]= {0,0,1,-1};

struct pozitieMatrice
{
    unsigned i,j;
} Iepuri[100];


void initializare(unsigned dimensiune)
{
    DIM=dimensiune;//DIM=numarul de linii si coloane ale tablei de joc
    if(dimensiune>=6)//daca DIM>6 atunci se mareste dimesiunea tablei de joc
        width=height=500;
    else
        width=height=400;
    latura=width/DIM;
    sus=(getmaxy()-width)/2;
    stanga=(getmaxx()-height)/2;
}

void actualizare_iepuri()
{
    nrIepuri=0;
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            if(TablaDeJoc[i][j]==1 || TablaDeJoc[i][j]==3)//daca se gaseste un iepure
            {
                Iepuri[nrIepuri].i=i;
                Iepuri[nrIepuri].j=j;
                nrIepuri++;
            }
}

void citireTabla()
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            f>>TablaDeJoc[j][i];
    actualizare_iepuri();
}


void desenareTabla()
{
    int i,j;
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,CYAN);
    for (i=0; i<DIM; i++)
        for (j=0; j<DIM; j++)
            bar3d(stanga+latura*(i),sus+latura*(j),stanga+latura*(i+1),sus+latura*(j+1),2,7);
}

void desenarePiese()
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            if(TablaDeJoc[i][j]==1)//iepure
                readimagefile("rabbit.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            else if(TablaDeJoc[i][j]==-1)//ciuperca
                readimagefile("mushroom.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            else if(TablaDeJoc[i][j]==-2 && TablaDeJoc[i+1][j]==-2 && i+1<DIM)//vulpe indreptata in jos
            {
                readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+2)-2,sus+latura*(j+1)-2);
             //   readimagefile("fox.jpg",stanga+latura*(i+1)+2,sus+latura*(j)+2,stanga+latura*(i+2)-2,sus+latura*(j+1)-2);
            }
            else if(TablaDeJoc[i][j]==-2 && TablaDeJoc[i][j+1]==-2 && j+1<DIM)//vulpe indreptata spre dreapta
            {
                readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+2)-2);
           //     readimagefile("fox.jpg",stanga+latura*(i)+2,sus+latura*(j+1)+2,stanga+latura*(i+1)-2,sus+latura*(j+2)-2);
            }
            else if(TablaDeJoc[i][j]==2)//vizuina
                readimagefile("hole.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            else if(TablaDeJoc[i][j]==3)//iepure in vizuina
                readimagefile("rabbit_in_hole.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
            else if(TablaDeJoc[i][j]!=-2)
                readimagefile("rectangle.jpg",stanga+latura*(i)+2,sus+latura*(j)+2,stanga+latura*(i+1)-2,sus+latura*(j+1)-2);
}

void pozitiaMatrice(int x,int y,pozitieMatrice &poz)
{
    //functia determina pozitia corespunzatoare in matrice pentru coordonatele x si y
    poz.j=(y-sus)/latura;
    poz.i=(x-stanga)/latura;
}

pozitieMatrice PozitiiAccesibile[4];//pot fi doar 4 pozitii accesibile pentru fiecare iepure
unsigned nrMutari=0;

bool MutareValida(pozitieMatrice poz)
{
    //functia determina daca o mutare este valida verificand daca
    //apartine matricei de pozitii accesibile
    for(int i=0; i<nrMutari; i++)
        if(PozitiiAccesibile[i].i==poz.i && PozitiiAccesibile[i].j==poz.j)
            return true;
    return false;
}

void afisareMutariValideIepure(pozitieMatrice poz)
{
    //poz=pozitia iepurelui
    nrMutari=0;
    /*
    k=0 -> N
    k=1 -> S
    k=2 -> E
    k=3 -> V
    */
    for(int k=0; k<4; k++)//parcurge toate directiile in care se poate afla o pozitie accesibila (N,S,E,V)
        if(TablaDeJoc[poz.i+di[k]][poz.j+dj[k]]<0 || TablaDeJoc[poz.i+di[k]][poz.j+dj[k]]==1 || TablaDeJoc[poz.i+di[k]][poz.j+dj[k]]==3)//daca iepurele curent poate sari peste vecinul curent
        {
            pozitieMatrice pozitieCurenta;//se salveaza pozitia curenta
            pozitieCurenta.i=poz.i+di[k];
            pozitieCurenta.j=poz.j+dj[k];
            //se verifica daca iepurele poate sari mai departe
            //iepurele sare in directia k pana cand intalneste un spatiu liber sau o vizuina goala
            while((TablaDeJoc[pozitieCurenta.i][pozitieCurenta.j]!=0 && TablaDeJoc[pozitieCurenta.i][pozitieCurenta.j]!=2) && pozitieCurenta.i<DIM && pozitieCurenta.j<DIM && pozitieCurenta.i>=0 && pozitieCurenta.j>=0)
            {
                pozitieCurenta.i+=di[k];
                pozitieCurenta.j+=dj[k];
            }
            if(pozitieCurenta.i<DIM && pozitieCurenta.j<DIM && pozitieCurenta.i>=0 &&pozitieCurenta.j>=0)//daca nu s-au depasit dimensiunile tablei de joc
                PozitiiAccesibile[nrMutari++]=pozitieCurenta;
        }
    //se marcheaza cu verde pozitiile accesibile pentru iepurele selectat
    for(int k=0; k<nrMutari; k++)
        if(TablaDeJoc[PozitiiAccesibile[k].i][PozitiiAccesibile[k].j]==2)
            readimagefile("green_hole.jpg",stanga+latura*(PozitiiAccesibile[k].i)+2,sus+latura*(PozitiiAccesibile[k].j)+2,stanga+latura*(PozitiiAccesibile[k].i+1)-2,sus+latura*(PozitiiAccesibile[k].j+1)-2);
        else
           readimagefile("green_rectangle.jpg",stanga+latura*(PozitiiAccesibile[k].i)+2,sus+latura*(PozitiiAccesibile[k].j)+2,stanga+latura*(PozitiiAccesibile[k].i+1)-2,sus+latura*(PozitiiAccesibile[k].j+1)-2);
}
bool JocCastigat()
{
    //functia verifica daca toti iepurii se afla in vizuina
    //parcurgand vectorul cu iepuri
    for(int i=0; i<nrIepuri; i++)
        if(TablaDeJoc[Iepuri[i].i][Iepuri[i].j]!=3)
            return false;
    return true;
}

//void afisareMiscariValideVulpe(int xVulpe,yVulpe)


//void mutareVulpe()

void mutareIepureMatrice(pozitieMatrice MutarePosibila,pozitieMatrice pozitieIepure)
{

    if(TablaDeJoc[pozitieIepure.i][pozitieIepure.j]==3)
        TablaDeJoc[pozitieIepure.i][pozitieIepure.j]=2;
    else
        TablaDeJoc[pozitieIepure.i][pozitieIepure.j]=0;
    if(TablaDeJoc[MutarePosibila.i][MutarePosibila.j]==2)
        TablaDeJoc[MutarePosibila.i][MutarePosibila.j]=3;
    else
        TablaDeJoc[MutarePosibila.i][MutarePosibila.j]=1;
}

    ifstream g("mutari.txt");

void rezolvaJoc()
{
    pozitieMatrice pozitieInitiala,pozitieFinala;
    while(g>>pozitieInitiala.i>>pozitieInitiala.j>>pozitieFinala.i>>pozitieFinala.j)
    {
        mutareIepureMatrice(pozitieFinala,pozitieInitiala);
        desenarePiese();
        delay(1500);
    }
}

void mutareIepure()
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        int x,y;
        getmouseclick(WM_LBUTTONDOWN,x,y);
        pozitieMatrice pozitieInitiala;
        pozitiaMatrice(x,y,pozitieInitiala);//se salveaza in pozitieInitiala pozitia in matrice corespunzatoare coordonatelor x si y
        if(TablaDeJoc[pozitieInitiala.i][pozitieInitiala.j]==1 || TablaDeJoc[pozitieInitiala.i][pozitieInitiala.j]==3)//daca user-ul dat click pe un iepure
        {
            int ok=1;
            afisareMutariValideIepure(pozitieInitiala);
            while(ok)//pana cand se selecteaza locul unde va sari iepurele
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    for(int k=0; k<nrMutari; k++)//dispare marcarea cu verde a pozitiilor disponibile
                        if(TablaDeJoc[PozitiiAccesibile[k].i][PozitiiAccesibile[k].j]==2)
                            readimagefile("hole.jpg",stanga+latura*(PozitiiAccesibile[k].i)+2,sus+latura*(PozitiiAccesibile[k].j)+2,stanga+latura*(PozitiiAccesibile[k].i+1)-2,sus+latura*(PozitiiAccesibile[k].j+1)-2);
                        else
                            readimagefile("rectangle.jpg",stanga+latura*(PozitiiAccesibile[k].i)+2,sus+latura*(PozitiiAccesibile[k].j)+2,stanga+latura*(PozitiiAccesibile[k].i+1)-2,sus+latura*(PozitiiAccesibile[k].j+1)-2);
                    int xFinal,yFinal;
                    getmouseclick(WM_LBUTTONDOWN,xFinal,yFinal);
                    pozitieMatrice pozitieFinala;
                    pozitiaMatrice(xFinal,yFinal,pozitieFinala);
                    if((TablaDeJoc[pozitieFinala.i][pozitieFinala.j]==0 || TablaDeJoc[pozitieFinala.i][pozitieFinala.j]==2) && MutareValida(pozitieFinala))//daca pozitia selectata este accesabila
                    {
                        if(TablaDeJoc[pozitieInitiala.i][pozitieInitiala.j]==3)//iepurele dispare din pozitia initiala
                            readimagefile("hole.jpg",stanga+latura*(pozitieInitiala.i)+2,sus+latura*(pozitieInitiala.j)+2,stanga+latura*(pozitieInitiala.i+1)-2,sus+latura*(pozitieInitiala.j+1)-2);
                        else
                            readimagefile("rectangle.jpg",stanga+latura*(pozitieInitiala.i)+2,sus+latura*(pozitieInitiala.j)+2,stanga+latura*(pozitieInitiala.i+1)-2,sus+latura*(pozitieInitiala.j+1)-2);
                        TablaDeJoc[pozitieInitiala.i][pozitieInitiala.j]--;//iepurele in vizuina = 3, vizuina = 3-1; iepure =1,spatiu liber=1-1
                        if(TablaDeJoc[pozitieFinala.i][pozitieFinala.j]==2)//daca iepurele sare intr-o vizuina
                        {
                            TablaDeJoc[pozitieFinala.i][pozitieFinala.j]=3;
                            actualizare_iepuri();
                            readimagefile("rabbit_in_hole.jpg",stanga+latura*(pozitieFinala.i)+2,sus+latura*(pozitieFinala.j)+2,stanga+latura*(pozitieFinala.i+1)-2,sus+latura*(pozitieFinala.j+1)-2);
                            //undo
                            if(JocCastigat())
                            {
                                setcolor(WHITE);
                                settextstyle(BOLD_FONT,HORIZ_DIR,5);
                                outtextxy(200,500,"You Won!");
                                delay(3000);//se asteapta 3 secunda
                                if(f.eof())//daca nu mai sunt matrici in fisierul de intrare se inchide jocul
                                    exit(1);
                                cleardevice();//se goleste ecranul
                                //se citeste o noua matrice si se trece la nivelul urmator
                                readimagefile("wallpaper.jpg",0,0,800,600);
                                citireTabla();
                                desenareTabla();
                                desenarePiese();
                            }
                        }
                        else
                        {
                            //daca iepurele a sarit pe un spatiu liber
                            readimagefile("rabbit.jpg",stanga+latura*(pozitieFinala.i)+2,sus+latura*(pozitieFinala.j)+2,stanga+latura*(pozitieFinala.i+1)-2,sus+latura*(pozitieFinala.j+1)-2);
                            TablaDeJoc[pozitieFinala.i][pozitieFinala.j]=1;
                            actualizare_iepuri();
                        }
                    }
                    ok=0;//s-a selectat pozitia pe care sa sara iepurele
                }
        }
        else if(x>=577 && y>=31 && x<=713 && y<=57)
            rezolvaJoc();
    }
}

void startJoc()
{
    initializare(5);//initializare(dimesiune matrice)
    //generare tabla:
    ofstream file("dim.txt");
    file<<DIM;
    file.close();
    system("GenerareMapa.exe");
    citireTabla();
    ofstream h("tablaCurenta.txt");
    for(int i=0;i<DIM;i++)
        for(int j=0;j<DIM;j++)
            h<<TablaDeJoc[i][j]<<' ';
    h.close();
    delay(100);
    system("RezolvareMapa.exe");
    int x;
    g>>x;
    if(x)
        readimagefile("wallpaper2.jpg",0,0,800,600);
    else
        readimagefile("wallpaper.jpg",0,0,800,600);
    desenareTabla();
    desenarePiese();
    while(1)
        //asteaptaClick();
        mutareIepure();
}

void apasaStart()
{
    readimagefile("start.jpg",0,0,800,600);
    delay(100);
    readimagefile("wall.jpg",0,0,800,600);
    startJoc();
}

void apasaInstructiuni()
{
    readimagefile("instructiuni.jpg",0,0,800,600);
    delay(100);
    readimagefile("wall.jpg",0,0,800,600);
    startJoc();
}

void apasaIesire()
{
    readimagefile("iesire.jpg",0,0,800,600);
    delay(100);
    readimagefile("wall.jpg",0,0,800,600);
    exit(1);
}

void startMenu()
{
    readimagefile("wall.jpg",0,0,800,600);
    while(!ismouseclick(WM_LBUTTONDOWN))
        ;
    int x,y;
    getmouseclick(WM_LBUTTONDOWN,x,y);
    if(x>=276 && x<=533 && y>=272 && y<=332)
        apasaStart();
    else if(x>=166 && x<=630 && y>=368 && y<=426)
        apasaInstructiuni();
    else if(x>=271 && x<=528 && y>=463 && y<=520)
        apasaIesire();
    else startMenu();
}


/*void asteptareClick()
{
    int i,j;
    while(1)
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        int xInit=mousex();
        int yInit=mousey();
        pozitiaMatrice(xInit,yInit,i,j);
        if(TablaDeJoc[i][j]==1 || TablaDeJoc[i][j]==3)
            mutareIepure();
        else if(TablaDeJoc[i][j]==-2)
            mutareVulpe();
    }
}*/

main()
{
   // PlaySound(TEXT("C:\\Users\\HP\\Desktop\\Jump in\\da.wav"),NULL,SND_SYNC);
    initwindow(800,600,"JUMPIN'",true);
    readimagefile("wall.jpg",0,0,800,600);
    startMenu();
    return 0;
}



