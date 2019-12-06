#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>


#define PI 3.14159265
#define E 2.718281828

//2 Dimensions, decision space = [-32,32]
double Ackley (double x[2]){
  double ret = -20;
  double temp;
  double temp2;
  temp = exp(-0.2*sqrt(0.5*(pow(x[0],2)+pow(x[1],2))));
  temp2 = exp(0.5*(cos(2*PI*x[0]) + cos(2*PI*x[1])));
  ret = ret * temp;
  ret = ret - temp2;
  ret = ret + E + 20;
  return ret;
}

double Ackley2 (double * x, int cnt){
  double ret = -20;
  double temp = 0;
  double temp2 = 0;
  for (int i = 0; i < 10; i++){
    temp = temp + 0.5*(pow(x[i],2));
    temp2 = temp2 + (1/cnt)*cos(2*PI*x[i]);
  }
  ret = ret * temp;
  ret = ret - temp2;
  ret = ret + E + 20;
  return ret;
}

//2 Dimensions, decision space = [-5.12, 5.12]
double Easom (double x[2]){
  double ret;
  ret = -cos(x[0])*cos(x[1])*exp(-pow(x[0]-PI, 2)-(pow(x[1]-PI, 2)));
  return ret;
}

//2 Dimensions, decision space = [-100,100]
double Rastrigin (double x[2]){ //if you want to scale up, add a dimension parm and set dim equal to it
  double ret = 20;
  int dim = 2; //can be changed if want to scale up
  for (int i = 0; i < dim; i++){
    ret = ret + pow(x[0], dim) - 10 * cos(2*PI*x[i]);
  }
  return ret;
}

double Rastrigin2 (double * x, int cnt){ //if you want to scale up, add a dimension parm and set dim equal to it
  double ret = 10*cnt;
  for (int i = 0; i < cnt; i++){
    ret = ret + pow(x[0], cnt) - 10 * cos(2*PI*x[i]);
  }
  return ret;
}

//10 Dimensions, decision space = [-5,5]
double Sphere (double x[10]){
  double ret = 0;
  for (int i = 0; i < 10; i++){
    ret = ret + pow(x[i], 2);
  }
  return ret;
}

double Sphere2 (double * x, int cnt){
  double ret = 0;
  for (int i = 0; i < cnt; i++){
    ret = ret + pow(x[i], 2);
  }
  return ret;
}



//10 Dimensions, decision space = [1,-1]
double SumOfPowers (double x[10]){
  double ret = 0;
  for (int i = 1; i <= 10; i++){
    ret = ret + pow(abs(x[i]), i);
  }
  return ret;
}

double SumOfPowers2 (double * x, int cnt){
  double ret = 0;
  for (int i = 1; i <= cnt+1; i++){
    ret = ret + pow(abs(x[i]), i);
  }
  return ret;
}

/*
int main(){
  double x1[2] = {0,0};
  double x2[2] = {PI,PI};
  double x3[10] = {0,0,0,0,0,0,0,0,0,0};
  printf("Ackley: %f\n", Ackley(x1));
  printf("Ackley2: %f\n", Ackley2(x3, 10));
  printf("Easom: %f\n", Easom(x2));
  printf("Rast: %f\n", Rastrigin(x1));
  printf("Rast2: %f\n", Rastrigin2(x3, 10));
  printf("Sphere: %f\n", Sphere(x3));
  printf("Sphere2: %f\n", Sphere2(x1,2));
  printf("SOP: %f\n", SumOfPowers(x3));
  double x4[2] = {0,0};
  printf("SOP2: %f\n", SumOfPowers2(x4,2));
  return 0;
}
*/
