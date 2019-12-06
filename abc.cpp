/* Artificial Bee Colony Algorithm Implementation
 * Jeff Stevens && Dylan Tallman
 * CS 401
 * Dr. Sevkli
 * 12/5/19
 */

#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>
#include<set>
#include"functions.cpp"


void printVec(std::vector< std::vector<double> > vec)
//Prints the contents of a vector. Used for debugging.
{
	printf("vector size after being passed in %d\n", vec.size());
	for(int i = 0; i < vec.size(); i++)
	{
		printf("subvector %d:",i);
		for (int j = 0; j < vec[i].size(); j++)
		{
			printf(" %f ",vec[i][j]);
		}
		printf("\n");
	}
}


int randomBeeWithException(int exception, int range)
/* This function returns a random number from a range of numbers that excludes one of them numbers in between
 * exception - The number to be excluded
 * range - the size of the range of numbers to pick a number from. 0-to-range.
 */
{
	std::set<int> possibleBees;
	for(int i = 0; i < range; i++)
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


std::vector<double> rouletteSelection(std::vector< std::vector<double> > solutionPopulation, std::vector<double> fitnesses, std::string minOrMax = "max")
/* Returns a solution from the population of solutions based upon a selection where the  probability of selection is dependent upon the fitness of a solution.
 * solutionPopulation - the solutions that the selection will be performed upon
 * fitnesses - a vector of each fitness value of each solution
 */
{
	bool DEBUG = false;
	
	DEBUG? printf("initialized roulette selection\n") : 0;
	//Sum up all the fitnesses
	double sumOfAllFitnesses = 0.0;
	for(int i = 0; i < fitnesses.size(); i++)
	{
		sumOfAllFitnesses += fitnesses[i];
	}
	DEBUG? printf("summed all fitnesses\n") : 0;
	//Find the probability of each solution to be selected, also create ranges of possible probabilities to select from
	std::vector<double> probabilities;
	probabilities.resize(solutionPopulation.size());
	double upperBoundOfProbability = 0;
	std::vector< std::vector<double> > probabilityRanges;
	std::vector<double> nestedVec;
	nestedVec.resize(2);
	DEBUG? printf("initialized vectors for probability ranges\n") : 0;
	for(int i = 0; i < probabilities.size(); i++)
	{
		if (minOrMax == "max")
		{
			probabilities[i] = (fitnesses[i])/(sumOfAllFitnesses);
		}
		if (minOrMax == "min")
		{
			probabilities[i] = (1/fitnesses[i])/(sumOfAllFitnesses);
		}
		probabilityRanges.push_back(nestedVec);
		probabilityRanges.back()[0] = upperBoundOfProbability;
		probabilityRanges.back()[1] = upperBoundOfProbability + probabilities[i];
		upperBoundOfProbability += probabilities[i];
	}
	DEBUG? printf("probability ranges calculated\n") : 0;
	//Select a random number between zero and one
	double random = static_cast <double> (rand() /( static_cast <double> (RAND_MAX/(1))));
	int chosenIndex = -1; //the index that will be chosen
	//Iterate through our ranges of probabilities to pick the correct one
	for(int i = 0; i < probabilityRanges.size(); i++)
	{
		if ((random >= probabilityRanges[i][0]) && (random < probabilityRanges[i][1]))
		{
			DEBUG? printf("solution picked!\n") : 0;
			chosenIndex = i; //this is the index of the solution that we will select
			break;
		}
	}
	DEBUG? printf("roulette selection complete\n") : 0;
	return solutionPopulation[chosenIndex];
}


std::vector<double> abcAlgorithm(std::string fitnessFunctionName, double searchSpaceLowerBound, double searchSpaceUpperBound, int dimensions, std::string minOrMax = "max")
/* The artificial bee colony algorithm - Used to optimize functions.
 * (*fitnessFunction)() - a function pointer that is used to pass the names of functions as parameters. These functions are the functions being optimized.
 * searchSpaceLowerBound - the lower bound of the search space for the function to be optimized
 * searchSpaceUpperBound - the upper bound of the search space for the function to be optimized
 * dimensions - the amount of dimensions the function to be optimized requires
 * mixOrmax - a string value dictating whether to find the global maximum or global minimum of the function to be optimized. Enter "max" for finding max, enter "min" for finding min.
 */
{
	bool DEBUG = false;
	DEBUG? printf("\nstarting abc algorithm...\n") : 0;
	std::vector<double> bestSolution;
	int numberOfEmployedBees = 100;
	int numberOfOnlookerBees = numberOfEmployedBees;
	std::vector< std::vector<double> > employedBees;
	employedBees.resize(numberOfEmployedBees);
	std::vector< std::vector<double> > onlookerBees;
	onlookerBees.resize(numberOfOnlookerBees);
	DEBUG? printf("allocated bee population vectors\n") : 0;
	
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
	DEBUG? printf("bee population initialized\n") : 0;
	
	//Main Algorithm - optimization of the population of solution population
	int iter = 0;
	std::vector<int> foodSourceLifespans;//The amount of iterations each food source has gone through without improving. After a set limit, these food sources will be discarded.
	foodSourceLifespans.resize(numberOfEmployedBees);
	int foodSourceLifespanLimit = 10; //The maximum amount of iterations a food source is allowed to be a part of the population without improving.
	std::vector<double> fitnesses; //vector of all of the fitnesses of the employed bee population
	fitnesses.resize(numberOfEmployedBees);
	DEBUG? printf("starting main algorithm...\n") : 0;
	while(iter < 100) //tentative aspiration critera
	{
		DEBUG? printf("iteration #%d\n",iter) : 0;
		//Each employed bee conducts a local search for a new food source based upon their current saved food source
		for(int eB = 0; eB < numberOfEmployedBees; eB++)
		{
			int randomDimension = rand() % dimensions; //pick a dimension to search within
			double betweenNeg1and1 = (-1.0) + static_cast <double> (rand() /( static_cast <double> (RAND_MAX/(1.0-(-1.0)))));//random float between negative 1 and 1
			int randomBee = randomBeeWithException(eB,numberOfEmployedBees); //Pick a random employed bee that's not eB, the number of the current employed bee.
			employedBees[eB][randomDimension] = employedBees[eB][randomDimension] + (betweenNeg1and1 * (employedBees[eB][randomDimension] - employedBees[randomBee][randomDimension]));
		}
		DEBUG? printf("local search of employed bees complete\n") : 0;
		//Now, the onlooker bees learn about each food source from each of the employed bees. The onlooker bees then decide which food source they want to visit based on roulette selection.
		//Evaluate fitnesses of the employed bees first
		double fitnessCheckArray2[2];
		double fitnessCheckArray10[10];
		for(int eB = 0; eB < numberOfEmployedBees; eB++)
		{
			if (fitnessFunctionName == "Ackley")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				fitnesses[eB] = Ackley(fitnessCheckArray2);
			}
			else if (fitnessFunctionName == "Easom")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				fitnesses[eB] = Easom(fitnessCheckArray2);
			}
			else if (fitnessFunctionName == "Rastrigin")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				fitnesses[eB] = Rastrigin(fitnessCheckArray2);
			}
			else if (fitnessFunctionName == "Sphere")
			{
				//Turn our vector into an array
				double beeArray[10];
				for(int i = 0; i < 10; i++)
				{
					beeArray[i] = employedBees[eB][i];
				}
				fitnesses[eB] = Sphere(beeArray);
			}
			else if (fitnessFunctionName == "SumOfPowers")
			{
				for(int i = 0; i < 10; i++)
				{
					fitnessCheckArray10[i] = employedBees[eB][i];
				}
				fitnesses[eB] = SumOfPowers(fitnessCheckArray10);
			}
		}
		DEBUG? printf("Fitness of new solutions evaluated\n") : 0;
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
		DEBUG? printf("Ending iteration and beginning next iteration\n\n") : 0;
		iter++;
	}
	//When the main iterations of the algorithm are complete, we use a greedy selection to return our best solution from the population.
	DEBUG? printf("Evaluating our population to find our best solution now\n") : 0;
	bestSolution = employedBees[0]; //give best solution an initial value
	double bestSolutionArray2[2];
	double bestSolutionArray10[10];
	double fitnessCheckArray2[2]; //used to input the values from our vectors into the fitness functions
	double fitnessCheckArray10[10]; // ||
	for(int eB = 0; eB < employedBees.size(); eB++)
	{
		if (minOrMax == "max")
		{
			if (fitnessFunctionName == "Ackley")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Ackley(fitnessCheckArray2) >= Ackley(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Easom")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Easom(fitnessCheckArray2) >= Easom(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Rastrigin")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Rastrigin(fitnessCheckArray2) >= Rastrigin(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Sphere")
			{
				double beeArray[10];
				double bestSolutionArray[10];
				for(int i = 0; i < 10; i++)
				{
					beeArray[i] = employedBees[eB][i];
					bestSolutionArray[i] = bestSolution[i];
				}
				if (Sphere(beeArray) >= Sphere(bestSolutionArray))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "SumOfPowers")
			{
				for(int i = 0; i < 10; i++)
				{
					fitnessCheckArray10[i] = employedBees[eB][i];
					bestSolutionArray10[i] = bestSolution[i];
				}
				if (SumOfPowers(fitnessCheckArray10) >= SumOfPowers(bestSolutionArray10))
				{
			 		bestSolution = employedBees[eB];
				}
			}
		}
		else if (minOrMax == "min")
		{
			if (fitnessFunctionName == "Ackley")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Ackley(fitnessCheckArray2) <= Ackley(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Easom")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Easom(fitnessCheckArray2) <= Easom(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Rastrigin")
			{
				fitnessCheckArray2[0] = employedBees[eB][0];
				fitnessCheckArray2[1] = employedBees[eB][1];
				bestSolutionArray2[0] = bestSolution[0];
				bestSolutionArray2[1] = bestSolution[1];
				if (Rastrigin(fitnessCheckArray2) <= Rastrigin(bestSolutionArray2))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "Sphere")
			{
				double beeArray[10];
				double bestSolutionArray[10];
				for(int i = 0; i < 10; i++)
				{
					beeArray[i] = employedBees[eB][i];
					bestSolutionArray[i] = bestSolution[i];
				}
				if (Sphere(beeArray) <= Sphere(bestSolutionArray))
				{
					DEBUG? printf("bestSolution changed\n") : 0;
					bestSolution = employedBees[eB];
				}
			}
			else if (fitnessFunctionName == "SumOfPowers")
			{
				for(int i = 0; i < 10; i++)
				{
					fitnessCheckArray10[i] = employedBees[eB][i];
					bestSolutionArray10[i] = bestSolution[i];
				}
				if (SumOfPowers(fitnessCheckArray10) <= SumOfPowers(bestSolutionArray10))
				{
			 		bestSolution = employedBees[eB];
				}
			}
		}
	}
	return bestSolution;
}


