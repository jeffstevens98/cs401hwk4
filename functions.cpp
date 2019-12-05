#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>


#define PI 3.14159265
#define E 2.718281828

double Ackley (double x, double y){
  double ret = -20;
  double temp;
  double temp2;
  temp = exp(-0.2*sqrt(0.5*(pow(x,2)*pow(y,2))));
  temp2 = exp(0.5*(cos(2*PI*x) + cos(2*PI*y)));
  ret = ret * temp;
  ret = ret - temp2;
  ret = ret + E + 20;
  return ret;
}

double Easom (double x1, double x2){
  double ret;
  ret = -cos(x1)*cos(x2)*exp(-pow(x1-PI, 2)-(pow(x2-PI, 2)));
  return ret;
}

double Rastrigin (double x1, double x2){
  double ret = 20;
  double temp1 = pow(x1, 2) - (10*cos(2*PI*x1));
  double temp2 = pow(x2, 2) - (10*cos(2*PI*x2));
  ret = ret + temp1 + temp2;
  return ret;
}

double Sphere (double x[10]){
  double ret = 0;
  for (int i = 0; i < 10; i++){
    ret = ret + pow(x[i], 2);
  }
  return ret;
}


/*
int main(){
  printf("Ackley: %f\n", Ackley(0,0));
  printf("Easom: %f\n", Easom(PI,PI));
  printf("Rast: %f\n", Rastrigin(0,0));
  double x[10] = {0,0,0,0,0,0,0,0,0,0};
  printf("Sphere: %f\n", Sphere(x));
  return 0;
}
*/
