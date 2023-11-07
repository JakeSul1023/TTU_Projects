/*
     Title:	program2.cpp   
     Author:	Jacob Sullivan
     Date:	11/6/23
     Purpose:   Traveling Salesman - find the lowest cost
                tour when traveling from US to 8 other countries
                and then back to US.
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include "GraphMatrix.h"
#include "lexicographic.cpp"
using namespace std;

//Global varibles for tracking the lowest cost tour and the set of country codes
Tour lowestTour;
int lowestCost = INT_MAX;	//Max int value 
const string COUNTRY_CODES[SIZE]={"AU", "BR", "CA", "CN", "GL", "IT", "NA", "RU", "US", "US"};

//Function Prototypes
int searchCountryCode(string);
GraphMatrix* readFileMakeMatrix();
void printStringArray(string*, int);
void saveTour(Tour*, string*, GraphMatrix*);
void findLowest(Tour*, int);

/* 
	Title: main
	Entry point of the program that handles dynamic allocation along with function calls 
	and printing the results of the lowest costing trips. This main function also handles
	printing all the countries codes. At the end releasing all the dynamicallyallocated variables.
*/
int main()
{	
	Tour *tourOptions = new Tour[40320];//Handles all the possible tours
	//read in the flight information from the file and then create the weight matrix
	GraphMatrix * matrix = readFileMakeMatrix();//varible to hold flight data
	string * countries = new string[SIZE-2];//Holding country codes besides the 2 US codes

	cout << "\n\n*************************COUNTRIES*******************\n";
	//Prints all the available country codes minus the US
	for(int x=0; x < SIZE-2; x++)
	{
		countries[x] = COUNTRY_CODES[x];
		cout << countries[x] << endl;
	}
	
	//generate all possible tours (starting & ending with "US") using lexicographic permute algorithm
	//you will need to call your lexicographic function, sending the modified countries array with the 8 country codes
	lexicographic(countries, SIZE - 2, tourOptions, matrix);//Generates all possible tours and their costs
	
	cout << "\n\n*************************SOLUTION*******************\n";

    findLowest(tourOptions, 40320); // Assuming 40320 tours have been generated print out all the new lowest costing tours

	cout << "\nHappy Traveling!\n";
	
	//Releases all the dynamically alloccated memory
	delete matrix;
	delete [] tourOptions;
	delete [] countries;
	
	return 0;
}
/*
	Function: searchCountryCode
	Parameters: a string with the country code like "BR"
	Returns: an integer representing this country's index in the GraphMatrix.
	For example, if the parameter is "BR", it should return index 1.  If the parameter is "CA", it should return index 3
	It is returning the index value in the COUNTRY_CODES array.	
	Implement this search function with the binary search algorithm!
*/
int searchCountryCode(string code)
{
	int left = 0;
	int right = SIZE - 1;//Sets the right boundary 

	while(left <= right)
	{
		int cntr = (left + right) / 2;//Center position

		if(COUNTRY_CODES[cntr] == code)//If the center matches the code it returns the center index
		{
			return cntr;
		}
		else if(COUNTRY_CODES[cntr] < code)//If the center is less than the code it narrows the search to the right half
		{
			left = cntr + 1;
		}
		else//If the center is greater is narrows the search to the left half
		{
			right = cntr -1;
		}
	}

	return -1;//Returns if code is not found
}

/*
	Title: readFileMakeMatrix
	Purpose:  this function will read the data in flights.txt and create an
	adjacency matrix with the couuntry codes.  The matrix values will be the
	cost of the flights. (DID NOT TOUCH)
*/
GraphMatrix* readFileMakeMatrix()
{
	ifstream inFile;
	GraphMatrix * matrix = new GraphMatrix(SIZE-1);
	cout << "\nCreated the matrix.";
	string country1, country2;
	int price;
	inFile.open("flights.txt");
	cout << "\nReading from flights.txt\n";
	if(inFile)
	{
		while(inFile >> country1)
		{
			inFile >> country2;
			inFile >> price;
			//add price to graph matrix
			matrix->addEdge(searchCountryCode(country1), searchCountryCode(country2), price);
			cout << "\nAdded edge from " << country1 << " to " << country2 << " with cost of $" << price;
		}
	}
	else
		cout << "\nSorry, I am unable to open the file.\n";
	inFile.close();
	cout << "\n\nFLIGHT PRICE GRAPH MATRIX\n";
	matrix->printGraph();
	cout << endl;
	return matrix;
}

/*
	Title: printStringArray
	Purpose:  this function will print the array of strings with a space
	between each string
*/
void printStringArray(string* arr, int size)
{
	for(int x = 0; x < size; x++)
	{
		cout << arr[x] << " ";//Prints each country code
	}

	cout << endl;
}

/*
	Title: saveTour
	Purpose: Appends the US to the front and end of the array. 
	Saves the current tour and its total coset into tourToptions array.
	While also printing out each tour and its weight to the screen. 
*/
void saveTour(Tour* tourOptions, string* countries, GraphMatrix* matrix)
{
	static int tourCount = 0;//Number of tours in total
	int cost = 0;//Accumulator of the cost of the tours

	tourOptions[tourCount].tour[0] = "US";//Appends US to the start
	
	for(int i = 0; i < SIZE - 2; ++i)//Copy the countries into the tour variable
	{
		tourOptions[tourCount].tour[i + 1] = countries[i];
	}

	tourOptions[tourCount].tour[SIZE - 1] = "US";//Appends the US to the end


	for(int i = 0; i < SIZE - 1; ++i)//Calculates the cost of the tours
    {
        int c1 = searchCountryCode(tourOptions[tourCount].tour[i]);//Starting index
        int c2 = searchCountryCode(tourOptions[tourCount].tour[i + 1]);//Next counrty index
        int edgeCost = matrix->getWeight(c1, c2);//Gets the weight of the edge
        cost += edgeCost;//Accumulates the total cost
        
        cout << edgeCost << " ";//Prints the weights to the screen
    }

	tourOptions[tourCount].cost = cost;//Assigns the calculated cost of the tour


	cout << "  has a tour weight: " << cost << endl;

	tourCount++;//Increments the tour count for the next tour to be processed
}

/*
	Title: findLowest
	Purpose: Finds and prints the tour with the newest lowest cost from the stored
	array of tours. Finally finishing with the lowest costing tour printing it to the
	screen.
*/
void findLowest(Tour* tourOptions, int numTours) 
{	
	if (numTours == 0) 
	{
        cout << "No tours to compare." << endl;
        return; // No tours to compare
    }

    int lowestCost = tourOptions[0].cost; // Start with the cost of the first tour
    int lowestIndex = 0;

    // Iterate over all tours to find the one with the lowest cost
    for (int i = 1; i < numTours; ++i) 
	{
        if (tourOptions[i].cost < lowestCost)//Compares the tour costs
		{
            lowestCost = tourOptions[i].cost;//Assigns the tour cost
            lowestIndex = i;
			cout << "New Lowest is: " << lowestCost << " with tour ";//Prints the newest lowest cost to the screen

    		for (int i = 0; i < SIZE; ++i) //Prints the actual tour codes to the screen
			{
       			cout << tourOptions[lowestIndex].tour[i] << " ";
   			}

			cout << endl;
        }
    }

    // Print the lowest cost and the corresponding tour
	cout << "\n\n**********************************************************\n";
    cout << "The tour with the lowest cost of $" << lowestCost << " is ";//Prints the lowest costing tour
    for (int i = 0; i < SIZE; ++i) 
	{
        cout << tourOptions[lowestIndex].tour[i] << " ";
    }
}