#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//struktura wezla
struct Node {
    int x,y;
    double g,h,f;
    Node* rodzic;

    //konstruktor dla wezla bezargumentowy
    Node() : x(0), y(0), g(0), h(0), f(0), rodzic(nullptr) {}
    //konstruktor dla wezla z argumentami
    Node(int x_, int y_, double g_, double h_,Node* rodzic_ = nullptr)
        : x(x_), y(y_), g(g_), h(h_), f(g_ + h_), rodzic(rodzic_) {}
};

// wyznaczanie odleglosci od punktu koncowego
double H_Euklidesowa(int x1,int y1 , int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

// do sprawdzania czy wezel jest w liscie
    bool czyWLiście(Node lista[], int count, int x, int y) {
        for (int i = 0; i < count; i++) {
            if (lista[i].x == x && lista[i].y == y) {
                return true;
            }
        }
        return false;
    }

    // do znajdowania indeksu
    int znajdzIndeks(Node lista[], int count, int x, int y) {
        for (int i = 0; i < count; i++) {
            if (lista[i].x == x && lista[i].y == y) {
                return i;
            }
        }
        return -1;
    }


int main() {

    //Zdefiniowanie rozmiaru tablicy ktora przedstawia grida , do ktorej pozniej beda wstawiane komorki
    const int ROW = 20;
    const int COL = 20;
    int tablica[ROW][COL];

    //kolejnosc sprawdzania sasiadow gora , dol ,lewo , prawo
    //zaczynamy w w punkcie lewo dol , ktory w tablicy ma wspolrzedne [0][19] , wiec aby isc na mapie do gory musimy zmniejszyc wspolrzedna y
    int k_sprawdzanie[4][2] = {
        {0, -1},   // góra
        {0, 1},    // dół
        {-1, 0},   // lewo
        {1, 0}     // prawo
    };

    // lista otwarta , przechowuje kratki ktore sa rozwazane pod ekspansje
    Node lOtwarta[ROW * COL];
    int otwartaCount = 0;

    //lista zamknieta , przechowuje kratki ktore juz byly odwiedzone , usuniete z listy otwartej
    Node lZamknieta[ROW * COL];
    int zamknietaCount = 0;

    // z pliku grid.txt wczytujemy komorki i wstawiamy je do tablicy
    ifstream plik("grid.txt");
    if (!plik) {
        cout << "Blad w dostepie do pliku" << endl;
        return -1;
    }

    for (int i = ROW - 1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            if (!(plik >> tablica[i][j])) {
                cout << "Nie mozna odczytac komorki z pliku" << endl;
                return -1;
            }
        }
    }
    plik.close();


    //stworzenie punktu startowego wspol(0,0) i punktu koncowego wspol(19,19) , zaczynamy od dolu od lewej strony
    Node pstart(0, 0, 0.0, H_Euklidesowa(0, 0, COL-1, ROW-1));
    Node pkoniec(COL-1, ROW-1, 0.0, 0.0);

    // dodanie pstart do listy otwartej
    lOtwarta[otwartaCount++] = pstart;

    bool celOsiagniety = false;
    Node* celNode = nullptr;


    //algorytm

    // gdy w liscie otwartej znajduja sie elementy
    while (otwartaCount > 0) {

        // wybranie z listy otwartej elementu z najmiejsza odlegloscia od punktu koncowego
        int indeksMin_f = 0;
        for (int i = 1; i < otwartaCount; i++) {
            if (lOtwarta[i].f < lOtwarta[indeksMin_f].f) {
                indeksMin_f = i;
            }
        }

        Node aktualny_Node = lOtwarta[indeksMin_f];

        // usuwanie elementu z listy otwartej
        for (int i = indeksMin_f; i < otwartaCount - 1; i++) {
            lOtwarta[i] = lOtwarta[i + 1];
        }
        otwartaCount--;

        // dodanie elementu do listy zamknietej
        lZamknieta[zamknietaCount++] = aktualny_Node;

        // sprawdza czy wspolrzedne aktualnego noda jest rowne pkoniec
        // jeśli algorytm dotarł do celu , oznacza ścieżkę w tablicy wartością 3
        // przechodząc wstecz przez węzły rodziców od punktu końcowego do startowego
        if (aktualny_Node.x == pkoniec.x && aktualny_Node.y == pkoniec.y) {
            celOsiagniety = true;
            Node* sciezka = &aktualny_Node;
            while (sciezka != nullptr) {
                tablica[sciezka->y][sciezka->x] = 3;
                sciezka = sciezka->rodzic;
            }
            break;
        }

        // Sprawdzanie sąsiadów
        for (int i = 0; i < 4; i++) {
            int sasiad_x = aktualny_Node.x + k_sprawdzanie[i][0];
            int sasiad_y = aktualny_Node.y + k_sprawdzanie[i][1];

            //czy sasiad wychodzi poza granice grida , jesli tak jest pomijany
            if (sasiad_x < 0 || sasiad_y < 0 || sasiad_x >= 20 || sasiad_y >=20 )
                {
                    continue;
                }



            // jesli wezel jest przeszkoda to jest pomijana
            if (tablica[ny][nx] == 5)
            {
                continue;
            }

            // jesli wezel jest w liscie zamknietej to jest pomijany
            if (czyWLiście(lZamknieta, zamknietaCount, nx, ny))
            {
                continue;
            }

            //nowy koszt g i h dla sasiada
            double g = aktualny_Node.g + 1.0;
            double h = H_Euklidesowa(nx, ny, pkoniec.x, pkoniec.y);

            // sprawdza czy wezel jest w liscie otwartej
            int indeksOtwarty = znajdzIndeks(lOtwarta, otwartaCount, nx, ny);

            if (indeksOtwarty != -1) {
                // gdy sciezka jest lepsza
                if (g < lOtwarta[indeksOtwarty].g) {
                    lOtwarta[indeksOtwarty].g = g;
                    lOtwarta[indeksOtwarty].f = g + lOtwarta[indeksOtwarty].h;
                }
            } else {
                // jesli wezel nie znajduje sie na liscie otwartej , to jest tam dodawany
                if (otwartaCount < ROW * COL) {
                    lOtwarta[otwartaCount++] = Node(nx, ny, g, h, new Node(aktualny_Node));
                }
            }
        }
    }

    // wyswietlenie tablicy z wyznaczona trasa
    for (int i = ROW - 1; i >= 0; i--) {
        for (int j = 0; j < COL; j++) {
            cout << tablica[i][j] << " ";
        }
        cout << endl;
    }

    // komunikat gdyby punkt koncowy nie bylby osiagalny
    if (!celOsiagniety) {
        cout << "Nie mozna dotrzec do celu" << endl;
        return -1;
    }

    return 0;
}
