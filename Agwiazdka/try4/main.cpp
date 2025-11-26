#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//struktura wezla
struct komorka {
    int x,y;
    double g,h,f;
    komorka* rodzic;

    //konstruktor dla wezla z argumentami , uzywany przy tworzeniu punktu startowego i punktu koncowego
    komorka(int x_, int y_, double g_, double h_,komorka* rodzic_ = nullptr)
        : x(x_), y(y_), g(g_), h(h_), f(g_ + h_), rodzic(rodzic_) {}
};

// wyznaczanie odleglosci od punktu koncowego
double hEuklidesowa(int x1,int y1 , int x2, int y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

// do sprawdzania czy wezel jest w liscie
    bool czyWLiście(komorka* lista[], int ilosc, int x, int y) {
        for (int i = 0; i < ilosc; i++) {
            if (lista[i]->x == x && lista[i]->y == y) {
                return true;
            }
        }
        return false;
    }

    // do znajdowania indeksu
    int znajdzIndeks(komorka* lista[], int ilosc, int x, int y) {
        for (int i = 0; i < ilosc; i++) {
            if (lista[i]->x == x && lista[i]->y == y) {
                return i;
            }
        }
        return -1;
    }

    // z pliku grid.txt wczytujemy komorki i wstawiamy je do tablicy
    // wpisywane jest tak , jakby tablica byla odwrocona inaczej punkt startowy znajdowalby sie w miejsciu (0,19) , a koncowy w (19,0)
    // zadeklarowalem punkt startowy w punkcie (0,0) a koncowy w punkcie(19,19)
    void uzupelnijTabele(int table[20][20],string nazwa){
    ifstream plik(nazwa);

    if (!plik) {
        cout << "Blad przy otwieraniu pliku , wczytana tablica domyslna" << endl;

        int domyslna[20][20] =
        {
            {0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,5,0,5,0,5,5,5,5,5,5},
            {0,0,0,0,0,0,5,0,0,0,5,0,5,0,0,0,5,0,0,0},
            {0,0,0,0,0,0,5,0,0,0,5,0,5,0,0,0,5,0,0,0},
            {0,0,0,0,0,0,5,0,0,0,5,0,5,0,0,0,5,5,5,5},
            {0,0,0,0,5,0,5,0,0,0,5,0,5,0,0,0,5,5,5,0},
            {0,0,0,0,5,0,5,5,0,0,5,0,5,0,0,0,5,0,5,5},
            {0,0,0,0,5,0,5,5,0,0,5,0,5,0,0,0,5,0,0,0},
            {0,0,0,5,5,5,5,5,0,0,5,0,0,0,0,0,5,0,0,0},
            {0,0,0,0,5,0,0,5,0,0,0,0,0,0,5,5,5,5,5,5},
            {0,0,0,0,5,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0},
            {0,0,0,0,5,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,5,0,0,0,0,5,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0}
        };

        for(int i=0;i<20;i++)
          for(int j=0;j<20;j++)
            table[i][j] = domyslna[i][j];
    }
    else {
        for (int i = 19; i >= 0; i--) {
            for (int j = 0; j < 20; j++) {
                if (!(plik >> table[i][j])) {
                    cout << "Nie mozna odczytac komorki z pliku" << endl;
                }
            }
        }
        plik.close();
    }
}

    void wyswietlMape(int tablica[20][20], bool celOsiagniety) {
    // Wyświetlenie tablicy z wyznaczoną trasą , sciezka oznaczona jako 3
    for (int i = 19; i >= 0; i--) {
        for (int j = 0; j < 20; j++) {
            cout << tablica[i][j] << " ";
        }
        cout << endl;
    }

    // Komunikat gdyby punkt końcowy nie był osiągalny
    if (celOsiagniety == false) {
        cout << "Nie mozna dotrzec do celu" << endl;
    }
    }

    //void Algorytm(int table[20][20]){
    //
    //}

int main() {

    //Zdefiniowanie rozmiaru tablicy ktora przedstawia grida
    //const int ROW = 20;
    //const int COL = 20;
    int tablica[20][20];


    //uzupelnienie tablicy
    uzupelnijTabele(tablica,"grid.txt");

    //kolejnosc sprawdzania sasiadow gora , dol ,lewo , prawo
    //zaczynamy w w punkcie lewo dol , ktory w tablicy ma wspolrzedne [0][0] , wiec aby isc na mapie do gory musimy zwiekszyc wspolrzedna y
    int kSprawdzanie[4][2] = {
    {0,1},  // góra
    {0, -1},  // dół
    {-1,0},  // lewo
    {1,0}    // prawo
};

    // lista otwarta , przechowuje kratki ktore sa rozwazane pod ekspansje
    komorka* lOtwarta[20 * 20];
    int otwartaIlosc = 0;

    //lista zamknieta , przechowuje kratki ktore juz byly odwiedzone , usuniete z listy otwartej
    komorka* lZamknieta[20 * 20];
    int zamknietaIlosc = 0;



    // tablica jest juz odwrocona , stworzenie punktu startowego wspol(0,0) i punktu koncowego wspol(19,19) , zaczynamy od dolu od lewej strony
    // idziemy prawo gora
    komorka* pStart = new komorka(0, 0, 0.0, hEuklidesowa(0, 0, 19, 19));
    komorka* pKoniec = new komorka(19, 19, 0.0, 0.0);

    // dodanie pstart do listy otwartej
    lOtwarta[0] = pStart;
    otwartaIlosc++;

    bool celOsiagniety = false;
    komorka* cel = nullptr;


    //algorytm

    // eksploracja dopoki elementy znajduja sie w liscie ,glowna petla algorytmu
    while (otwartaIlosc > 0) {

        // wybranie z listy otwartej elementu z najmiejszym f
        int indeksMinf = 0;
        // przeszukiwanie listy otwartej w celu znalezienia najmniejszego f
        for (int i = 1; i < otwartaIlosc; i++) {
            if (lOtwarta[i]->f < lOtwarta[indeksMinf]->f) {
                indeksMinf = i;
            }
        }

        komorka* aktualnaKomorka = lOtwarta[indeksMinf];

        // usuwanie elementu z listy otwartej przez przesuniecie elementow w lewo
        for (int i = indeksMinf; i < otwartaIlosc - 1; i++){
            lOtwarta[i] = lOtwarta[i + 1];
            }
        otwartaIlosc--;

        // dodanie elementu do listy zamknietej
        lZamknieta[zamknietaIlosc] = aktualnaKomorka;
        zamknietaIlosc++;

        // sprawdza czy wspolrzedne aktualnej komorki jest rowne pkoniec
        // jeśli algorytm dotarł do celu , oznacza ścieżkę w tablicy wartością 3
        // przechodząc wstecz przez wskazniki na rodziców od punktu końcowego do startowego
        if (aktualnaKomorka->x == pKoniec->x && aktualnaKomorka->y == pKoniec->y) {
            celOsiagniety = true;
            komorka* sciezka = aktualnaKomorka;
            while (sciezka != nullptr) {
                tablica[(*sciezka).y][(*sciezka).x] = 3;
                sciezka = (*sciezka).rodzic;
            }
            break;
        }

        // Sprawdzanie sąsiadów
        for (int i = 0; i < 4; i++) {
            int sasiadX = aktualnaKomorka->x + kSprawdzanie[i][0];
            int sasiadY = aktualnaKomorka->y + kSprawdzanie[i][1];

            //czy sasiad wychodzi poza granice grida , jesli tak jest pomijany
            if (sasiadX < 0 || sasiadY < 0 || sasiadX >= 20 || sasiadY >=20 )
                {
                    continue;
                }



            // jesli wezel jest przeszkoda to jest pomijana
            if (tablica[sasiadY][sasiadX] == 5)
            {
                continue;
            }

            // jesli wezel jest w liscie zamknietej to jest pomijany
            if (czyWLiście(lZamknieta, zamknietaIlosc, sasiadX, sasiadY))
            {
                continue;
            }

            //nowy koszt ruchu g i h dla sasiada
            double g = aktualnaKomorka->g + 1.0;
            double h = hEuklidesowa(sasiadX, sasiadY, pKoniec->x, pKoniec->y);

            // sprawdza czy wezel jest w liscie otwartej
            int indeksOtwarty = znajdzIndeks(lOtwarta, otwartaIlosc, sasiadX, sasiadY);

            if (indeksOtwarty != -1) {
                // gdy sciezka jest lepsza
                if (g < lOtwarta[indeksOtwarty]->g) {
                    lOtwarta[indeksOtwarty]->g = g;
                    lOtwarta[indeksOtwarty]->f = g + lOtwarta[indeksOtwarty]->h;
                    lOtwarta[indeksOtwarty]->rodzic = aktualnaKomorka;
                }
            } else {
                // jesli wezel nie znajduje sie na liscie otwartej , to jest tam dodawany
                if (otwartaIlosc < 20 * 20) {
                    lOtwarta[otwartaIlosc++] = new komorka(sasiadX, sasiadY, g, h, aktualnaKomorka);
                }
            }
        }
    }

    // wyswietlenie tablicy z wyznaczona trasa
    wyswietlMape(tablica,celOsiagniety);

    return 0;
}
