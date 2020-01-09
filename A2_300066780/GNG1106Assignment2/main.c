/*
Amit Nayak 300066780 GNG1106 Assignment 2 Question 2
The purpose of this program is to calculate the weight
of a number of washers given the # of washers, density, thickness,
external diameter, and internal diameter. This program can be for calculations
for different metals as long as their metal density is given.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Prototype
double totalWeight(int, double, double, double, double);

/*
The main function prompts the user to enter in the data for calculation
and calls upon the totalWeight function to calculate the weight of the washers.
After calculating the weight the program prints the original data the user inputed
and prints the final calculation of the weight.
*/
int main()
{
    //Declaration of variables
    int num;
    double den, thick, ex_dia, in_dia, weight;

    //This section of code prompts the user to enter in the data of the washers.
    //Then it records the data in the variables defined earlier.
    printf("Please enter the number of washers: \n");
    scanf("%d", &num);
    printf("Please enter the density of the washer's material: \n");
    scanf("%lf", &den);
    printf("Please enter the the thickness of the washer: \n");
    scanf("%lf", &thick);
    printf("Please enter the external diameter: \n");
    scanf("%lf", &ex_dia);
    printf("Please enter the diameter of the hole: \n");
    scanf("%lf", &in_dia);

    //The final weight is calculated by passing in the variables-
    //into the totalWeight function then weight equal to the final answer.
    weight = totalWeight(num, den, thick, ex_dia, in_dia);

    system("cls"); //clears the screen to display all the data in a clear way.

    //Displays all user inputed data plus the final answer.
    printf("The washer characteristics are: \n");
    printf("Density: %.5f Kg/cm^3\n", den);
    printf("Thickness: %.3f cm\n", thick);
    printf("External diameter: %.3f cm\n", ex_dia);
    printf("Diameter of the hole: %.3f cm\n\n", in_dia);
    printf("The weight of %d washers is %0.2e Kg", num, weight);

    return 0;
}
/*
    This function passes in the user inputed data to calculate the final weight:
    num corresponds to the number of washers
    den corresponds to the density of the metal
    thick corresponds to the thickness of the washer
    ex_dia corresponds to the external diameter
    in_dia corresponds to the internal diameter
    w is the final weight calculation that is returned to the "weight" variable in the main function.

*/
double totalWeight(int num, double den, double thick, double ex_dia, double in_dia)
{
    double w; //variable for weight calculation.
    //Sequence of cumulative steps to calculate the final weight broken down from the given equation.
    w = pow(ex_dia, 2) - pow(in_dia, 2);
    w = num * den * thick * M_PI * w;
    w = w / 4;
    return w;
}






