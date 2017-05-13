#include <iostream>
#include <unistd.h>
#include <ncurses.h>



    using namespace std;

int zbiornik_paliwa= 5000;
float utarg;
float cena = 4.65;
bool trwa = true;

int wartosc1 = 65;
int wartosc2 = 50;

int nalane1;
int nalane2;

float naleznosc1;
float naleznosc2;

//long long sum=5000;
//int ilosc_powtorzen= 50000;
//long long sum=0;
/*
struct struktura_dystrybutora{
    int kupione_paliwo;
    float naleznosc;

};*/


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting_thread(void *arg)
{
    int nalane=0;
    float naleznosc=0;
   int pojemnosc_baku = *(int *) arg;
   //float jedna_dziesiata_baku= pojemnosc_baku*0.1;
    //int dziesiatki= 1;


   for (int i = 0; i < pojemnosc_baku; i++){
        //Poczatek sekcji krytycznej
        pthread_mutex_lock(&mutex);
                zbiornik_paliwa = zbiornik_paliwa -1;
                utarg= utarg+cena;
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

    pthread_exit(NULL);

}

void* monitoring(void *arg){
    erase();
    initscr();
    printw("======================================================================");
    mvprintw(2,25,"***STACJA BEZNYNOWA***");
    mvprintw(3,5,"Pojemnosc zbiornika: %u l.", zbiornik_paliwa);
    mvprintw(4,5,"Cena litra paliwa: %*.*f PLN ",5,2, cena);
    mvprintw(5,5,"Pojemnosc zbiornika 1 samochodu: %u l.", wartosc1);
    mvprintw(6,5,"Pojemnosc zbiornika 2 samochodu: %u l.", wartosc2);

    while(trwa){
        mvprintw(10,10,"Zbiornik [ %u litrow ] ", zbiornik_paliwa);
        mvprintw(12,10,"Utarg [ %*.*f PLN ] ",5,2, utarg);

        if(nalane1 >0){
            mvprintw(15,10,"samochod 1 [ %u litrow ]  ", nalane1);
            mvprintw(16,20," [ %*.*f PLN ] ",5,2, naleznosc1);}
        if(nalane2 >0){
            mvprintw(18,10,"samochod 2 [ %u litrow ]  ", nalane2);
            mvprintw(19,20," [ %*.*f PLN ] ",5,2, naleznosc2);}

        refresh();



    }
    getch();
    endwin();

}

int main (void){

   //OKNO POCZATKOWE
    initscr();
    printw("======================================================================");
    move(5,25);
    printw("***STACJA BEZNYNOWA***");
    //move(30,25);
    mvprintw(20,0,"======================================================================");
    mvprintw(21,20,"wcisnij dowonly klawisz");
    refresh();
    getch();
    endwin();



    pthread_t watek_wyswietlania;
    int wartosc3 = 1;
    pthread_create(&watek_wyswietlania,NULL, monitoring, &wartosc3);


    pthread_t watek1;//id
    pthread_t watek2;

    pthread_create(&watek1,NULL, counting_thread, &wartosc1);
    usleep(5000000);

    pthread_create(&watek2,NULL, counting_thread, &wartosc2);


    //MONITOROWANIE WATKOW



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