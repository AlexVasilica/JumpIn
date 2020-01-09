#include<iostream>
#include<cstdlib>
#include<fstream>
#include<time.h>
#include<cstring>
#include<stack>
using namespace std;

int TablaDeJoc[10][10];
int di[]= {-1,1,0,0};
int dj[]= {0,0,1,-1};
unsigned nrIepuri=0;
char Results[10000][1000];
unsigned nrRes=0,nrMutari=0;

struct pozitieMatrice
{
    unsigned i,j;
} Iepuri[100];

struct mutare
{
    pozitieMatrice init,fin;
};
stack <mutare> mutari;

ifstream f ("tabla.txt");

void citireTabla(unsigned DIM)
{

    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            f>>TablaDeJoc[j][i];
}

void initializareIepuri(unsigned DIM)
{
    nrIepuri=0;
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            if(TablaDeJoc[i][j]==1 || TablaDeJoc[i][j]==3)
            {
                Iepuri[nrIepuri].i=i;
                Iepuri[nrIepuri].j=j;
                nrIepuri++;
            }
}

void actualizareIepuri(int i,pozitieMatrice pozitie)
{
    Iepuri[i].i=pozitie.i;
    Iepuri[i].j=pozitie.j;
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

bool esteSolutie()
{
    //functia parcurge tabloul cu iepuri si verifica daca toti iepurii se afla in vizuina
    for(int i=0; i<nrIepuri; i++)
        if(TablaDeJoc[Iepuri[i].i][Iepuri[i].j]!=3)
            return false;
    return true;
}

void mutareIepure(pozitieMatrice MutarePosibila,int i)
{

    if(TablaDeJoc[Iepuri[i].i][Iepuri[i].j]==3)
        TablaDeJoc[Iepuri[i].i][Iepuri[i].j]=2;
    else
        TablaDeJoc[Iepuri[i].i][Iepuri[i].j]=0;
    if(TablaDeJoc[MutarePosibila.i][MutarePosibila.j]==2)
        TablaDeJoc[MutarePosibila.i][MutarePosibila.j]=3;
    else
        TablaDeJoc[MutarePosibila.i][MutarePosibila.j]=1;
}

void salvareTablaDeJocTemp(int TablaDeJocTemp[][10],unsigned DIM)
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            TablaDeJocTemp[i][j]=TablaDeJoc[i][j];
}

void stergereMutari(int TablaDeJocTemp[][10],unsigned DIM)
{
    for(int i=0; i<DIM; i++)
        for(int j=0; j<DIM; j++)
            TablaDeJoc[i][j]=TablaDeJocTemp[i][j];
}

mutare mutareAnterioara;

void salvareMutare(char Result[],int i,pozitieMatrice pozitieInitiala,pozitieMatrice pozitieFinala,unsigned DIM)
{
    char s[100];
    for(int x=0; x<DIM; x++)
        for(int y=0; y<DIM; y++)
        {
            itoa(TablaDeJoc[x][y],s,10);
            strcat(Result,s);
        }

    itoa(i,s,10);
    strcat(Result,s);
    itoa(pozitieInitiala.i,s,10);
    strcat(Result,s);
    itoa(pozitieInitiala.j,s,10);
    strcat(Result,s);
    itoa(pozitieFinala.i,s,10);
    strcat(Result,s);
    itoa(pozitieFinala.j,s,10);
    strcat(Result,s);
}

int nrApeluriRecursive=0;
unsigned contor=0;

bool Rezolva(unsigned DIM)
{
    nrApeluriRecursive++;
    if(nrApeluriRecursive>1000)
        return false;
    if(esteSolutie() &&mutari.size()<=20 )
       return true;
    for(int i=0; i<nrIepuri; i++)
        for(int j=0; j<4; j++)
        {
            pozitieMatrice pozMutare=mutareValida(Iepuri[i],j,DIM);
            pozitieMatrice pozInitiala=Iepuri[i];
            if(!(mutareAnterioara.init.i==pozMutare.i && mutareAnterioara.init.j==pozMutare.j && mutareAnterioara.fin.i==pozInitiala.i && mutareAnterioara.fin.j==pozInitiala.j))
                if(pozMutare.i!=DIM && pozMutare.j!=DIM)
                {
                    int TablaDeJocTemp[10][10];
                    char Result[1000]="";
                    salvareTablaDeJocTemp(TablaDeJocTemp,DIM);
                    mutareAnterioara.init=pozInitiala;
                    mutareAnterioara.fin=pozMutare;
                    mutari.push(mutareAnterioara);
                    mutareIepure(pozMutare,i);
                    nrMutari++;
                    salvareMutare(Result,i,pozInitiala,pozMutare,DIM);
                    bool wasnotbefore=true;
                    char s[100];
                    for(int k=0; k<nrRes; k++)
                        if(strcmp(Result,Results[k])==0)
                            wasnotbefore=false;
                    if(wasnotbefore)
                    {
                        actualizareIepuri(i,pozMutare);
                        strcpy(Results[nrRes++],Result);
                        if(Rezolva(DIM)==true)
                            return true;
                        else
                        {
                            stergereMutari(TablaDeJocTemp,DIM);
                            mutari.pop();
                            actualizareIepuri(i,pozInitiala);
                        }
                    }
                    else
                    {
                        stergereMutari(TablaDeJocTemp,DIM);
                        mutari.pop();
                        actualizareIepuri(i,pozInitiala);
                    }
                }
            }
    return false;
}

void reverseStack()
{
    stack<mutare> mutariAux;
    while(!mutari.empty())
    {
        mutariAux.push(mutari.top());
        mutari.pop();
    }
    swap(mutari,mutariAux);
}
ofstream h ("mutari.txt");
void outMutari()
{
    reverseStack();
    mutare mutareCurenta;

    while(!mutari.empty())
    {
        mutareCurenta=mutari.top();
        h<<mutareCurenta.init.i<<' '<<mutareCurenta.init.j<<' '<<mutareCurenta.fin.i<<' '<<mutareCurenta.fin.j<<endl;
        mutari.pop();
    }
}

main()
{
    ifstream g("dim.txt");
    unsigned DIM;
    g>>DIM;
    citireTabla(DIM);
    initializareIepuri(DIM);
    mutareAnterioara.fin.i=5;
    mutareAnterioara.fin.j=5;
    mutareAnterioara.init.i=5;
    mutareAnterioara.init.j=5;
    if(Rezolva(DIM))
       {
           h<<1<<' ';
           outMutari();
       }
    else
        h<<0<<' ';

}
