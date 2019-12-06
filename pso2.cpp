//Particle Swarm Optimization for Ackley Funciton


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "functions.cpp"

#define IT_MAX 50
#define PART_MAX 50
//#define POSVEL_MAX 2
#define PI 3.14159265
#define BETA 0.9
#define E 2.718281828
#define AER 10
#define SS 2


/*
struct particle {
  double position[POSVEL_MAX];
  double velocity[POSVEL_MAX];
  double pbest;
  double weight;
};
*/
struct swarm {
  struct particle * pswarm[PART_MAX];
  double pos[PART_MAX][AER];
  double vel[PART_MAX][AER];
  double pbest[PART_MAX][AER];
  double v_pbest[PART_MAX];
  double gbest[PART_MAX];
  double v_gbest;
  double weight;
};
struct s_swarm {
  struct particle * pswarm[PART_MAX];
  double pos[PART_MAX][SS];
  double vel[PART_MAX][SS];
  double pbest[PART_MAX][SS];
  double v_pbest[PART_MAX];
  double gbest[PART_MAX];
  double v_gbest;
  double weight;
};



void create_swarm (struct swarm * swarm, int posvel, int fun){
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      if (fun == 2){
        swarm->pos[i][j] = (double)rand()/((double)RAND_MAX/10.24)-5.12;
      }
      else{
        swarm->pos[i][j] = (double)rand()/((double)RAND_MAX/64.0)-32;
      }
      if (fun == 2){
        swarm->vel[i][j] = (double)rand()/((double)RAND_MAX/10.24)-5.12;
      }
      else{
        swarm->vel[i][j] = (double)rand()/((double)RAND_MAX/64.0)-32;
      }
    }
    swarm->v_pbest[i] = 1000.0;
  }
  swarm->v_gbest = 1000.0;
}

void position_update (struct swarm * swarm, int posvel, int fun){
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      double temp = swarm->pos[i][j] + swarm->vel[i][j];
      double max;
      if (fun == 0){
        max = 32.0;
      }
      else if (fun == 2){
        max = 5.12;
      }
      else{
        max = 100.0;
      }
      if (temp > max)
        temp = max;
      if (temp < -1.0*max)
        temp = -1.0*max;
      swarm->pos[i][j] = temp;
    }
  }
}

void gbpb_update (struct swarm * swarm, int posvel, int fun){
  double min = swarm->v_gbest;
  double xmin[AER];
  double partfun;
  for (int i = 0; i < PART_MAX; i++){
    double x[posvel];
    for (int j = 0; j < posvel; j++){
      x[j] = swarm->pos[i][j];
    }
    if (fun == 0){
      partfun = Ackley2(x,AER);
    }
    else if (fun == 2){
      partfun = Easom(x);
    }
    else{
      partfun = Rastrigin2(x,AER);
    }

    if (partfun < min){
      min = partfun;
      for (int k = 0; k < posvel; k++){
        xmin[k] = swarm->pos[i][k];
      }
    }
    if (partfun > swarm->v_pbest[i])
      swarm->v_pbest[i] = partfun;
      for (int k = 0; k < posvel; k++){
        swarm->pbest[i][k] = xmin[k];
      }
  }
  swarm->v_gbest = min;
  for (int k = 0; k < posvel; k++){
    swarm->gbest[k] = xmin[k];
  }
}


void velocity_update (struct swarm * swarm, int posvel){
  swarm->weight = swarm->weight * BETA;
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      double vu = swarm->vel[i][j] * swarm->weight;
      vu = vu + 0.5*(swarm->pbest[i][j] - swarm->pos[i][j]);
      vu = vu + 0.9*(swarm->gbest[j] - swarm->pos[i][j]);
      swarm->vel[i][j] = vu;
    }
  }
}
void s_create_swarm (struct s_swarm * swarm, int posvel, int fun){
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      if (fun == 1){
        swarm->pos[i][j] = (double)rand()/((double)RAND_MAX/10.0)-5.0;
      }
      else{
        swarm->pos[i][j] = (double)rand()/((double)RAND_MAX/2.0)-1.0;
      }
      if (fun == 2){
        swarm->vel[i][j] = (double)rand()/((double)RAND_MAX/10.0)-5.0;
      }
      else{
        swarm->vel[i][j] = (double)rand()/((double)RAND_MAX/2.0)-1.0;
      }
    }
    swarm->v_pbest[i] = 1000.0;
  }
  swarm->v_gbest = 1000.0;
}

void s_position_update (struct s_swarm * swarm, int posvel, int fun){
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      double temp = swarm->pos[i][j] + swarm->vel[i][j];
      double max;
      if (fun == 1){
        max = 5.0;
      }
      else{
        max = 1.0;
      }
      if (temp > max)
        temp = max;
      if (temp < -1.0*max)
        temp = -1.0*max;
      swarm->pos[i][j] = temp;
    }
  }
}

void s_gbpb_update (struct s_swarm * swarm, int posvel, int fun){
  double min = swarm->v_gbest;
  double xmin[SS];
  double partfun;
  for (int i = 0; i < PART_MAX; i++){
    double x[posvel];
    for (int j = 0; j < posvel; j++){
      x[j] = swarm->pos[i][j];
    }
    if (fun == 1){
      partfun = Sphere2(x,SS);
    }
    else{
      partfun = SumOfPowers2(x,SS);
    }

    if (partfun < min){
      min = partfun;
      for (int k = 0; k < posvel; k++){
        xmin[k] = swarm->pos[i][k];
      }
    }
    if (partfun > swarm->v_pbest[i])
      swarm->v_pbest[i] = partfun;
      for (int k = 0; k < posvel; k++){
        swarm->pbest[i][k] = xmin[k];
      }
  }
  swarm->v_gbest = min;
  for (int k = 0; k < posvel; k++){
    swarm->gbest[k] = xmin[k];
  }
}


void s_velocity_update (struct s_swarm * swarm, int posvel){
  swarm->weight = swarm->weight * BETA;
  for (int i = 0; i < PART_MAX; i++){
    for (int j = 0; j < posvel; j++){
      double vu = swarm->vel[i][j] * swarm->weight;
      vu = vu + 0.5*(swarm->pbest[i][j] - swarm->pos[i][j]);
      vu = vu + 0.9*(swarm->gbest[j] - swarm->pos[i][j]);
      swarm->vel[i][j] = vu;
    }
  }
}


int main (int argc, char *argv[]){
  srand(time(NULL));
  if (argc < 2){
    printf("Please use command line args: 0 for Ackley, 1 for Sphere, 2 for Easom, 3 for Rastrigin, 4 for Sum of Powers\n");
    exit(0);
  }
  printf("%s\n", argv[1]);
  int def = atoi(argv[1]);
  if (def == 0 || def == 2 || def == 3) {
    struct swarm swarm;
    create_swarm(&swarm, AER, def);
    for (int i = 0; i < IT_MAX; i++){
      gbpb_update(&swarm, AER, def);
      position_update(&swarm, AER, def);
      velocity_update(&swarm, AER);
      printf("it: %d, gbest: %f\n", i, swarm.v_gbest);
    }
  }
  else{
    struct s_swarm swarm;
    s_create_swarm(&swarm, SS, def);
    for (int i = 0; i < IT_MAX; i++){
      s_gbpb_update(&swarm, SS, def);
      s_position_update(&swarm, SS, def);
      s_velocity_update(&swarm, SS);
      printf("it: %d, gbest: %f\n", i, swarm.v_gbest);
    }
  }

  return 0;
}
