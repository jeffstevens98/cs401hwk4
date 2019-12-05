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
#include<cstdlib>
#include<ctime>
#include<string>
#include<set>
#include<functions.cpp>


int randomBeeWithException(int exception, int range)
/* This function returns a random number from a range of numbers that excludes one of them numbers in between
 * exception - The number to be excluded
 * range - the size of the range of numbers to pick a number from. 0-to-range.
 */
{
	std::set<int> possibleBees;
	for(int i = 0; i < numberOfEmployedBees; i++)
	{
		if (i != exception)
		{
			possibleBees.insert(i);
		}
	}
	int choice = rand() % possibleBees.size();
	int count = 0;
	int randomBee = -1;
	std::set<int>::iterator it;
	for (it = possibleBees.begin(); true; ++it)
	{
		if (count == choice)
		{
			randomBee = *it; // Note the "*" here. It is needed to dereference the iterator object.
			return randomBee;
		}
		count++;
	}
}


vector<double> rouletteSelection(vector< vector<double> > solutionPopulation, vector<double> fitnesses)
/* Returns a solution from the population of solutions based upon a selection where the  probability of selection is dependent upon the fitness of a solution.
 * solutionPopulation - the solutions that the selection will be performed upon
 * fitnesses - a vector of each fitness value of each solution
 */
{
	//Sum up all the fitnesses
	double sumOfAllFitnesses = 0;
	for(int i = 0; i < fitnesses.size(); i++)
	{
		sumOfAllFitness += fitnesses[i];
	}
	//Find the probability of each solution to be selected, also create ranges of possible probabilities to select from
	vector<double> probabilities;
	probabilities.resize(solutionPopulation.size());
	double upperBoundOfProbability = 0;
	vector< vector<double> > probabilityRanges;
	probabilityRanges.resize(solutionPopulation.size())
	vector<double> nestedVec;
	nestedVec.resize(2);
	for(int i = 0; i < probabilities.size(); i++)
	{
		probabilities[i] = (fitnesses[i])/(sumOfAllFitnesses);
		probabilityRanges[i].push_back(nestedVec);
		probabilityRanges[i][0] = upperBoundOfProbability;
		probabilityRanges[i][1] = upperBoundOfProbability + probability[i];
		upperBoundOfProbability += probabiliity
	}
	//Select a random number between zero and one
	double random = static_cast <double> (rand() /( static_cast <double> (RAND_MAX/(1)));
	int chosenIndex = -1; //the index that will be chosen
	//Iterate through our ranges of probabilities to pick the correct one
	for(int i = 0; i < probabilityRanges.size(); i++)
	{
		if ((random > probabilityRanges[i][0]) && (random < probabilityRanges[i][1]))
		{
			chosenIndex = i; //this is the index of the solution that we will select
		}
	}
	
	return solutionPopulation[i];
}


double abcAlgorithm(double (*fitnessFunction)(), double searchSpaceLowerBound, double searchSpaceUpperBound, int dimensions, string minOrMax = "max")
/* The artificial bee colony algorithm - Used to optimize functions.
 * (*fitnessFunction)() - a function pointer that is used to pass the names of functions as parameters. These functions are the functions being optimized.
 * searchSpaceLowerBound - the lower bound of the search space for the function to be optimized
 * searchSpaceUpperBound - the upper bound of the search space for the function to be optimized
 * dimensions - the amount of dimensions the function to be optimized requires
 * mixOrmax - a string value dictating whether to find the global maximum or global minimum of the function to be optimized. Enter "max" for finding max, enter "min" for finding min.
 */
{
	double bestSolution = 0;
	int numberOfEmployedBees = 100;
	int numberOfOnlookerBees = numberOfEmployedBees;
	vector< vector<double> > employedBees;
	employedBees.resize(numberOfEmployedBees);
	vector< vector<double> > onlookerBees;
	onlookerBees.resize(numberOfOnlookerBees);
	
	//Initialization - the initial food sources are located
	for(int i = 0; i < numberOfEmployedBees; i++)
	{
		for(int d = 0; d < dimensions; d++)
		{ 
			//Generate a random value within our solution space for a single dimension
			employedBees[i].push_back((searchSpaceLowerBound) + static_cast <float> (rand() /( static_cast <float> (RAND_MAX/(searchSpaceUpperBound-(searchSpaceLowerBound))))));
		}
	}
	//This bit of code is the same as above, we are just giving initial solutions to the second half of the swarm; the onlooker bees
	for(int i = 0; i < numberOfOnlookerBees; i++)
	{
		for(int d = 0; d < dimensions; d++)
		{ 
			onlookerBees[i].push_back((searchSpaceLowerBound) + static_cast <float> (rand() /( static_cast <float> (RAND_MAX/(searchSpaceUpperBound-(searchSpaceLowerBound))))));
		}
	}
	
	//Main Algorithm - optimization of the population of solution population
	int iter = 0;
	vector<int> foodSourceLifespans;//The amount of iterations each food source has gone through without improving. After a set limit, these food sources will be discarded.
	foodSourceLifespans.resize(numberOfEmployedBees);
	int foodSourceLifespanLimit = 10; //The maximum amount of iterations a food source is allowed to be a part of the population without improving.
	vector<double> fitnesses; //vector of all of the fitnesses of the employed bee population
	fitnesses.resize(numberOfEmployedBees);
	while(iter < 100) //tentative aspiration critera
	{
		//Each employed bee conducts a local search for a new food source based upon their current saved food source
		for(int eB = 0; eB < numberOfEmployedBees; eB++)
		{
			int randomDimension = rand() % dimensions; //pick a dimension to search within
			double betweenNeg1and1 = (-1.0) + static_cast <double> (rand() /( static_cast <double> (RAND_MAX/(1.0-(-1.0))))) //random float between negative 1 and 1
			int randomBee = randomBeeWithException(eB,numberOfEmployedBees); //Pick a random employed bee that's not eB, the number of the current employed bee.
			employedBees[eB][randomDimension] = employedBees[eB][randomDimension] + (betweenNeg1and1 * (employedBees[eB][randomDimension] - employedBees[randomBee][randomDimension]);
		}
		//Now, the onlooker bees learn about each food source from each of the employed bees. The onlooker bees then decide which food source they want to visit based on roulette selection.
		//Evaluate fitnesses of the employed bees first
		for(int eB = 0; eB < numberOfEmployedBees; eB++)
		{
			fitnesses[eB] = fitnessFunction(employedBees[eB]);
		}
		for(int oB = 0; oB < numberOfOnlookerBees; oB++)
		{
			onlookerBees[oB] = rouletteSelection(employedBees,fitnesses);
		}
		//If a solution has existed within our population for longer than the allowed food source lifespan, it is discarded and a scout bee will find a new one.
		////////////////////
		//////////////////// work on this right here, champ!
		////////////////////
		//Send the onlookers bees out as employed bees for the next iteration of the algorithm.
		employedBees = onlookerBees;
	}
	//When the main iterations of the algorithm are complete, we use a greedy selection to return our best solution from the population.
	for(int eB = 0; eB < //fitnesses.size(); eB++)
	{
		////
	}
	return bestSolution;
}


int main()
//Calls all of our optimizations and displays their results.
{
	srand(time(NULL)); //set random seed
	
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
