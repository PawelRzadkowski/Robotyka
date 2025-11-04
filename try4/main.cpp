#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//struktura wezla
struct komorka {
    int x,y;
    double g,h,f;
    komorka* rodzic;

    //konstruktor dla wezla bezargumentowy
    komorka() : x(0), y(0), g(0), h(0), f(0), rodzic(nullptr) {}
    //konstruktor dla wezla z argumentami
    komorka(int x_, int y_, double g_, double h_,komorka* rodzic_ = nullptr)
        : x(x_), y(y_), g(g_), h(h_), f(g_ + h_), rodzic(rodzic_) {}
};

// wyznaczanie odleglosci od punktu koncowego
double H_Euklidesowa(int x1,int y1 , int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

// do sprawdzania czy wezel jest w liscie
    bool czyWLiście(komorka lista[], int ilosc, int x, int y) {
        for (int i = 0; i < ilosc; i++) {
            if (lista[i].x == x && lista[i].y == y) {
                return true;
            }
        }
        return false;
    }

    // do znajdowania indeksu
    int znajdzIndeks(komorka lista[], int ilosc, int x, int y) {
        for (int i = 0; i < ilosc; i++) {
            if (lista[i].x == x && lista[i].y == y) {
                return i;
            }
        }
        return -1;
    }

    // z pliku grid.txt wczytujemy komorki i wstawiamy je do tablicy
    // wpisywane jest tak , jakby tablica byla odwrocona inaczej punkt startowy znajdowalby sie w miejsciu (0,19) , a koncowy w (19,0)
    // w ten sposob zadeklarowalem punkt startowy w punkcie (0,0) a koncowy w punkcie(19,19)
    void uzupelnijTabele(int table[20][20],string filename){
        ifstream plik(filename);
        if (!plik) {
            cout << "Blad przy otwieraniu pliku" << endl;
            return;
        }

        for (int i = 19; i >= 0; i--) {
            for (int j = 0; j < 20; j++) {
                if (!(plik >> table[i][j])) {
                    cout << "Nie mozna odczytac komorki z pliku" << endl;
                }
            }
        }
        plik.close();
    }


int main() {

    //Zdefiniowanie rozmiaru tablicy ktora przedstawia grida , do ktorej pozniej beda wstawiane komorki
    const int ROW = 20;
    const int COL = 20;
    int tablica[ROW][COL];


    //uzupelnienie tablicy
    uzupelnijTabele(tablica,"grid.txt");

    //kolejnosc sprawdzania sasiadow gora , dol ,lewo , prawo
    //zaczynamy w w punkcie lewo dol , ktory w tablicy ma wspolrzedne [0][19] , wiec aby isc na mapie do gory musimy zmniejszyc wspolrzedna y
    int k_sprawdzanie[4][2] = {
        {0, 1},   // góra
        {0, -1},    // dół
        {-1, 0},   // lewo
        {1, 0}     // prawo
    };

    // lista otwarta , przechowuje kratki ktore sa rozwazane pod ekspansje
    komorka lOtwarta[ROW * COL];
    int otwartaIlosc = 0;

    //lista zamknieta , przechowuje kratki ktore juz byly odwiedzone , usuniete z listy otwartej
    komorka lZamknieta[ROW * COL];
    int zamknietaIlosc = 0;



    // tablica jest juz odwrocona , stworzenie punktu startowego wspol(0,0) i punktu koncowego wspol(19,19) , zaczynamy od dolu od lewej strony
    // idziemy prawo gora
    komorka pstart(0, 0, 0.0, H_Euklidesowa(0, 0, 19, 19));
    komorka pkoniec(19, 19, 0.0, 0.0);

    // dodanie pstart do listy otwartej
    lOtwarta[0] = pstart;
    otwartaIlosc++;

    bool celOsiagniety = false;
    komorka* cel = nullptr;


    //algorytm

    // gdy w liscie otwartej znajduja sie elementy
    while (otwartaIlosc > 0) {

        // wybranie z listy otwartej elementu z najmiejsza odlegloscia od punktu koncowego
        int indeksMin_f = 0;
        for (int i = 1; i < otwartaIlosc; i++) {
            if (lOtwarta[i].f < lOtwarta[indeksMin_f].f) {
                indeksMin_f = i;
            }
        }

        komorka aktualna_komorka = lOtwarta[indeksMin_f];

        // usuwanie elementu z listy otwartej
        for (int i = indeksMin_f; i < otwartaIlosc - 1; i++) {
            lOtwarta[i] = lOtwarta[i + 1];
        }
        otwartaIlosc--;

        // dodanie elementu do listy zamknietej
        lZamknieta[zamknietaIlosc] = aktualna_komorka;
        zamknietaIlosc++;

        // sprawdza czy wspolrzedne aktualnej komorki jest rowne pkoniec
        // jeśli algorytm dotarł do celu , oznacza ścieżkę w tablicy wartością 3
        // przechodząc wstecz przez wskazniki na rodziców od punktu końcowego do startowego
        if (aktualna_komorka.x == pkoniec.x && aktualna_komorka.y == pkoniec.y) {
            celOsiagniety = true;
            komorka* sciezka = &aktualna_komorka;
            while (sciezka != nullptr) {
                tablica[(*sciezka).y][(*sciezka).x] = 3;
                sciezka = (*sciezka).rodzic;
            }
            break;
        }

        // Sprawdzanie sąsiadów
        for (int i = 0; i < 4; i++) {
            int sasiad_x = aktualna_komorka.x + k_sprawdzanie[i][0];
            int sasiad_y = aktualna_komorka.y + k_sprawdzanie[i][1];

            //czy sasiad wychodzi poza granice grida , jesli tak jest pomijany
            if (sasiad_x < 0 || sasiad_y < 0 || sasiad_x >= 20 || sasiad_y >=20 )
                {
                    continue;
                }



            // jesli wezel jest przeszkoda to jest pomijana
            if (tablica[sasiad_y][sasiad_x] == 5)
            {
                continue;
            }

            // jesli wezel jest w liscie zamknietej to jest pomijany
            if (czyWLiście(lZamknieta, zamknietaIlosc, sasiad_x, sasiad_y))
            {
                continue;
            }

            //nowy koszt g i h dla sasiada
            double g = aktualna_komorka.g + 1.0;
            double h = H_Euklidesowa(sasiad_x, sasiad_y, pkoniec.x, pkoniec.y);

            // sprawdza czy wezel jest w liscie otwartej
            int indeksOtwarty = znajdzIndeks(lOtwarta, otwartaIlosc, sasiad_x, sasiad_y);

            if (indeksOtwarty != -1) {
                // gdy sciezka jest lepsza
                if (g < lOtwarta[indeksOtwarty].g) {
                    lOtwarta[indeksOtwarty].g = g;
                    lOtwarta[indeksOtwarty].f = g + lOtwarta[indeksOtwarty].h;
                }
            } else {
                // jesli wezel nie znajduje sie na liscie otwartej , to jest tam dodawany
                if (otwartaIlosc < ROW * COL) {
                    lOtwarta[otwartaIlosc++] = komorka(sasiad_x, sasiad_y, g, h, new komorka(aktualna_komorka));
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
