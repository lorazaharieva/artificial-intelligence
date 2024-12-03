#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cmath>

struct Item 
{
    int weight;
    int value;
};

const int POPULATION_SIZE = 1300;
const int MAX_GENERATIONS = 200;
const int SELECTION_SIZE = 150;

int fitness(const std::vector<bool>& chromosome, const std::vector<Item>& items, int capacity) 
{
    int totalValue = 0;
	int totalWeight = 0;

    for (int i = 0; i < chromosome.size(); ++i) 
	{
        if (chromosome[i]) 
		{
            totalValue += items[i].value;
			totalWeight += items[i].weight;
        }
    }

    int penalty = totalValue - (totalWeight - capacity) * 10;
    
	return totalWeight <= capacity ? totalValue : penalty;
}

std::vector<bool> randomChromosome(int size, const std::vector<Item>& items, int capacity) 
{
    std::vector<bool> chromosome(size);
    int currWeight = 0;

    for (int i = 0; i < size; ++i) 
	{
		int myRand = std::rand();

        if ((currWeight + items[i].weight <= capacity) && (myRand % 2)) 
		{
            chromosome[i] = true;
            currWeight += items[i].weight;
        } 
		else 
		{
            chromosome[i] = false;
        }
    }
    return chromosome;
}

std::pair<std::vector<bool>, std::vector<bool>> crossover(const std::vector<bool>& parent1, const std::vector<bool>& parent2) 
{
    int size = parent1.size();

    int point1 = std::rand() % size;
    int point2 = std::rand() % size;

    if (point1 > point2) 
	{
		std::swap(point1, point2);
	}

    std::vector<bool> offspring1(size), offspring2(size);

    for (int i = 0; i < size; ++i) 
	{
        if (i >= point1 && i <= point2) 
		{
            offspring1[i] = parent2[i];
            offspring2[i] = parent1[i];
        } 
		else 
		{
            offspring1[i] = parent1[i];
            offspring2[i] = parent2[i];
        }
    }
    
	return {offspring1, offspring2};
}

void mutate(std::vector<bool>& chromosome, double mutationRate) 
{
    for (int i = 0; i < chromosome.size(); ++i) 
	{
		double myRand = (double)std::rand();

        if (myRand / RAND_MAX < mutationRate) 
		{
            chromosome[i] = !chromosome[i];
        }
    }
}

std::vector<std::vector<bool>> selectTop(const std::vector<std::vector<bool>>& population, const std::vector<int>& scores, int topSize) 
{
    std::vector<std::pair<int, std::vector<bool>>> scoredPopulation;

    for (int i = 0; i < population.size(); ++i) 
	{
        scoredPopulation.emplace_back(scores[i], population[i]);
    }

    std::sort(scoredPopulation.rbegin(), scoredPopulation.rend());
    std::vector<std::vector<bool>> selected;

    for (int i = 0; i < topSize; ++i) 
	{
        selected.push_back(scoredPopulation[i].second);
    }

    return selected;
}

int main() 
{
    int capacity, numItems;
    std::cin >> capacity >> numItems;

    std::vector<Item> items(numItems);
    for (int i = 0; i < numItems; ++i) 
	{
        std::cin >> items[i].weight >> items[i].value;
    }

    std::vector<std::vector<bool>> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) 
	{
        population[i] = randomChromosome(numItems, items, capacity);
    }

    int bestFitness = 0;

    for (int generation = 1; generation <= MAX_GENERATIONS; ++generation) 
	{
        std::vector<int> scores(POPULATION_SIZE);

        for (int i = 0; i < POPULATION_SIZE; ++i) 
		{
            scores[i] = fitness(population[i], items, capacity);
        }

        bestFitness = std::max(bestFitness, *std::max_element(scores.begin(), scores.end()));

        if (generation == 1 || generation % 5 == 0 || generation == MAX_GENERATIONS) 
		{
            std::cout << "Generation " << generation << ": " << bestFitness << std::endl;
        }

        auto selected = selectTop(population, scores, SELECTION_SIZE);

        std::vector<std::vector<bool>> newPopulation;
        while (newPopulation.size() < POPULATION_SIZE) 
		{
            int p1 = std::rand() % SELECTION_SIZE;
            int p2 = std::rand() % SELECTION_SIZE;

            auto [child1, child2] = crossover(selected[p1], selected[p2]);

            double currMutationRate = 0.1 * (1.0 - (double)generation / MAX_GENERATIONS);
			mutate(child1, currMutationRate); 
            mutate(child2, currMutationRate);

            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }
        population = newPopulation;
    }
    return 0;
}
