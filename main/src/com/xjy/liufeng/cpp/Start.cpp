#include <iostream>
#include <cmath>
using namespace std;

int main(int argc, char** argv) {
    //system("color 0c");
    for (double y = 1.4; y >= -1.4; y -= 0.1) {
        for (double x = -1.4; x <= 1.4; x += 0.05) {
            cout << /*putchar*/((pow(x*x+y*y-1,3)<=x*x*pow(y,3))?'x':' ');
        }
        endl(cout);//cout << '\n' << endl; or cout << endl;
        //sleep(t); t = 100
    }
    cout << "\tHello, World!" << endl;
    return 0;
}
