/* DE ADAUGAT:
    __________________________________________

    X-fiecare iepure trebuie mutat minim o data
    -existaMutareVulpe()
    -vector de mutari
    -fiecare iepure trebuie sa ajunga pe o pozitie diferita de cea intiala
    __________________________________________
*/
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
using namespace std;
int valoriPosibile[]= {0,-1,2,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,3,3,3,-2}; //22
int TablaDeJoc[10][10];
int di[]= {-1,1,0,0};
int dj[]= {0,0,1,-1};
unsigned nrIepuri,nrVizuine,nrMutariPosibile;
bool existaIepure,iepuriMutati[100];

struct pozitieMatrice
{
    unsigned i,j;
} Iepuri[100],MutariPosibile[1000][2];

void initializareTablaDeJoc(unsigned DIM)
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            TablaDeJoc[i][j]=0;
}

void initializareIepuri(unsigned DIM)
{
    nrIepuri=0;
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            if(TablaDeJoc[i][j]==3)
            {
                Iepuri[nrIepuri].i=i;
                Iepuri[nrIepuri].j=j;
                nrIepuri++;
            }
}


void initializareIepuriMutati()
{
    for(int i=0; i<nrIepuri; i++)
        iepuriMutati[i]=false;
}

void initializare(unsigned DIM)
{
    initializareTablaDeJoc(DIM);
    nrVizuine=0;
    nrIepuri=0;
}

void asezareVulpe(int i,int j,unsigned DIM)
{
    int ok=rand()%2;
    if(ok==0 && j+1<DIM && TablaDeJoc[i][j+1]!=-2)
        TablaDeJoc[i][j+1]=-2;
    else if(i+1<DIM && TablaDeJoc[i+1][j]!=-2)
        TablaDeJoc[i+1][j]=-2;
    else
        while(TablaDeJoc[i][j]==-2)
            TablaDeJoc[i][j]=valoriPosibile[rand()%22];
}
void generareMatriceRezolvata(unsigned DIM)
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            if(TablaDeJoc[i][j]!=-2) //daca nu a fost deja asezata o vulpe acolo
            {
                TablaDeJoc[i][j]=valoriPosibile[rand()%22];
                if(TablaDeJoc[i][j]==-2)
                    asezareVulpe(i,j,DIM);
                else if(TablaDeJoc[i][j]==3)
                {
                    nrIepuri++;
                    nrVizuine++;
                }
                else if(TablaDeJoc[i][j]==2)
                    nrVizuine++;
            }
    if(nrIepuri==0 || nrIepuri>DIM)
    {
        initializare(DIM);
        generareMatriceRezolvata(DIM);
    }

    initializareIepuri(DIM);
    initializareIepuriMutati();
}

pozitieMatrice mutareValida(pozitieMatrice iepureCurent,int i,unsigned DIM)
{
    if(TablaDeJoc[iepureCurent.i+di[i]][iepureCurent.j+dj[i]]<0 || TablaDeJoc[iepureCurent.i+di[i]][iepureCurent.j+dj[i]]==3 || TablaDeJoc[iepureCurent.i+di[i]][iepureCurent.j+dj[i]]==1)// || existaMutareVulpe())
    {
        pozitieMatrice pozMutareCurenta;
        pozMutareCurenta.i=iepureCurent.i+di[i];
        pozMutareCurenta.j=iepureCurent.j+dj[i];//pozMutareCurenta retine adresa primului obstacol
        while((TablaDeJoc[pozMutareCurenta.i][pozMutareCurenta.j]!=0 && TablaDeJoc[pozMutareCurenta.i][pozMutareCurenta.j]!=2) && pozMutareCurenta.i<DIM && pozMutareCurenta.j<DIM && pozMutareCurenta.i>=0 && pozMutareCurenta.j>=0)//atata timp cat se intalnesc obstacole si nu se depaseste dimensiunea matricei
        {
            pozMutareCurenta.i+=di[i];
            pozMutareCurenta.j+=dj[i];
        }
        if(pozMutareCurenta.i<DIM && pozMutareCurenta.j<DIM && pozMutareCurenta.i>=0 && pozMutareCurenta.j>=0)//altfel spus: pozitia curenta nu mai reprezinta un obstacol peste care iepurele sa poata sari
            return pozMutareCurenta;//returneaza prima pozitie la care iepurele curent poate sari
    }
    pozitieMatrice pozMutareCurenta;
    pozMutareCurenta.i=DIM;
    pozMutareCurenta.j=DIM;
    return pozMutareCurenta;
}

