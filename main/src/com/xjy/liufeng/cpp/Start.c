#include<stdio.h>
#include<math.h>
//#include<windows.h>

int main() {
  system("color 0c");
  //double x, y;
  for(double y=1.5;y>=-0.5;y-=0.05) {
    for(double x=-1.5;x<=1.5;x+=0.025) {
      putchar((pow(x*x+y*y-1,3)<=x*x*pow(y,3))?'*':' ');
    }
      printf("\n");
      //Sleep(100);
  }
    printf("\tHello world!\n");
    return 0;
}
