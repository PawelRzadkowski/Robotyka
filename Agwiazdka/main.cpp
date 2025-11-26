//Pawe³ Rzadkowski nr. 175489

#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//struktura pojedynczego wezla , komorki
struct komorka {
	int x;
	int y;
	double g;
	double h;
	double f;
	komorka* rodzic;
};

//wyznaczenie odleglosci od punktu koncowego
double hEuklidesowa(int x1,int y1, int x2, int y2){
	return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

//do sprawdzania czy wezel jest w liscie
bool czyWLiscie(komorka* lista[], int ilosc, int x, int y){
	for(int i=0; i < ilosc; i++){
		if(lista[i]->x == x && lista[i]->y == y){
			return true;
		}
	}
	return false;
}

//do znajdowania indeksu
int znajdzIndeks(komorka* lista[], int ilosc, int x , int y){
	for(int i=0; i < ilosc; i++){
		if(lista[i]->x == x && lista[i]->y == y){
			return i;
		}
	}
	return -1;
}

//z pliku grid.txt wczytujemy komorki i wstawiamy je do tablicy
//wpisywane sa od dolu do gory , wiec tablica jest odwrocona
//w przeciwnym wypadku punkt startowy znajdowal sie w punkcie[0][19] , a koncowy w [19][0]
//po odwroceniu punkt startowy znajduje sie w [0][0] a koncowy w [19][19]
void uzupelnijTabele(int table[20][20], string nazwa){
	ifstream plik(nazwa);
	if(!plik){
		cout << "Blad przy otwieraniu pliku , albo plik nie istnieje , wczytana tablica domyslna" << endl;
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
		for(int i= 19; i>=0; i--){
			for(int j=0; j < 20; j++){
				if(!(plik >> table[i][j])){
					cout << "Nie mozna bylo odczytac komorki z pliku" << endl;
				}
			}
		}
		plik.close();
	}
}

//wyswietlenie tablicy z wyznaczona trasa , sciezka zaznaczona na mapie jako 3
void wyswietlMape(int tablica[20][20], bool celOsiagniety){
	for(int i=19; i>=0; i--){
		for(int j=0;j<20;j++){
			cout << tablica[i][j] << " ";
		}
		cout << endl;
	}

	//wiadomosc gdyby nie dalo sie dotrzec do celu
	if (celOsiagniety == false){
		cout << "Nie mozna bylo dotrzec do celu"<<endl;
	}
}

//void Algorytm(int table[20][20]){
//
//}


int main()
{
	//Zdefiniowanie tablicy ktora bedzie przedstawiac grida
	//const int ROW=20;
	//const int COL=20;
	int tablica[20][20];

	//uzupelnienie tablicy
	uzupelnijTabele(tablica,"grid.txt");

	//kolejnosc sprawdzania sasiadow : gora , dol ,lewo ,prawo
	//start w prawej dolnej czesci grida
	int kSprawdzanie[4][2]={
		{0,1},  //gora
		{0,-1},  //dol
		{-1,0},  //lewo
		{1,0}  //prawo
	};

	//lista otwarta , bedzie przechowywac kratki ktore sa rozwazane pod ekspansje
	komorka* lOtwarta[20*20];
	int otwartaIlosc=0;

	//lista zamknieta , bedzie przechowywac kratki ktore juz byly odwiedzone , usuniete z listy otwartej
	komorka* lZamknieta[20*20];
	int zamknietaIlosc=0;

	//stworzenie punktu startowego i punktu koncowego
	komorka* pStart = new komorka;
	pStart->x=0;
	pStart->y=0;
	pStart->g=0.0;
	pStart->h=hEuklidesowa(0,0,19,19);
	pStart->f=pStart->g+pStart->h;
	pStart->rodzic=nullptr;

	komorka* pKoniec = new komorka;
	pKoniec->x=19;
	pKoniec->y=19;
	pKoniec->g=0.0;
	pKoniec->h=0.0;
	pKoniec->f=pKoniec->g + pKoniec->h;
	pKoniec->rodzic=nullptr;

	//dodanie pStart do listy otwartej
	lOtwarta[0]=pStart;
	otwartaIlosc++;

	bool celOsiagniety=false;
	komorka* cel = nullptr;

	//algorytm

	//eksploracja dopoki elementy znajduja sie w liscie , glowna petla algorytmu
	while (otwartaIlosc > 0){

		//wybranie z listy otwartej elementu o najmniejszym fabs
		int indeksMinf=0;
		//przeszukiwanie listy otwartej w celu znalezenia najmniejszego f
		for(int i=1; i<otwartaIlosc;i++){
			if(lOtwarta[i]->f < lOtwarta[indeksMinf]->f){
				indeksMinf=i;
			}
		}

		komorka* aktualnaKomorka=lOtwarta[indeksMinf];

		//usuwanie elementu z listy otwartej przez przesuniecie elementow w lewo
		for(int i=indeksMinf;i<otwartaIlosc - 1; i++){
			lOtwarta[i]=lOtwarta[i+1];
		}
		otwartaIlosc--;

		//dodanie elementu do listy zamknietej
		lZamknieta[zamknietaIlosc] = aktualnaKomorka;
		zamknietaIlosc++;

		//czy wspolrzedne aktualnej komorki sa rowne pKoniec
		//jesli algorytm dotarl do celu, oznacza sciezke w tablicy 3
		//przechodzac wstecz przez wskazniki na rodzicow od pKoniec do pStart
		if(aktualnaKomorka->x == pKoniec->x && aktualnaKomorka->y == pKoniec->y){
			celOsiagniety=true;
			komorka* sciezka = aktualnaKomorka;
			while(sciezka != nullptr){
				tablica[(*sciezka).y][(*sciezka).x]=3;
				sciezka=(*sciezka).rodzic;
			}
			break;
		}

		//sprawdzanie sasiadow
		for(int i=0;i<4;i++){
			int sasiadX = aktualnaKomorka->x + kSprawdzanie[i][0];
			int sasiadY = aktualnaKomorka->y + kSprawdzanie[i][1];

			//czy sasiad wychodzi poza granice grida , jesli tak jest pomijany
			if(sasiadX < 0 || sasiadY<0 || sasiadX >=20 || sasiadY>=20){
				continue;
			}

			//jesli wezel jest przeszkoda to jest pomijany
			if(tablica[sasiadY][sasiadX]==5){
				continue;
			}

			//jesli wezel jest w liscie zamknietej to jest pomijany
			if(czyWLiscie(lZamknieta,zamknietaIlosc,sasiadX,sasiadY)){
				continue;
			}

			//nowy koszt ruchu g i h dla sasiada
			double g=aktualnaKomorka->g+1.0;
			double h=hEuklidesowa(sasiadX,sasiadY,pKoniec->x, pKoniec->y);

			//sprawdza czy wezel jest w liscie otwartej
			int indeksOtwarty=znajdzIndeks(lOtwarta,otwartaIlosc,sasiadX,sasiadY);

			if(indeksOtwarty != -1){
				// gdy sciezka jest lepsza
				if(g < lOtwarta[indeksOtwarty]->g){
					lOtwarta[indeksOtwarty]->g =g;
					lOtwarta[indeksOtwarty]->f = g+lOtwarta[indeksOtwarty]->h;
					lOtwarta[indeksOtwarty]->rodzic = aktualnaKomorka;
				}
			}
			else {
				//jesli wezel nie znajduje sie na liscie otwartej , to jest tam dodawany
				if(otwartaIlosc < 20 * 20) {
					lOtwarta[otwartaIlosc] = new komorka;
					lOtwarta[otwartaIlosc]->x=sasiadX;
					lOtwarta[otwartaIlosc]->y=sasiadY;
					lOtwarta[otwartaIlosc]->g=g;
					lOtwarta[otwartaIlosc]->h=h;
					lOtwarta[otwartaIlosc]->f=g+h;
					lOtwarta[otwartaIlosc]->rodzic = aktualnaKomorka;
					otwartaIlosc++;
				}
			}
		}
	}

	//wyswietlenie tablicy z wyznaczona trasa
	wyswietlMape(tablica,celOsiagniety);

	return 0;
}
