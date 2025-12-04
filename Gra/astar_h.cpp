#ifndef ASTAR_H
#define ASTAR_H
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

struct komorka {
    int x;
    int y;
    double g;
    double h;
    double f;
    komorka* rodzic;
};

double hEuklidesowa(int x1,int y1, int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

bool czyWLiscie(komorka* lista[], int ilosc, int x, int y){
    for(int i=0; i < ilosc; i++){
        if(lista[i]->x == x && lista[i]->y == y){
            return true;
        }
    }
    return false;
}

int znajdzIndeks(komorka* lista[], int ilosc, int x , int y){
    for(int i=0; i < ilosc; i++){
        if(lista[i]->x == x && lista[i]->y == y){
            return i;
        }
    }
    return -1;
}

void uzupelnijTabele(int table[20][20], string nazwa){
    ifstream plik(nazwa);
    if(!plik){
        int domyslna[20][20] ={0};
        for(int i=0;i<20;i++)
            for(int j=0;j<20;j++)
                table[i][j]=0;
    }
    else {
        for(int i=19;i>=0;i--){
            for(int j=0;j<20;j++){
                plik >> table[i][j];
            }
        }
        plik.close();
    }
}

komorka* znajdzRuchAStar(int tablica[20][20], int sx, int sy, int tx, int ty){
    komorka* lOtwarta[400]; int otwartaIlosc=0;
    komorka* lZamknieta[400]; int zamknietaIlosc=0;

    komorka* start = new komorka{sx,sy,0.0,hEuklidesowa(sx,sy,tx,ty),0,nullptr};
    start->f = start->g + start->h;

    komorka* koniec = new komorka{tx,ty,0,0,0,nullptr};

    lOtwarta[otwartaIlosc++] = start;

    int kSprawdzanie[4][2] = {{0,1},{0,-1},{-1,0},{1,0}};

    while(otwartaIlosc > 0){
        int indeksMin = 0;
        for(int i=1;i<otwartaIlosc;i++)
            if(lOtwarta[i]->f < lOtwarta[indeksMin]->f)
                indeksMin = i;

        komorka* akt = lOtwarta[indeksMin];
        for(int i=indeksMin; i < otwartaIlosc-1; i++)
            lOtwarta[i] = lOtwarta[i+1];
        otwartaIlosc--;

        lZamknieta[zamknietaIlosc++] = akt;

        if(akt->x == koniec->x && akt->y == koniec->y){
            komorka* sciezka = akt;
            while(sciezka->rodzic && sciezka->rodzic->rodzic)
                sciezka = sciezka->rodzic;
            return sciezka;
        }

        for(int i=0;i<4;i++){
            int nx = akt->x + kSprawdzanie[i][0];
            int ny = akt->y + kSprawdzanie[i][1];

            if(nx<0 || ny<0 || nx>=20 || ny>=20) continue;
            if(tablica[ny][nx] == 5) continue;
            if(czyWLiscie(lZamknieta,zamknietaIlosc,nx,ny)) continue;

            double g = akt->g + 1;
            double h = hEuklidesowa(nx,ny,tx,ty);

            int idx = znajdzIndeks(lOtwarta, otwartaIlosc, nx, ny);
            if(idx != -1){
                if(g < lOtwarta[idx]->g){
                    lOtwarta[idx]->g = g;
                    lOtwarta[idx]->f = g + lOtwarta[idx]->h;
                    lOtwarta[idx]->rodzic = akt;
                }
            }
            else{
                komorka* nowa = new komorka{nx,ny,g,h,g+h,akt};
                lOtwarta[otwartaIlosc++] = nowa;
            }
        }
    }

    return nullptr;
}

#endif