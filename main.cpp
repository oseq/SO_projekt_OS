#include <iostream>




    using namespace std;

int main() {

    string wyraz;
    srand(time(NULL));
    int los =( rand() % 100)+0;

    cout << "Jak sie nazywa twoja dupa?" << endl;
    cin>>wyraz;
    cout<<"Twoja "<<wyraz<<" kocha cie na "<<los<<"%";


    return 0;
}