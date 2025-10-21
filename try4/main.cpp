#include <iostream>
#include <cmath>
#include <list>
#include <fstream>

using namespace std;

// struktura kom�rki
struct Node {
    int x,y; //wsp�rz�dne Noda na tablicy
    double g,h,f; //do heurestyki
    Node* rodzic;  //wska�nik na rodzica

    Node(int x_, int y_, double g_, double h_,Node* rodzic_ = nullptr) //konstruktor dla Noda
        : x(x_), y(y_), g(g_), h(h_), f(g_ + h_), rodzic(rodzic_) {}
};

// wyznaczanie odleg�o�ci do punktu ko�cowego
double H_Euklidesowa(int x1,int y1 , int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

int main()
{
    //zdefiniowanie rozmiaru tablicy
   const int ROW=20;
   const int COL=20;

   int tablica[ROW][COL];

   //kolejno�� sprawdzania s�siaduj�cych kom�rek g�ra , d� , lewo , prawo
   // koszt ruchu wynosi 1
   int k_sprawdzanie[4][2] = {
    {0, 1},   // g�ra (y + 1)
    {0, -1},  // d� (y - 1)
    {-1, 0},  // lewo (x - 1)
    {1, 0}    // prawo (x + 1)
   };

   // lista otwarta , zawiera pola kt�re rozwa�ane s� pod ekspansje
    list<Node*> lotwarta;

    // lista zamknieta , zawiera pola ju� odwiedzone
    list<Node*> lzamknieta;

    // dodanie punktu startowego do listy zamkni�tej
    Node* pstart = new Node(0, ROW-1, 0.0, 0.0, nullptr);
    lzamknieta.push_back(pstart);

    Node* pkoniec = new Node(COL-1, 0, 0.0, 0.0, nullptr);

    ifstream plik("grid.txt");
    if(!plik){
        cout << "b��d w dost�pie do pliku" <<endl;
        return -1;
    }

    for(int i=ROW-1;i>=0;i--){
        for(int j=0;j< COL;j++){
            if(!(plik>>tablica[i][j])){
                cout<< "nie mo�na by�o odczyta� kom�rki z pliku grid.txt"<<i+1<<endl;
                return 1;
            }
        }
    }
    plik.close();


    for (int i = ROW-1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            cout << tablica[i][j] << " ";
        }
        cout << endl;
    }

    return 0;

}
