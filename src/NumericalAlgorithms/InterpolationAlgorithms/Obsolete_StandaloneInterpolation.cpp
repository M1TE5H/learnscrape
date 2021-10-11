/*=========================================================*/
// PROGRAM STRUCTURE
/*=========================================================*/
/*

    Version 2.0 Updated Code Preface (MTMCC 13)

    ||==============================================||
    ||             WARNING: REQUIREMENT             ||
    ||----------------------------------------------||
    ||     g++ compiler options: -llapack -lblas    ||
    ||     if solve(A,x) routines are employed      ||
    ||==============================================||

    
    The following are either modifications or additions
    to the original code:

    1.   Added a generic routine for a solving a matrix-
         vector routine for a symmetric tridiagonal
 	 matrix of dimensions n x n. Details of the
	 implementation of this routine are given before
	 the function definition.

    2.   Implemented the tridiagonal solver into the
         main as an alternative way to determine the
	 second derivatives; and used these to pass
	 into the previous defined routine to evaluate
	 the approximation to sin(x) for an input x.

    3.   Routines to print out comparison between
         the original and the tridiagonal solver (TDS)
	 methods.



   Note that the routines are NOT the most efficient;
   it would have more sensible to use vectors instead
   of matrices with a larger number of zeros; but
   this is to ensure the code is easily to read and
   re-use. Additionally, the main reason to implement
   a tridiagonal solver is that it is faster (when
   working with the vectors---instead of the matrices
   as done here)


   --------------------------------------------------

   INVESTIGATION RESULTS: for N=10

   Generally there a difference of O(10^-16) between
   the second derivatives found from the two
   different methods and usually the corresponding
   differences in the approximated sin(x) values are
   not noticeable except at the certain values,
   e.g. consider input x = 3.14.

   Note: tests for other N values have not been
         undertaken.

   --------------------------------------------------

   Version 1.0. Original Code Preface (See: 4)

   This program performs a cubic spline interpolation
   for the sin(x) function given a set of known points
   lying within the interval [0, 2*pi]. This set of
   known points are spaced evenly in the interval.
   This number of known points can be modified by
   a macro, which will also allow a "pseudo-dynamic"
   evaluation of the interval size.

   An input value is taken and the cubic spline
   interpolation approximation value is printed.
   
   There is also a routine to produce data to be
   plotted, giving the cubic spline approximation
   for the sine function. The number of points
   for this routine can modified as intended.

*/


#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "armadillo"

using namespace std;
using namespace arma;


// Macros
#define PI 3.1415926535897932384626433
#define NUMBER_OF_POINTS 10


// Prototype Functions
int splinefit (double, double&, int, double, double[], double[]);
double sine_spline (double, double, double[], double[]);
void Plot_Spline_versus_Actual_Sine(int, double, double[],
				    double[], std::ofstream&);
void Tridiagonal_Solver (int, mat, colvec, double[]);



