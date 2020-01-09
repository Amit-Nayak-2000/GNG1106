/*
Amit Nayak 300066780 GNG1106 Assignment 5
October 21, 2018
This Program calculates the years require for reforestation based on the amount of forest cut down.
It uses 3 functions to take in, calculate and print the data in a table format for the user.
The program can be run multiple times.
*/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
// Define symbolic constant

typedef struct
{
    char name[15];
    double totalAcres;
    double minUncut;
    double maxUncut;
    double reforRate;
} FOREST;

// Prototypes
double getPositiveValue();
void displayTable(FOREST *, double [2][20]);
void getForestInput(FOREST *);
void calculateTableData(FOREST *, double [2][20]);

/*--------------------------------------------------------------------
Function: main
Description:  Gets from the user values for the equation.
----------------------------------------------------------------------*/
int main()
{
    char quit;

    //The do while loop allows the user to continue calculating or quit the program.
    do{
        FOREST tree;
        FOREST *treePtr;
        treePtr = &tree;
        double points[2][20];

        getForestInput(treePtr);  //gets input
        calculateTableData(treePtr, points);  //calculates answer
        displayTable(treePtr, points);  //prints answer

        fflush(stdin);
        printf("\nDo you want to quit (y/n): ");
        scanf("%c", &quit);
    }while (quit == 'n');

    if (quit == 'y')
        return 0;

}
/*------------------------------------------------------------------------
Function: getPositiveValue
Returns: A value strictly positive (>0)
Description: Reads a real value from the user, checks that it is strictly
             positive, and returns the value.
------------------------------------------------------------------------*/
double getPositiveValue()
{
    double value;
    do
    {
        scanf("%lf",&value);
        if(value <= 0.0)
            printf("The value must be greater than zero: \n");
    }
    while(value <= 0.0);
    return(value);
}
/*-----------------------------------------------------------------------
Function: displayTable
Description: Prints the table values in the 2d array.
------------------------------------------------------------------------*/
void displayTable(FOREST *treePtr, double points[2][20])
{
    //This section of code prints out the table.
    printf("\nForest: %s", (*treePtr).name);
    printf("Uncut Area Min: %0.4f\n", (*treePtr).minUncut);
    printf("Uncut Area Max: %0.4f\n", (*treePtr).maxUncut);
    printf("Reforestation Rate: %.2f\n", (*treePtr).reforRate);
    printf("%10s    %10s\n","Uncut Area Start", "Num Years for Total Reforestation");
    printf("----------------------------------------------\n");
    for(int i = 0; i < 20; i++)
        printf("%10.2f    %10.0f\n", points[0][i], points[1][i]);  //print values using array table.
}
/*
This Function prompts the user to enter the forest characterstics and stores them in the struct of forest via pointers.
This function also prevents any incorrect inputs via while loop.
*/
void getForestInput(FOREST *treePtr)
{
    bool invalidRate = false;
    bool invalidMinUncut = false;
    bool invalidMaxUncut = false;

     //This section of code gets all of the user data to fill all the members of FOREST struct.
    printf("Forest Name: ");
    fgets((*treePtr).name, 15, stdin);
    printf("Total Acres: ");
    (*treePtr).totalAcres = getPositiveValue();
    printf("Acres Uncut Minimum: ");
    (*treePtr).minUncut = getPositiveValue();
    printf("Acres Uncut Maximum: ");
    (*treePtr).maxUncut = getPositiveValue();
    printf("Reforestation Rate: ");
    (*treePtr).reforRate = getPositiveValue();

    //The next section of code checks for an invalid input and asks the user to reinput.
   if((*treePtr).reforRate <= 0 || (*treePtr).reforRate >= 1 ){
            invalidRate = true;
    while (invalidRate){
            printf("Please enter a Reforestation rate between 0 and 1: ");
            (*treePtr).reforRate = getPositiveValue();
            if((*treePtr).reforRate <= 0 || (*treePtr).reforRate >= 1 )
                invalidRate = true;
            else
                invalidRate = false;
        };
    }

    if((*treePtr).minUncut >= (*treePtr).maxUncut){
            invalidMinUncut = true;
    while (invalidMinUncut){
            printf("Please enter a minimum uncut value less than the maximum uncut value: ");
            (*treePtr)->minUncut = getPositiveValue();
            if((*treePtr)->minUncut >= (*treePtr)->maxUncut)
                invalidMinUncut = true;
            else
                invalidMinUncut = false;
        };
    }

    if((*treePtr)->maxUncut >= (*treePtr)->totalAcres){
            invalidMaxUncut = true;
    while (invalidMaxUncut){
            printf("Please enter a maximum uncut value less than the total acres: ");
            (*treePtr).maxUncut = getPositiveValue();
            if((*treePtr)->maxUncut >= (*treePtr)->totalAcres)
                invalidMaxUncut = true;
            else
                invalidMaxUncut = false;
        };
    }
}
//Caclulates the number of years for total reforestation given the inputs and the increments of the user data.
//stores the data in a 2d array for printing.
void calculateTableData(FOREST *treePtr, double points[2][20])
{
    double areaStart;  //starting area
    float inc;  //increment
    inc = ((*treePtr)->maxUncut - (*treePtr)->minUncut) / (19);
    areaStart = (*treePtr).minUncut;

    for(int i = 0; i < 20; i++)
    {
        points[0][i] = areaStart;  //set array points to uncut start area
        points[1][i] = (log((*treePtr).totalAcres / areaStart)) / (log(((*treePtr).reforRate + 1)));  //set array points to total reforestation years
        areaStart = areaStart + inc;  //increment area

    }
}
