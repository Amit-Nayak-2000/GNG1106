/*------------------------------------------------------------------
Names: Amit Nayak, Jake Martin
Date: Dec 9th, 2018
File: deliverable.c
Description: MCG - Pipe Friction Factor: GNG 1106 Final Project
---------------------------------------------------------------------*/

//LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <gng1106plplot.h>
#include <math.h>
#include <float.h>
#include <string.h>

//DEFINITIONS
#define TRUE 1           //c doesn't do boolean type so we pretend
#define FALSE 0
#define X 0
#define Y 1
#define NAMELENGTH 50    //max length of the filename
#define TOLERANCE 0.0000001 //accuracy of root finding
#define MAXSIZE 100

//data structure
typedef struct
{
    double density;
    double velocity;
    double roughness;
    double viscosity;
    double minD;
    double maxD;
    double diameter;
    double Re;

} FACTOR;

// function prototypes
double getPositiveValue();
void getValidFileName(char name[]);
void calcReynolds(FACTOR *frictPTR);
double calcFx(double FNum, FACTOR *frictPTR);
double rootFind(double leftpt, double rightpt, FACTOR *frictPTR);
void calcPoints(FACTOR *frictPTR, double points[][MAXSIZE]);
void plot(FACTOR *frictPTR, double points[][MAXSIZE]);
int writeFile(FACTOR *frictPTR, char name[]);
int readFile(FACTOR *frictPTR, char name[]);
void GetInput(char name[], FACTOR *frictPTR);

int main(){
    printf("Pipe Friction Factor v1.1 - By Amit Nayak & Jake Martin\n");

    //create structure for values, and its pointer
    FACTOR *frictPTR, friction;
    frictPTR = &friction;

    //create data array
    double points[2][MAXSIZE];

    //create filename array
    char name[NAMELENGTH];

    //get user input
    GetInput(name, frictPTR);

    //calculates the relationship
    calcPoints(frictPTR, points);

    //plot the relationship
    plot(frictPTR, points);
    printf("Friction factor at minimum diameter is %lf\n", points[Y][0]);
    printf("Friction factor at maximum diameter is %lf\n", points[Y][MAXSIZE-1]);

    //finished
    return 0;
}

double getPositiveValue(){
    double value;
    int flag;

    do {
        fflush(stdin); //clear keyboard buffer
        scanf("%lf", &value); //scan in a value

        //if less than 0
        if (value < 0){
            flag = TRUE; //loop around afterwards
            printf("Please enter a value greater than or equal to zero: \n");
        }
        else { //otherwise
            flag = FALSE; //all good
        }

    } while (flag);

    //value is confirmed positive, return
    return value;
}

void getValidFileName(char name[]){

    //get a new, valid name
    int flag;
    do {
        printf("Enter a valid filename: \n");
        fflush(stdin); //clear keyboard buffer
        scanf("%s", name); //scan in string
        flag = FALSE; //assume name is good

        //scan through name looking for illegal chars
        for (int i = 0; i < NAMELENGTH; i++){

            if(name[i] == '/' ||
               name[i] == '?' ||
               name[i] == '<' ||
               name[i] == '>' ||
               name[i] == '\\' ||
               name[i] == ':' ||
               name[i] == '*' ||
               name[i] == '|'){

                flag = TRUE; //name is not good
                printf("ERROR: Filename cannot contain illegal characters. \n");
            }

        }

    } while (flag); //repeat while flag is true

    //once the name is good, we append .txt to the filename
    strcat(name, ".txt");
    return;
}

void calcReynolds(FACTOR *frictPTR){

    //Reynolds number
    double Reynolds;

    //terms in Reynolds Equation
    double rho = frictPTR->density;
    double V = frictPTR->velocity;
    double D = frictPTR->diameter;
    double mu = frictPTR->viscosity;

    //Reynolds Equation
    Reynolds = (rho * V * D) / mu;

    frictPTR->Re = Reynolds;
}