/*============================================================*/
// MAIN PROGRAM
/*============================================================*/
int main()
{
  double x_input, approx_sin_x_input;
  double spacing = 2.0*PI/(NUMBER_OF_POINTS-1);
  ofstream PLOTFILE ("SplineSinePlot.txt");

  double y[NUMBER_OF_POINTS], ypp[NUMBER_OF_POINTS],
         ypp_1[NUMBER_OF_POINTS-2];


  
  // Fill array of y with values of sine at the "known" points
  for (int i=0; i<NUMBER_OF_POINTS; i++)
    {
      y[i] = sin(i*spacing);
    }


  // Construct matrix A and vector b for our problem
  
  /* We must be very careful with out our indices and
     what A and b actually represent. The triagonal
     solver is used for the matrix-vector equation
     is a "sub-problem" of our actual problem in
     which we are solving for {ypp_2,....,ypp_(N-1)}
     as opposed to whole vector containing the second
     derivatives {ypp} = {ypp_1,...,ypp_N}. So the
     vector b that is passed into the tridiagonal solver
     is (N-2) dimensional, whose first element
     corresponds the second element of {ypp}, namely
     ypp_2 ---- and whose final element corresponds to
     the penultimate element of {ypp}, namely ypp_(N-1).

     So therefore we will define the vector b that is
     to be passed into the tridiagonal solver as the
     vector in which element i corresponds to element
     (i+1) of the y vector.

     Later we will define a new N-dimensional array w,
     whose first and last elements are set to zero in
     consistency with the problem, and the middle elements
     of w are given by double derivatives solved by the
     tridiagonal solver, namely the (N-2)-dimensional
     array ypp_1.
     
   */


  mat A = zeros<mat>(NUMBER_OF_POINTS-2, NUMBER_OF_POINTS-2);
  colvec b = zeros<colvec>(NUMBER_OF_POINTS-2);

  // Avoid repeat calculations; define scaling factor
  double SF = 6.0/(spacing*spacing);
  
  A.at(0,0) = 4.0;
  A.at(1,0) = 1.0;
  A.at(0,1) = 1.0;
  b.at(0)   = SF*(y[2] - 2.0*y[1] + y[0]);

  for (int i=1; i<NUMBER_OF_POINTS-2; i++)
    {
      A.at(i,i)   = 4.0;
      A.at(i,i-1) = 1.0;
      A.at(i-1,i) = 1.0;

      
      b.at(i) = SF*(y[i+2] - 2.0*y[i+1] + y[i]);
    }

  
  // Employ triagonal solver; update ypp_1
  Tridiagonal_Solver(NUMBER_OF_POINTS-2, A, b, ypp_1); 


  // Print out ypp_1
  /*
  cout << endl;
  for (int i=0; i<NUMBER_OF_POINTS-2; i++)
    {
      cout << ypp_1[i] << endl;
    }
  cout << endl;
  */


  // Proceed with the routines from the original program
  cout << "Insert an input value x in the interval [0, 2*pi]: ";
  cin >> x_input;


  // Check for input errors
  if (x_input < 0.0 || x_input > 2.0*PI)
    {
      cerr << "ERROR|Input: x is not in the interval [0, 2*pi]." << endl;
      return 1;
    }


  // Run original spline-fit routine  
  int run = splinefit(x_input, approx_sin_x_input, NUMBER_OF_POINTS,
		      spacing, y, ypp);

  


  /* Define array w which will contain the second derivatives of
     y as determined by the tridiagonal solver */
  double w[NUMBER_OF_POINTS];

  // Set the first and last elements of w to zero.
  w[0] = 0.0;
  w[NUMBER_OF_POINTS-1]=0.0;

  // Set the middle (N-2) elements of w to that ypp_1
  for (int i=1; i<NUMBER_OF_POINTS-1; i++)
    {
      w[i] = ypp_1[i-1];
    }


  // Print out ypp and w for comparision and their difference
  cout << "============================================" << endl;
  cout << "ypp(ORIGINAL)" << '\t' << "ypp(TDS)" << '\t'
       << "Difference" << endl;
  cout << "--------------------------------------------" << endl;
  for (int i=0; i<NUMBER_OF_POINTS; i++)
    {
      cout << ypp[i] << '\t' << w[i] << '\t'
	   << fabs(ypp[i] - w[i])  << endl;
    }
  cout << "============================================" << endl;

  // Evaluate approximation using TDS "ypp" values w
  double approx_sin_x_input_TDS  = sine_spline(x_input, spacing, y, w);


  // Print out results
  cout << setprecision(9);
  cout << "Cubic Spline approximated sin(x) = "
       << approx_sin_x_input << endl;
  cout << "Cubic Spline approximated sin(x) with TDS:"
       << approx_sin_x_input_TDS << endl;
  cout << "Difference with and without TDS: "
       << fabs(approx_sin_x_input_TDS - approx_sin_x_input) << endl;
  cout << "Actual value sin(x) = " << sin(x_input) << endl;

  //Plot_Spline_versus_Actual_Sine(100, spacing, y, ypp, PLOTFILE);
  

  return 0;
}



/*=========================================================*/
// FUNCTIONS
/*=========================================================*/


