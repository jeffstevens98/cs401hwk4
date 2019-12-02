/* Artificial Bee Colony Algorithm Implementation
 * Jeff Stevens && Dylan Tallman
 * CS 401
 * Dr. Sevkli
 * 12/5/19
 */

#include<iostream>
#include<vector>
#include<cmath>
#include<cstdio>
//#include<functions.cpp>


//Test function prototypes
double Ackley()
{
	return 0;
}


double Sphere()
{
        return 0;
}


double Easom()
{
        return 0;
}


double Rastrigin()
{
        return 0;
}


double sumOfPowers()
{
        return 0;
}



double abcAlgorithm(double (*fitnessFunction)())
{
	double bestSolution = 0;
	return bestSolution;
}

int main()
{
	printf("Artificial Bee Colony Function Optimization Results\n");
	printf("Ackley Function:");
	double ackleyOptimized = abcAlgorithm(Ackley);
	printf(" %f\n", ackleyOptimized);
	printf("Sphere Function:");
	double sphereOptimized = abcAlgorithm(Sphere);
	printf(" %f\n", sphereOptimized);
	printf("Easom Function:");
	double easomOptimized = abcAlgorithm(Easom);
	printf(" %f\n", easomOptimized);
	printf("Rastrigin Function:");
	double rastriginOptimized = abcAlgorithm(Rastrigin);
	printf(" %f\n", rastriginOptimized);
	printf("Sum of Powers Function:");
	double sumOfPowersOptimized = abcAlgorithm(sumOfPowers);
	printf(" %f\n", sumOfPowersOptimized);
}
