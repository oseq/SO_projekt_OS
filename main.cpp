#include <iostream>
#include <unistd.h>
#include <ncurses.h>
using namespace std;

//OSKAR SPERUDA 2017

//zmienne
int zbiornik_paliwa= 5000; //pojemnosc glownego zbiornika z którego pobierane jest palio
float utarg=0;             //utarg stacji
float cena = 4.65;

int wartosc1 = 40;         //zmienna okreslajaca jak duzo paliwa tankuje 1 samochod
int wartosc2 = 30;         //zmienna okreslajaca jak duzo paliwa tankuje 2 samochod

int nalane1=0;             //zmienna okreslajaca ile paliwa zostalo to samochodu 1
int nalane2=0;             //zmienna okreslajaca ile paliwa zostalo to samochodu 1
float naleznosc1=0;        //zmienna okreslajaca ile powinnnien zaplacic kierowca samochodu 1
float naleznosc2=0;        //zmienna okreslajaca ile powinnnien zaplacic kierowca samochodu 1

bool start1= false;       //zmienne boolowie okreslajace czy dany dystrybutor pracuje
bool start2=false;

//id watkow:
pthread_t watek1;           //watek tankowania 1 samochodu
pthread_t watek2;           //watek tankowania 2 samochodu
pthread_t watek_wyswietlania;   //watek minitorowania procesu tankowania

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex


void* tankowanie(void *arg)
{
   int pojemnosc_baku = *(int *) arg; //zienna inicjowana wartoscia argumentu funkcji pojemnoscia baku samochodu

   for (int i = 0; i < pojemnosc_baku; i++){
          pthread_mutex_lock(&mutex);     //Poczatek sekcji krytycznej
         //##########################################
         zbiornik_paliwa = zbiornik_paliwa -1;
         //##########################################
         pthread_mutex_unlock(&mutex);     //Koniec sekcji krytycznej

         if (pojemnosc_baku ==wartosc1){
            nalane1+=1;
            naleznosc1+=cena;}
         else if (pojemnosc_baku==wartosc2){
           nalane2+=1;
           naleznosc2+=cena;
       }
       usleep(200000);
    }

    if (pojemnosc_baku ==wartosc1)
        utarg += naleznosc1;
    else if (pojemnosc_baku==wartosc2)
       utarg+=naleznosc2;

    pthread_exit(NULL);

}

void* monitoring(void *){
    erase();
    initscr();

    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("======================================================================");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(4));
    mvprintw(2,25,"***STACJA BEZNYNOWA***");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(1));
    mvprintw(3,0,"======================================================================");
    mvprintw(4,5,"Pojemnosc poczatkowa zbiornika: %u l.", zbiornik_paliwa);
    mvprintw(5,5,"Cena litra paliwa: %*.*f PLN ",5,2, cena);
    mvprintw(6,5,"Pojemnosc zbiornika 1 samochodu: %u l.", wartosc1);
    mvprintw(7,5,"Pojemnosc zbiornika 2 samochodu: %u l.", wartosc2);
    mvprintw(20,3,"Nacisnij:      aby zakonczyc program");
    mvprintw(21,3,"Nacisnij:      aby wlaczyc/wylaczyc dystrybutro 1");
    mvprintw(22,3,"Nacisnij:      aby wlaczyc/wylaczyc dystrybutor 2");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(4));
    mvprintw(20,14,"q");
    mvprintw(21,14,"1");
    mvprintw(22,14,"2");
    attroff(COLOR_PAIR(4));

    timeout(0);
    while(true){  //petla monitoringu
        attron(COLOR_PAIR(2));
        mvprintw(10,10,"Zbiornik [ %u litrow ] ", zbiornik_paliwa);
        mvprintw(11,10,"Utarg [ %*.*f PLN ] ",5,2, utarg);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(1));

        mvprintw(14, 10, "DYSTRYBUTOR 1:");
        mvprintw(15, 10, "[ %u litrow ]  ", nalane1);
        mvprintw(16, 10, "[ %*.*f PLN ] ", 5, 2, naleznosc1);

        mvprintw(14, 40, "DYSTRYBUTOR 2:");
        mvprintw(15, 40, "[ %u litrow ]  ", nalane2);
        mvprintw(16, 40, "[ %*.*f PLN ] ", 5, 2, naleznosc2);

        mvprintw(22, 50, "...");
        attroff(COLOR_PAIR(1));
        char c;
        c=getch();
        if(c=='q'){
            if(start1)
            pthread_cancel(watek1);
            if(start2)
            pthread_cancel(watek2);
            endwin();
            pthread_cancel(watek_wyswietlania);
            break;}
         else if(c=='1' && start1==true){
            pthread_cancel(watek1);
            start1=false;}
         else if(c=='2'&& start2==true){
            pthread_cancel(watek2);
            start2=false;}
        else if(c=='1'&& start1==false) {
            pthread_create(&watek1, NULL, tankowanie, &wartosc1);//rozpoczacie watku pierwszego dystrybutora
            start1=true;}
        else if(c=='2'&& start2==false){
            pthread_create(&watek2,NULL, tankowanie, &wartosc2);//rozpoaczcie watku drugiego dystrybutora
            start2=true;}
        refresh();
    }
    attron(COLOR_PAIR(3));
    mvprintw(24,3,"NACISNIJ DOWOLNY PRZYCISK BY ZAKOCZYC");
    attroff(COLOR_PAIR(3));
    timeout(10000);
    getch();
    attroff(COLOR_PAIR(1));
    endwin();

}

void okno_startowe(){

    //OKNO POCZATKOWE
    initscr();

    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("======================================================================");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(4));
    mvprintw(5,25,"***   STACJA BEZNYNOWA     ***");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(1));
    mvprintw(20,0,"======================================================================");
    mvprintw(14,20,"Ladowanie programu prosze czekac ;)");
    mvprintw(15,25,"[");
    mvprintw(15,46,"]");
    attroff(COLOR_PAIR(1));

   for(int x=26;x<46;x++){
       attron(COLOR_PAIR(3));
       mvprintw(15,x,"#");
       attroff(COLOR_PAIR(3));
       usleep(150000);
       refresh();
   }

    mvprintw(21,20,"Nacisnij dowony przycisk aby kontyuowac");
    getch();
    refresh();
    endwin();

}

int main (void){

    okno_startowe();//wywolanie funkcji okna startowego ncurses

    pthread_create(&watek_wyswietlania,NULL, monitoring,NULL); //rozpoczacie watku monittoringu tankowania


    pthread_join(watek1, NULL);                   //czekanie na zakcnczenie watkow
    pthread_join(watek2, NULL);
    pthread_join(watek_wyswietlania, NULL);





}