/* Subroutine for solving the linear system of equations
   for cubic spline fitting. Note that this spectral
   method is not optimal for speed. This routine accepts
   the y-values at n points with uniform spacing h and
   returns the second derivative values in ypp

   NOTE: in comparison to original form of the routine,
   this routine has been update to include two additional
   inputs: a value for the input x and an a reference to
   the value of the approximated value of sine as
   determined by cubic spline. In order to so, this
   function calls the spline routine function to update
   the reference.
   
   WARNING: For the initial method, this strategy seems
   convenient; however for a series of evaluations, this
   method is hindered by repeated calculations; though
   they do not play a significant role at this stage.
*/


int splinefit (double x_input, double& approx_sin_x_input,
	       int n, double h, double y[], double ypp[])
{
  int i,k;
  double q,fac;
  double *b,*dotp;

  // Calculate the RHS of the set of linear equations
  b = new double [n-1];
  fac=6.0/(h*h);
  for (i=1;i<n-1;i++) {
    b[i]=fac*(y[i+1]-2.0*y[i]+y[i-1]);
  }

  // Calculate the inner products between b and the eigenvectors
  dotp = new double [n-1];
  fac=sqrt(2.0/(n-1));
  q=PI/(n-1);
  for (k=1;k<n-1;k++) {
    dotp[k]=0.0;
    for (i=1;i<n-1;i++) {
      dotp[k]+=b[i]*sin(q*i*k);
    }
    dotp[k]*=fac;
  }

  // Divide the dot products by the eigenvalues
  for (k=1;k<n-1;k++) {
    dotp[k]/=(4.0+2.0*cos(q*k));
  }

  // Construct the vector y''
  ypp[0]=0.0;
  for (i=1;i<n-1;i++) {
    ypp[i]=0.0;
    for (k=1;k<n-1;k++) {
      ypp[i]+=dotp[k]*sin(q*i*k);
    }
    ypp[i]*=fac;
  }
  ypp[n-1]=0.0;

  // Determine and update the value of approx sin(x)
  approx_sin_x_input = sine_spline(x_input, h, y, ypp);

  // Release memory
  delete[] dotp;
  delete[] b;

  return 0;
}


/* Cubic spline formula: evaluates the value of the spline
   at a given input value with the knowledge of the function
   and its derivatives at the pre-determined points.
   
*/

double sine_spline (double x_input, double spacing, double y[], double ypp[])
{
  // Determine interval of interest
  double interval_position = floor(x_input/spacing);
  int interval = int(interval_position);

  double x_lower = interval*spacing;
  double x_upper = x_lower + spacing;

  // cout << interval << " and " << x_lower << endl;

  // Avoid repeating calculations
  double x_lower_difference = x_input - x_lower;
  double x_upper_difference = x_upper - x_input;

  double cubic_spline_sine;
  double inverse_spacing = 1.0/spacing; // Convenient to define

  cubic_spline_sine = inverse_spacing*(x_upper_difference*y[interval]
				       + x_lower_difference*y[interval+1]
				       -(1.0/6.0)*(x_lower_difference)*(x_upper_difference)
				       *((x_upper_difference + spacing)*ypp[interval] +
					 (x_lower_difference + spacing)*ypp[interval+1]));

  return cubic_spline_sine;

  
    
}


/* Write out to a file data for a plot of the approximated
   sine values from the cubic spline as well as an error
   (as defined in any convenient fashion written into the
   for loop below) with respect to actual values of sin(x)
   at the same point.

   NOTE: this produces (Number_of_Iterations + 1) data points
   including the point x=0.0.
*/


void Plot_Spline_versus_Actual_Sine (int Number_of_Iterations,
				     double spacing, double y[],
				     double ypp[], std::ofstream& FILE)
{
  double x_initial = 0.0, x_input;
  double iteration_step_size = 2.0*PI/Number_of_Iterations;
  double approx_sin_x_input, error;
  int run;

  /* cout << "x" << '\t' << "CubicSpline[sin(x)]"
          << '\t' << "Error" << endl;*/ 

  for (int i=0; i<=Number_of_Iterations; i++)
    {
      x_input = x_initial + i*iteration_step_size;
      run = splinefit(x_input, approx_sin_x_input,
		      NUMBER_OF_POINTS, spacing, y, ypp);

      error = approx_sin_x_input - sin(x_input);

      FILE << x_input << '\t'
	   << approx_sin_x_input << '\t' << error << endl;

    }
}


