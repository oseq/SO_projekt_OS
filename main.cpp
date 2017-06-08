#include <iostream>
#include <unistd.h>
#include <ncurses.h>


using namespace std;

int zbiornik_paliwa= 5000;
float utarg;
float cena = 4.65;
bool trwa = true;

int wartosc1 = 40;
int wartosc2 = 30;

int nalane1;
int nalane2;

float naleznosc1;
float naleznosc2;


pthread_t watek1;//id
pthread_t watek2;
pthread_t watek_wyswietlania;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* tankowanie(void *arg)
{
    int nalane=0;
    float naleznosc=0;
   int pojemnosc_baku = *(int *) arg;



   for (int i = 0; i < pojemnosc_baku; i++){

       //Poczatek sekcji krytycznej
        pthread_mutex_lock(&mutex);
       zbiornik_paliwa = zbiornik_paliwa -1;

       //Koniec sekcji krytycznej
       pthread_mutex_unlock(&mutex);

       nalane = nalane +1;
       naleznosc = naleznosc+cena;
       if (pojemnosc_baku ==wartosc1){
           nalane1=nalane;
           naleznosc1=naleznosc;

       }else{
           nalane2=nalane;
           naleznosc2=naleznosc;
       }



       usleep(200000);


    }
    utarg= utarg+naleznosc;
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
    mvprintw(4,5,"Pojemnosc zbiornika: %u l.", zbiornik_paliwa);
    mvprintw(5,5,"Cena litra paliwa: %*.*f PLN ",5,2, cena);
    mvprintw(6,5,"Pojemnosc zbiornika 1 samochodu: %u l.", wartosc1);
    mvprintw(7,5,"Pojemnosc zbiornika 2 samochodu: %u l.", wartosc2);
    mvprintw(20,3,"Nacisnij:      aby zakonczyc program");
    mvprintw(21,3,"Nacisnij:      aby wylaczyc dystrybutro 1");
    mvprintw(22,3,"Nacisnij:      aby wylaczyc dystrybutor 2");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(4));
    mvprintw(20,14,"q");
    mvprintw(21,14,"o");
    mvprintw(22,14,"p");
    attroff(COLOR_PAIR(4));


    char c;
    timeout(0);
    while(trwa){
        attron(COLOR_PAIR(2));
        mvprintw(10,10,"Zbiornik [ %u litrow ] ", zbiornik_paliwa);
        mvprintw(11,10,"Utarg [ %*.*f PLN ] ",5,2, utarg);

        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(1));

        //if(nalane1 >0) {

            mvprintw(14, 10, "DYSTRYBUTOR 1:");
            mvprintw(15, 10, "[ %u litrow ]  ", nalane1);
            mvprintw(16, 10, "[ %*.*f PLN ] ", 5, 2, naleznosc1);
        //}

     //   if (nalane2 > 0) {
            mvprintw(14, 40, "DYSTRYBUTOR 2:");
            mvprintw(15, 40, "[ %u litrow ]  ", nalane2);
            mvprintw(16, 40, "[ %*.*f PLN ] ", 5, 2, naleznosc2);
       //     }

        mvprintw(22, 50, "...");


        attroff(COLOR_PAIR(1));

        c=getch();
        if(c=='q'){
            pthread_cancel(watek1);
            pthread_cancel(watek2);
            endwin();
            pthread_cancel(watek_wyswietlania);


            break;}
        else if(c=='o')
            pthread_cancel(watek1);
        else if(c=='p')
            pthread_cancel(watek2);



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


    okno_startowe();




    pthread_create(&watek_wyswietlania,NULL, monitoring,NULL);




    pthread_create(&watek1,NULL, tankowanie, &wartosc1);
    usleep(5000000);

    pthread_create(&watek2,NULL, tankowanie, &wartosc2);





    pthread_join(watek1, NULL);
    pthread_join(watek2, NULL);
    trwa= false;
    pthread_join(watek_wyswietlania, NULL);





}



/*
struct sum_runner_struct{
    long long limit;
    long long answer;

};
//funkcja watku generujaca sume od 0 do N
void* sum_runner(void* arg){

    struct sum_runner_struct *arg_struct= (struct sum_runner_struct*) arg;

    long long sum = 0;



    for (long long i= 0; i<=arg_struct->limit; i++){
        sum +=i;

    }

    arg_struct ->answer = sum;

    pthread_exit(0) ;
}



int main(int argc, char ** argv) {


    //STACJA BENZYNOWA


    if (argc <2 ) {
        printf ("Użycie: %s <liczba 1> <liczba 2> ... <liczba N>\n", argv[0]);
        exit(-1);
    }

    int liczba_argumentow= argc - 1; //liczba argumentow to liczba argumentow -1 bo pierwszy to nazwa pliku
    struct sum_runner_struct args[liczba_argumentow];

    pthread_t nazwyWatkow[liczba_argumentow];
    for (int i=0; i< liczba_argumentow;i++) {

        args[i].limit = atoll(argv[i+1]);

        pthread_attr_t attr; //thread atrybut
        pthread_attr_init(&attr);
        pthread_create(&nazwyWatkow[i], &attr, sum_runner, &args[i]);

    }

for(int i=0; i<liczba_argumentow;i++) {
    pthread_join(nazwyWatkow[i], NULL);//czekanie na zakonczenie watku
    cout<<"Suma dla watku:"<<i<<", argumentu:"<< args[i].limit<<" = "<<args[i].answer<<endl;
     //printf("Sum for thread %u is %lld \n",nazwyWatkow[i],args[i].answer);

}

    return 0;
}
 */