/*
     Title:	lexicographic.cpp  
     Author: Jacob Sullivan
     Date:	11/6/23
     Purpose:   The purpose of this file is to hold the lexicogrpahical function
                which is used in program2.cpp for finding all the permutaions and sending 
                over the new permuatation to the saveTour functions to be printed and saved. 
*/

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//Tour structure as a global variable
struct Tour
{
	string tour[SIZE];
	int cost;
};

//Function prototype
void saveTour(Tour*, string*, GraphMatrix*);

/*
    Title: lexicographic
    Purpose: Implements a lexicographical permutation algorithim to generate all 
    permutations of the countries
*/
void lexicographic(string* countries, int size, Tour* tourOptions, GraphMatrix* matrix)
{
    int iterationCount = 1;//Tracks permutations generated
    bool isDone = false;//Flag to determine when all permutaions have been generated

    cout << "\nLEXICOGRAPHIC ALGORITHM\n";
    cout << "\n" << iterationCount << ":\t";//Prints the inital first permutations
    
    for (int i = 0; i < size; ++i)//Outputs the country codes in current order
    {
        cout << countries[i] << " ";
    }

    saveTour(tourOptions, countries, matrix);// Saves the current permutaion
    iterationCount++;//Increments for the next permutation

    while (!isDone)// Continue till all permuations are made
    {
        cout << "\n" << iterationCount << ":\t"; //Iteration count print

        iterationCount++;//Iterates the count

        /*
            ---FIND lowBound OR DETERMINE IF DONE---
        */
        isDone = true;
        int lowBound = 0;
        for (int i = 0; i < size - 1; i++)//Pivot for the next permutaion
        {
            if (countries[i] < countries[i + 1])
            {
                isDone = false;//There is at least one more permutaion
                lowBound = i;// updates the new lowbound with the new pivot
            }
        }

        if (isDone)
        {
            continue;//skip remaining steps and check condition
        }

        /*
            ---FIND uppBound---
        */
        int uppBound = 0;//Swapped with the pivot
        for (int j = size - 1; j > 0; j--)
        {
            if (countries[j] > countries[lowBound])
            {
                uppBound = j;//Found a character for the swap
                break;
            }
        }

        /*
            ---SWAP ELEMENTS---
        */
        string temp = countries[lowBound];
        countries[lowBound] = countries[uppBound];
        countries[uppBound] = temp;

        /*
            ---REVERSE ELEMENTS FROM lowBound+1 TO size-1---
        */
        for (int i = lowBound + 1, j = size - 1; i < j; ++i, --j)
        {
            string temp = countries[i];
            countries[i] = countries[j];
            countries[j] = temp;
        }

        for (int i = 0; i < size; ++i)//Outputs a new permutaion
        {
            cout << countries[i] << " ";
        }

        saveTour(tourOptions, countries, matrix);//Saves the permutaions as a new tour
        
        if(iterationCount > 40320)//Breaks loop if expected number of permutaions has been reached
        {
            break;
        }
    }
}