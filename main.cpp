#include <iostream>




    using namespace std;

int ilosc_powtorzen= 500000;
long long sum=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting_thread(void *arg)
{
    int offset = *(int *) arg;
   for (int i = 0; i < ilosc_powtorzen; i++){

        //Poczatek sekcji krytycznej
        pthread_mutex_lock(&mutex);

        sum +=offset;
       //cout<<sum<<endl;

        pthread_mutex_unlock(&mutex);

    }
    pthread_exit(NULL);

}

int main (void){

    cout<<endl<<"ile powtorzen wykonac?"<<endl;
    cin>>ilosc_powtorzen;

    pthread_t watek1;
    int wartosc1 = 1;
    pthread_create(&watek1,NULL, counting_thread, &wartosc1);


    pthread_t watek2;
    int wartosc2 = -1;
    pthread_create(&watek2,NULL, counting_thread, &wartosc2);


    pthread_join(watek1, NULL);
    pthread_join(watek2, NULL);


    cout<<"Suma wynosi: "<<sum;
    return 0;


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