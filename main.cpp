#include <iostream>




    using namespace std;



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

    //long long limit = atoll(argv[1]);


   // pthread_t tid; //thread id

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
    cout<<"Suma["<<args[i].limit<<"]="<<args[i].answer<<endl;
    }

    return 0;
}