/*==========================================================*/
// SYMMETRIC TRIDIAGONAL MATRIX EQUATION SOLVER ROUTINE
/*==========================================================*/
/* Routine description:

   Consider an n x n symmetric tridiagonal matrix [A] given
   by:

             [ A11 A12   0    0   ...  ]
             | A21 A22  A23   0   ...  |
       [A] = |  0  A32  A33  A34  ...  |
             |  0   0   A43  ...  ...  |
             [ ... ...  ...  ...  ...  ]

   And we wish to determine the solution vector x,
   given by the matrix-vector equation [A].x = b,
   where b is a known vector.

   For this tridiagonal matrix system, we may perform
   the matrix factorisation [A] = [L][D][L]^T where:

       [D] = diag(d_1, d_2, ..., d_n)

             [  1   0    0    0   ...  ]
             | e_1  1    0    0   ...  |
       [L] = |  0  e_2   1    0   ..   |
             |  0   0   e_3  ...  ...  |
             [ ... ...  ...  ...  ...  ]

   where we have:

    --  [D]_11      =  d_1     = A11
      
    --  [L]_{k,k-1} =  e_{k-1} = A_{k,k-1}/d_{k-1}

    --  d_k         =  A_{kk} - [e_{k-1} x A_{k,k-1}] 

   Then we may determine x by respectively solving
   the following equations:

   1.  [L].y     = b  for y where y = ([D][L]^T).x

       This is done simplying by exploiting the
       form of [L] to give:
       
            y_1 = b_1

            y_i = b_i - L_{i,i-1} y_{i-1}
       
       for i=2,3,...,n.

   
   2.  [D].z     = y  for z where z = ([L]^T).x

       This is done by noting that [D] is diagonal
       and so:

            z_i = y_i/(D_{i,i})


   3.  ([L]^T).x = z  for x

       This can be done by using backwards substitution.


 */

void Tridiagonal_Solver (int n, mat A, colvec b, double x[])
{
  int k;

  // A.print();
  // cout << endl;
  //  b.print();
  //  cout << endl;

   // Initialise D and L as n x n zero matrices
   mat D = zeros<mat>(n,n);
   mat L = zeros<mat>(n,n);
   
   // Set element D11 = A11
   D.at(0,0) = A.at(0,0);
   L.at(0,0) = 1.0;

   // Construct D and L using above relations
   for (k=1; k<n; k++)
     {
       L.at(k,k)   = 1.0;
       L.at(k,k-1) = A.at(k,k-1)/(D.at(k-1,k-1));
       D.at(k,k)   = A.at(k,k) - (L.at(k,k-1))*(A.at(k,k-1));
     }

   // Print matrices
   // D.print();
   // cout << endl;
   // L.print();
   // cout << endl;


   // set up column vectors initialised to zeros  
   colvec y = zeros<colvec>(n);
   colvec z = zeros<colvec>(n);
   colvec xx = zeros<colvec>(n);


   // Solve for y using forwards substitution
   y.at(0) = b.at(0);
   
   for (int i=1; i<n; i++)
     {
       y.at(i) = b.at(i) - (L.at(i,i-1))*y.at(i-1); 
     }
   //y.print();
   //cout << endl;

   // Solve for z
   for (int i=0; i<n; i++)
     {
       z.at(i) = (y.at(i))/(D.at(i,i));
     }
   //z.print();
   //cout << endl;


   // Solve for xx (=x) using backwards substitution
   L = L.t(); //transpose L
   xx.at(n-1) = z.at(n-1);
   
   for (int i=n-2; i>=0; i--)
     {
       xx.at(i) = z.at(i) - (L.at(i,i+1))*(xx.at(i+1));
     }

   // xx.print();

   // set input array x to have values of xx
   for (int i=0;i<n;i++)
    {
      x[i] = xx.at(i);
    }

}
