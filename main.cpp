#include <iostream>
#include <unistd.h>
#include <ncurses.h>



    using namespace std;

int zbiornik_paliwa= 5000;
double utarg;
float cena = 5;
bool trwa = true;
//long long sum=5000;
//int ilosc_powtorzen= 50000;
//long long sum=0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting_thread(void *arg)
{


    int pojemnosc_baku = *(int *) arg;
   for (int i = 0; i < pojemnosc_baku; i++){



        //Poczatek sekcji krytycznej
        pthread_mutex_lock(&mutex);

       // sum +=offset;
       //cout<<sum<<endl;

       zbiornik_paliwa = zbiornik_paliwa -1;

        pthread_mutex_unlock(&mutex);

       usleep(10000);
       utarg= utarg+cena;





    }
    trwa = false;
    pthread_exit(NULL);


}

int main (void){

   //OKNO POCZATKOWE
    initscr();
    printw("======================================================================");
    move(5,25);
    printw("STACJA BEZNYNOWA");
    //move(30,25);
    mvprintw(20,0,"======================================================================");
    mvprintw(21,20,"wcisnij dowonly klawisz");
    refresh();
    getch();
    endwin();



    pthread_t watek1;//id
    int wartosc1 = 1000;
    pthread_create(&watek1,NULL, counting_thread, &wartosc1);

    pthread_t watek2;
    int wartosc2 = 3000;

    pthread_create(&watek2,NULL, counting_thread, &wartosc2);

    //MONITOROWANIE WATKOW
    erase();
    initscr();
    printw("Uzycie zbiornika glownego");
    while(trwa){
        mvprintw(10,10,"Zbiornik %u", zbiornik_paliwa);
        refresh();

    }
    getch();
    endwin();


    pthread_join(watek1, NULL);
    pthread_join(watek2, NULL);


   // cout<<"Paliwa w zbiorniku "<<zbiornik_paliwa<<endl;
   // cout<<"Utarg "<<utarg<<endl;
   // cout<<"sum "<<sum;


   /* initscr();
    printw("paliwo: %u ", zbiornik_paliwa);
    refresh();
    getch();
    endwin();
    return 0;*/


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

    //long long *limit_ptr = (long long*) arg;
  //  long long limit  = *limit_ptr;


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