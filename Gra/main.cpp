#include <iostream>
#include <conio.h>
#include <windows.h>
#include "astar_h.cpp"

using namespace std;

#include <windows.h>

void kolor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clearScreenNoBlink() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);
}
void rysujMapeKolorowo(int mapa[20][20]) {
    for (int y = 19; y >= 0; y--) {
        for (int x = 0; x < 20; x++) {

            int v = mapa[y][x];

            if (v == 5) {                 // ściana
                kolor(8);
                cout << "##";
            }
            else if (v == 2) {            // gracz
                kolor(10);
                cout << "@ ";
            }
            else if (v == 8) {            // przeciwnik
                kolor(12);
                cout << "& ";
            }
            else if (v == 3) {            // ścieżka A*
                kolor(14);
                cout << "* ";
            }
            else {                        // puste
                kolor(7);
                cout << " .";
            }

            kolor(7);
        }
        cout << "\n";
    }
}

int main(){
    int mapa[20][20];
    uzupelnijTabele(mapa, "grid.txt");

    int gx = 0, gy = 0;
    int ex = 19, ey = 19;

    mapa[gy][gx] = 2;
    mapa[ey][ex] = 8;

    while(true){

    clearScreenNoBlink();

    rysujMapeKolorowo(mapa);

    cout << "Sterowanie: WASD. Uciekaj przed przeciwnikiem!" << endl;

    char c = _getch();
    int nx=gx, ny=gy;

    if(c=='w') ny++;
    if(c=='s') ny--;
    if(c=='a') nx--;
    if(c=='d') nx++;

    if(nx>=0 && nx<20 && ny>=0 && ny<20 && mapa[ny][nx] != 5){
        mapa[gy][gx] = 0;
        gx = nx; gy = ny;
        mapa[gy][gx] = 2;
    }

    int kopia[20][20];
    for(int y=0;y<20;y++)
        for(int x=0;x<20;x++)
            kopia[y][x] = mapa[y][x];

    komorka* nastepny = znajdzRuchAStar(kopia, ex, ey, gx, gy);
    if(nastepny){
        mapa[ey][ex] = 0;
        ex = nastepny->x;
        ey = nastepny->y;
        mapa[ey][ex] = 8;
    }

    if(ex == gx && ey == gy){
        clearScreenNoBlink();
        cout << "ZLAPANO CIE! KONIEC GRY." << endl;
        break;
    }

}
}