double calcFx(double FNum, FACTOR *frictPTR){

    //F(x) of the Colebrook Equation
    double colebrook;

    //terms in Colebrook Equation
    double f = FNum;
    double epsilon = frictPTR->roughness;
    double D = frictPTR->diameter;
    double Re = frictPTR->Re;

    //Colebrook Equation
    colebrook = ( 1 / sqrt(f) ) +
            2.0*log10( (epsilon / (3.7 * D)) + (2.51 / (Re * sqrt(f))) );

    return colebrook;
}

double rootFind(double leftpt, double rightpt, FACTOR *frictPTR){

    double midpt, rtvalue, midvalue, root;

    do {
        midpt = (leftpt + rightpt) / 2;
        rtvalue = calcFx(rightpt, frictPTR);
        midvalue = calcFx(midpt, frictPTR);

        if (rtvalue * midvalue >= 0) {
            rightpt = midpt;
        }
        else {
            leftpt = midpt;
        }
    } while ((rightpt - leftpt) > TOLERANCE);
    root = (rightpt + leftpt) / 2;

    return root;
}

void calcPoints(FACTOR *frictPTR, double points[][MAXSIZE]){

    //determine increment value
    double inc = (frictPTR->maxD - frictPTR->minD) / (MAXSIZE);

    //start at minimum
    frictPTR->diameter = frictPTR->minD;

    //fill arrays with data
    for (int i = 0; i < MAXSIZE; i++){
        calcReynolds(frictPTR); //recalculate Re for each new diameter
        points[X][i] = frictPTR->diameter;
        points[Y][i] = rootFind(0.0000001, 0.9999, frictPTR);
        //increment
        frictPTR->diameter += inc;
    }

}

void plot(FACTOR *frictPTR, double points[][MAXSIZE]){

    //initialize PLplot
    plsdev("wingcc");
    plinit();
    plwidth(3);

    //establish cartesian plane
    plenv(points[X][0], points[X][MAXSIZE - 1], 0, (points[Y][0] * 2), 0, 0);
    plcol0(RED);
    pllab("Diameter of Pipe", "Fluid Friction Factor",
                "Diameter of Pipe vs Fluid Friction Factor");

    //plot data
    plcol0(BLUE);
    plline(MAXSIZE, points[X], points[Y]);

    //end plot
    plend();
}

int writeFile(FACTOR *frictPTR, char name[]){ //assumed valid name input
    //establish file pointer
    FILE *fp = fopen(name, "w");

    //if file can't open
    if (fp == NULL){
        printf("ERROR: File could not be written! \n");
        return 1; //error!
    }
    //file is now confirmed to be open. time to write!

    //write all the user data
    fprintf(fp, "%f,", frictPTR->minD);
    fprintf(fp, "%f,", frictPTR->maxD);
    fprintf(fp, "%f,", frictPTR->roughness);
    fprintf(fp, "%f,", frictPTR->velocity);
    fprintf(fp, "%f,", frictPTR->density);
    fprintf(fp, "%f,", frictPTR->viscosity);

    //done, close the file
    fclose(fp);
    return 0; //success!
}

int readFile(FACTOR *frictPTR, char name[]){ //assumed valid name input
    //establish file pointer
    FILE *fp = fopen(name, "r");

    //if file can't open
    if (fp == NULL){
        printf("ERROR: File could not be read! \n");
        return 1; //error!
    }

    //file is now confirmed to be open. time to read!
    fscanf(fp, "%lf,", &frictPTR->minD);
    fscanf(fp, "%lf,", &frictPTR->maxD);
    fscanf(fp, "%lf,", &frictPTR->roughness);
    fscanf(fp, "%lf,", &frictPTR->velocity);
    fscanf(fp, "%lf,", &frictPTR->density);
    fscanf(fp, "%lf,", &frictPTR->viscosity);

    //done, close the file
    fclose(fp);
    return 0; //success!
}

