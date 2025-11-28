#include <iostream>
#include <cmath>
using namespace std;
//#define RED "\033[31m";

int main(int argc, char** argv) {
    system("color 0c");
    for(double i = 1.4; i >= -1.4; i-=0.1) {
        for(double j = -1.4; j <= 1.4; j+=0.05) {
            putchar((pow(i*i+j*j-1,3)<=i*i*pow(j,3))?'*':' ');
        }
        cout << "\n" << endl;
        //Sleep(100);
    }

    cout << "\tHello, World!" << endl;
    return 0;
}
