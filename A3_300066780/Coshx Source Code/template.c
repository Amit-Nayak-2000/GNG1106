/*
Amit Nayak 300066780 GNG 1106 Assignment 3 October 7 2018.
Cosh(x) calculator.
This program takes in the x value that the user wants to calculate and the n value (the number of terms used in the equation).
The equation comes from the cosh(x) series given in the assignment.
Multiple loops in the program prevent the user from entering invalid input.
A major loop can allow the user to keep calculating values.
coshyper function calculates the final answer, getNumTerms gets the n value from the user.
*/
#include <stdio.h>
#include <math.h>

//Prototypes
float coshyper(float, int);
int getNumTerms();

int main() {
    //Declaration of Variables
    int n;  //number of terms
    float x, ans; //x value to be computed and answer in respective order.
    char quit;

    //This section of code gets the x value from the user and calls the function that gets the n value from the user.
    //This is in a do while loop where if the user does not want to quit the program runs.
    do {
        printf(" Enter the value for x : ");  //Asks for x value
        scanf("%f", &x);

        fflush(stdin);  //flushing the input

        n = getNumTerms();  //calls the function that gets the n value

        ans = coshyper(x, n);  //passes through x and n values and calculates/returns the cosh(x) value.
        printf(" The value of Cos(%f) is : %.5E\n", x, ans);

        //Loop that asks the user if they want to quit or not. prevents any invalid inputs.
        do {
            printf("Do you want to quit (y/n): ");
            scanf("%c", &quit);
        } while (quit != 'y' && quit != 'n');

        if (quit == 'y') {
        printf("Program Terminated");  //tells the user that the program is terminated.
        return 0;
        }
    } while (quit == 'n'); //brings the program back to the beginning.

}

//This function takes in the x value and n value from the user and computes the cosh(x) using the series formula given in the assignment.
float coshyper(float x, int n) {
    float sum = 1; //The first value is always 1.
    float t = 1;  //the term number
    int i; //counter for loop

    for (i = 1; i <= n; i++) {
        t = t * x * x / (2 * i * (2 * i - 1));
        sum = sum + t;
    }

    return sum;  //returns the final answer.
}

//This function takes in the n value from the user.
//the value must be positive so there is a while loop that keeps the program asking until the user gives a positive integer.
int getNumTerms() {
    int num;
    do {
        printf("Enter how many terms: ");
        scanf("%d", &num);
    }
    while (num <= 0);

    return num;  //returns the number of terms used in the equation.
}
