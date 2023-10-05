/*
    Author: Jacob Sullivan (jtsullivan43)
    Date: 9/5/23
    Purpose: The purpose of this program is to perform GCF finding algorithims/methods. With a menu option to allow the user
    to input which method they would like to use. In total all will get the same answer after choosing a menu choice. There are 3
    functions implemented for each of the methods with their own descriptions describing how it works. 
*/
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

int bookExercise(int, int, int*, int*);
int consecIntChecker(int, int);
int middleSchool(int, int);

int main(int inputct, char* inputvar[])
{
    if(inputct != 3)    //Checks to see for correct number of arguments in the command line
    {
        cout << "Error! Please type: <./program.exe> <number1> <number2>\n\n";
        return 1;   //Returns an error code
    }

    int opt = 0;    //User menu choice
    int gcf = 0;    //Holds the values of the functions

    try{    //Runs unless the catch command is flagged

        int rand1 = stoi(inputvar[1]);  //Users first random number 
        int rand2 = stoi(inputvar[2]);  //Users second random number 

        //Checks to see if the user inputs are both 0
        if(rand1 == 0 && rand2 == 0)
        {
            cout << "\ngcf(" << rand1 <<", " << rand2 << ") = undefined\n\n"; 
            return 0;
        }
        
        //If the user inputs a negative number we negate the sign
        if(rand1 < 0 )
        {
            rand1 *= -1;
        }
        if(rand2 < 0)
        {
            rand2 *= -1;
        }
    

        do{ //Runs the main user interface until the user chooses 4 to quit the program

            //The series of cout statements are for the user menu options
            cout << "\nWelcome to the GCF Machine!\n\n";
            cout << "Please choose a option below:\n";
            cout << "1.) Extended Euclid's Algorithim\n";
            cout << "2.) Consecutive Integer Checking Algorithim\n";
            cout << "3.) Middle-School Method\n";
            cout << "4.) End the Program\n\n";
            cout << "Choice: ";
            cin >> opt;

            while(!cin.good())  //Checks to make sure the user inputs an integer
            {
                cin.clear();
                cin.ignore();
                
                cout << "Invaild input! Please only enter integers.\nChoice: ";
                cin >> opt;
            }

            while(opt < 1 || opt > 4)   //Verifies if the user input a number between 1 and 4
            {
                cout << "Please input a number between 1 and 4!\nChoice: ";
                cin >> opt;
            }

            int x = 0;  //Variables for the bookExercise() to hold the results 
            int y = 0;  //Variables for the bookExercise() to hold the results

            switch(opt)    //Swtich statement based on users choice
            {
                case 1 :
                    gcf = bookExercise(rand1, rand2, &x, &y);
                    cout << "\ngcf(" << rand1 <<", " << rand2 << ") = " << gcf << "\n\n";
                    break;

                case 2 :
                    gcf = consecIntChecker(rand1, rand2);
                    cout << "\ngcf(" << rand1 <<", " << rand2 << ") = " << gcf << "\n\n";
                    break;

                case 3 :
                    gcf = middleSchool(rand1, rand2);
                    cout << "\ngcf(" << rand1 <<", " << rand2 << ") = " << gcf << "\n\n";
                    break;
            }

        }while(opt != 4);

        cout << "You've chosen option 4. Goodbye!\n\n";
    }

    
    catch(const invalid_argument&) //Will return an error if the inputs from the user are invalid
    {
        cout << "The arguments provided are not vaild integers! Ending program.\n\n";
        return 1;
    }
    
    return 0;
}

/*
    The function implements the Extended Euclidean Algorithim to find the GCF of two numbers 
    given by the user and find the coefficients for x and y to implement the full Extended 
    algorithim. This function is done with the use of recursive calls and returning the gcf.
*/
int bookExercise(int rand1, int rand2, int* x, int* y)
{
    if(rand1 == 0)  //Base case
    {
        *x = 0;
        *y = 1;

        return rand2;
    }

    int gcf = bookExercise(rand2%rand1, rand1, x, y);   //Recursive call along with finding rand1%rand2

    //Updates x and y with the results of the recursive function and storing the value inside a temp variable 
    int temp = *y - (rand2/rand1) * *x;
    *y = *x;
    *x = temp;

    return gcf;
};

/*
    This function takes in two integers and assignments and finds the min and max values out of the pair taken in.
    Then loops through the small number and then comparing the 2 numbers with the smallest value. Using the modulus operation
    checking to see if its a factor of both. Then decrementing the small number and checking again.
*/
int consecIntChecker(int rand1, int rand2)
{

    if(rand1 == 0)  //Base case
    {
        return rand2;
    }
    if(rand2 == 0)  //Other base case
    {
        return rand1;
    }

    int small;  //Holds the smallest value of the pair
    int val;    //Holds the largest value of the pair

    small = min(rand1, rand2);  //Finds the smaller number of the pair
    val = max(rand1, rand2);    //Find the larger number of the pair

    while(small > 1)
    {
        if(rand1 % small == 0 && rand2 % small == 0)    //Comparing to see if small is a factor
        {
            return small;
        }
        
        small--;    //Decrements the smallest value
    }

    return 1;
};

/*
    This function takes in two integers and then creates two vectors to hold the factors of the two numbers.
    First looping thtough the first number looking for the factors and storing them into the vector. Then after doing
    the same thing with the second value taken in. After storing all the factors it then compares by looping through both
    the first and second vectors then comparing the two vectors and if a match is found ti first stores it and then mark the 
    part of the vector so it can be used again to find a match.
*/

int middleSchool(int rand1, int rand2)
{

    if(rand1 == 0)  //Base case
    {
        return rand2;
    }
    if(rand2 == 0)  //Other base case
    {
        return rand1;
    }

    std::vector<int> primeFactor1;
    std::vector<int> primeFactor2;
    int gcf = 1;

    // Find prime factors of rand1
    for(int i = 2; i <= rand1; ++i) //Looping through the first value given
    {
        while(rand1 % i == 0)   //Checking to see if its a factor
        {
            primeFactor1.push_back(i);  //Adding the new factor to the vector
            rand1 /= i; //Dividing the current value by its factor
        }      
    }
    
    // Find prime factors of rand2
    for(int i = 2; i <= rand2; ++i) //Looping through the second value given
    {
        while(rand2 % i == 0)   //Checking to see if its a factor
        {
            primeFactor2.push_back(i);  //Adding the new factor to the vector
            rand2 /= i; //Dividing the current value by its factor
        }
    }

    // Find common factors and calculate GCF
    for (int i = 0; i < primeFactor1.size(); ++i)   //Loops through the first vector
    {
        for (int j = 0; j < primeFactor2.size(); ++j)   //Loops through the the second vector
        {
            if (primeFactor1[i] == primeFactor2[j] && primeFactor1[i] != -1 && primeFactor2[j] != -1)   //Compares the two vectors then stores it
            {
                gcf *= primeFactor1[i]; //Running of the gcf matches
                primeFactor1[i] = -1;  //Marking this position as used
                primeFactor2[j] = -1;  //Marking this position as used
                break;  
            }
        }
    }

    return gcf;
};