void mutareIepure(pozitieMatrice MutarePosibila,pozitieMatrice pozitieIepure)
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

void actualizareIepuri(int i,pozitieMatrice pozitie)
{
    Iepuri[i].i=pozitie.i;
    Iepuri[i].j=pozitie.j;
}

bool auFostMajoritateaIepurilorMutati(unsigned DIM)
{
    unsigned contor=0;
    for(int i=0; i<nrIepuri; i++)
        if(iepuriMutati[i]==false)
            contor++;
    if(contor>DIM/2)
        return false;
    return true;
}

bool existaIepuriAfaraDinVizuina()
{
    unsigned nrIepuriLiberi=0;
    for(int i=0; i<nrIepuri; i++)
        if(TablaDeJoc[Iepuri[i].i][Iepuri[i].j]==1)
            nrIepuriLiberi++;
    if(nrIepuriLiberi)
        return true;
    return false;
}

void actualizareMutari(unsigned DIM)
{
    nrMutariPosibile=0;
    pozitieMatrice pozCurenta;
    for(int i=0; i<nrIepuri; i++)
        for(int k=0; k<4; k++)
        {
            pozCurenta=mutareValida(Iepuri[i],k,DIM);
            if(pozCurenta.i!=DIM && pozCurenta.j!=DIM)
            {
                MutariPosibile[nrMutariPosibile][0]=Iepuri[i];
                MutariPosibile[nrMutariPosibile++][1]=pozCurenta;
            }
        }
}

unsigned getNumarIepure(pozitieMatrice pozitieIepure,unsigned DIM)
{
    for(int i=0; i<nrIepuri; i++)
        if(Iepuri[i].i==pozitieIepure.i && Iepuri[i].j==pozitieIepure.j)
            return i;
}

bool matriceNerezolvata(unsigned DIM)
{
    actualizareMutari(DIM);
    unsigned numarMaximDeMutari=rand()%20+5,iepureCurent;
    pozitieMatrice pozitieIepure,pozitieFinala;
    for(int i=0; i<numarMaximDeMutari && nrMutariPosibile; i++)
    {
        int nrMutare=rand()%nrMutariPosibile;//se alege o mutare random
        pozitieIepure=MutariPosibile[nrMutare][0];
        unsigned numarIepure=getNumarIepure(pozitieIepure,DIM);
        pozitieFinala=MutariPosibile[nrMutare][1];
        mutareIepure(pozitieFinala,pozitieIepure);
        iepuriMutati[numarIepure]=true;
        actualizareIepuri(numarIepure,pozitieFinala);
        actualizareMutari(DIM);
    }
    if(existaIepuriAfaraDinVizuina() && auFostMajoritateaIepurilorMutati(DIM))
        return true;
    else
        return false;
}

void outMape(unsigned DIM)
{
    ofstream f ("tabla.txt");
    srand(time(NULL));
    unsigned contor=0;
    for(int k=0; k<100;k++)
    {
        initializare(DIM);
        generareMatriceRezolvata(DIM);
        if(matriceNerezolvata(DIM))
        {
            contor++;
            for(int i=0; i<DIM; i++)
            {
                for(int j=0; j<DIM; j++)
                    f<<TablaDeJoc[i][j]<<' ';
                f<<endl;
            }
        }
    }
    cout<<contor;
}

main()
{
    ifstream f ("dim.txt");
    unsigned DIM;
    f>>DIM;
    outMape(DIM);
}