void GetInput(char name[], FACTOR *frictPTR){

    char choice;
    int flag;
    do {
        printf("Open file or enter data manually? (o/m) \n");
        fflush(stdin); //clear keyboard buffer
        scanf("%c", &choice); //scan in a letter

        if (choice == 'o'){ //OPEN FILE
            flag = FALSE; //valid choice

            do {
                getValidFileName(name); //fills name[] with valid name
            } while (readFile(frictPTR, name)); //loop while readfile() returns 1 (error)

        }
        else if (choice == 'm'){ //ENTER MANUALLY
            flag = FALSE; //valid choice

            //min and max diameters--------------------------------------------
            int invalidDiameter;
            do {
                invalidDiameter = FALSE;
                printf("Please enter the Minimum and Maximum diameters in m: \n");
                frictPTR->minD = getPositiveValue();
                frictPTR->maxD = getPositiveValue();

                if (frictPTR->minD >= frictPTR->maxD){
                    invalidDiameter = TRUE;
                    printf("Maximum diameter must be larger than minimum diameter. \n");
                }
            } while(invalidDiameter);
            //-----------------------------------------------------------------


            //roughness--------------------------------------------------------
            int invalidRoughness;
            do {
                invalidRoughness = FALSE;
                printf("Please enter roughness of the pipe in m: \n");
                frictPTR->roughness = getPositiveValue();

                if (frictPTR->roughness < 1e-7 || frictPTR->roughness > 3e-3){
                    invalidRoughness = TRUE;
                    printf("Roughness must be greater than 1x10^-7 and less than 3x10^-3. \n");
                }
            } while(invalidRoughness);
            //-----------------------------------------------------------------


            //flow rate--------------------------------------------------------
            printf("Please enter the flow rate in m/s: \n");
            frictPTR->velocity = getPositiveValue();
            //-----------------------------------------------------------------


            //fluid density----------------------------------------------------
            int invalidFluidDensity;
            do {
                invalidFluidDensity = FALSE;
                printf("Please enter the fluid density in kg/m^3: \n");
                frictPTR->density = getPositiveValue();

                if (frictPTR->density < 0.5 || frictPTR->density > 2000){
                    invalidFluidDensity = TRUE;
                    printf("Fluid density must be greater than 0.5 and less than 2000. \n");
                }
            } while(invalidFluidDensity);
            //-----------------------------------------------------------------


            //viscosity--------------------------------------------------------
            int invalidViscosity;
            do {
                invalidViscosity = FALSE;
                printf("Please enter the viscosity in N*s/m^2: \n");
                frictPTR->viscosity = getPositiveValue();

                if (frictPTR->viscosity < 1e-6 || frictPTR->viscosity > 300){
                    invalidViscosity = TRUE;
                    printf("Viscosity must be greater than 1x10^-6 and less than 300. \n");
                }
            } while(invalidViscosity);
            //-----------------------------------------------------------------


            //ask to save data-------------------------------------------------
            char choice2;
            int flag2;
            do {
                printf("Save entered data? (y/n) \n");
                fflush(stdin); //clear keyboard buffer
                scanf("%c", &choice2); //scan in a letter

                if (choice2 == 'y'){ //save data
                    flag2 = FALSE; //valid choice

                    getValidFileName(name); //get the filename for the file
                    writeFile(frictPTR, name);

                }
                else if (choice2 == 'n'){ //don't save data
                    flag2 = FALSE; //valid choice
                }
                else {
                    flag2 = TRUE; //invalid choice
                    printf("Invalid choice. Enter either 'y' or 'n'. \n");
                }
            } while(flag2);
            //-----------------------------------------------------------------
        }
        else { //INVALID LETTER
            flag = TRUE; //invalid choice
            printf("Invalid choice. Enter either 'o' or 'm'. \n");
        }
    } while(flag);

    /*at this point, the friction struct has been filled with valid data
    and the user has finished their input/saving. Calculations and plotting
    can begin.*/

    return;
}