int main()
//Calls all of our optimizations and displays their results.
{
	srand(time(NULL)); //set random seed
	
	printf("Artificial Bee Colony Function Optimization Results (best minimizations)\n\n");
	printf("Ackley Function: ");
	std::vector<double> ackleyOptimized = abcAlgorithm("Ackley", -32.0, 32.0, 2, "min");
	double ackleyArray[2];
	ackleyArray[0] = ackleyOptimized[0];
	ackleyArray[1] = ackleyOptimized[1];
	printf("%f\nInput variables: %f, %f\n\n", Ackley(ackleyArray), ackleyOptimized[0], ackleyOptimized[1]);
	printf("Sphere Function: ");
	std::vector<double> sphereOptimized = abcAlgorithm("Sphere", -25.0, 25.0, 10, "min");
	double sphereArray[10];
	for (int i = 0; i < 10; i++)
	{
		sphereArray[i] = sphereOptimized[i];
	}
	printf("%f\nInput variables: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n\n", Sphere(sphereArray), sphereArray[0], sphereArray[1], sphereArray[2], sphereArray[3], sphereArray[4], sphereArray[5], sphereArray[6], sphereArray[7], sphereArray[8], sphereArray[9]);
	printf("Easom Function: ");
	std::vector<double> easomOptimized = abcAlgorithm("Easom", -5.12, 5.12, 2, "min");
	double easomArray[2];
	easomArray[0] = easomOptimized[0];
	easomArray[1] = easomOptimized[1];
	printf("%f\nInput variables: %f, %f\n\n", Easom(easomArray), easomOptimized[0], easomOptimized[1]);
	printf("Rastrigin Function: ");
	std::vector<double> rastriginOptimized = abcAlgorithm("Rastrigin", -100.0, 100.0, 2, "min");
	double rastriginArray[2];
	rastriginArray[0] = rastriginOptimized[0];
	rastriginArray[1] = rastriginOptimized[1];
	printf("%f\nInput variables: %f, %f\n\n", Rastrigin(rastriginArray), rastriginOptimized[0], rastriginOptimized[1]);
	printf("Sum of Powers Function:");
	std::vector<double> sopOptimized = abcAlgorithm("SumOfPowers", -1, 1, 10, "min");
	double sopArray[10];
	for (int i = 0; i < 10; i++)
	{
		sopArray[i] = sopOptimized[i];
	}
	printf("%f\nInput Variables: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n\n", SumOfPowers(sopArray), sopArray[0], sopArray[1], sopArray[2], sopArray[3], sopArray[4], sopArray[5], sopArray[6], sopArray[7], sopArray[8], sopArray[9]);
}
