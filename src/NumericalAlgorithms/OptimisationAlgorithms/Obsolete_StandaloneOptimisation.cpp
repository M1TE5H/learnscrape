/*=============================================================================
  PROGRAM STRUCTURE
  =============================================================================

                 ||==============================================||
                 ||             WARNING: REQUIREMENT             ||
                 ||----------------------------------------------||
                 ||     g++ compiler options: -llapack -lblas    ||
                 ||          (and generally -larmadillo)         ||
                 ||==============================================||

  This program performs the Steepest Descents and Conjugate Gradients routines
  to solve the matrix-vector equation A.x = b where A is a n x n symmetric
  square matrix and b is randomly-defined n-dimensional column vector.

  The matrix A chosen here is the matrix given in MTMCC 12 and chosen values
  are such that they ensure its positive definite nature.

  *****************************************************************************
  INVESTIGATION RESULTS
  *****************************************************************************

  1. This program works sensibly for matrices of dimensionality 101 x 101
     for the tested values between 0.1--0.99.

  2. It has been tested a bit for larger valued matrices (e.g. 211 x 211) where
     Steepest Descent often fails, especially for large values of xxx.

  3. Conjugate gradients usually meets the tolerance criteria in much
     less steps than Steepest Descents; this is significant for larger matrices.

  ============================================================================*/



#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "armadillo"

using namespace std;
using namespace arma;


#if (ARMA_VERSION_MAJOR > 0 || (ARMA_VERSION_MINOR >= 9 && ARMA_VERSION_PATCH >= 50))
#define RAND randu
#else
#define RAND rand
#endif


// Macros
#define PI 3.14159265358979323844
#define MAXIMUM_NUMBER_OF_ITERATIONS 1000
#define STEEPEST_DESCENTS_TOLERANCE 1.0e-10
#define CONJUGATE_GRADIENTS_TOLERANCE 1.0e-10


// Prototype Functions
double RMS_Error(colvec, colvec);
void Steepest_Descents_Step (mat, colvec&, colvec, colvec&, colvec&, double&);
void Steepest_Descents_Routine (mat, colvec&, colvec);
void Conjugate_Gradients_Step(mat, colvec&, colvec, colvec&, colvec&, colvec&,
			      double&, double&);
void Conjugate_Gradients_Routine(mat, colvec&, colvec);

/*============================================================================*/
// MAIN PROGRAM
/*============================================================================*/
int main()
{
  int n,k,i,j;
  double xxx,el;
  mat A;
  colvec x,xx,b;

  /***************************************************************************/
  // Initialisation of Matrix A and column vector b
  /***************************************************************************/

  cout << "==========================================================" << endl;
  cout << "INPUTS" << endl;
  cout << "----------------------------------------------------------" << endl;


  // Input size of square matrix A and check for errors
  cout << "Enter size of matrix n (must be odd): ";
  cin >> n;
  
  if (n % 2 != 1 || n < 1) {
    cerr << "ERROR|Input: n must be odd and positive." << endl;
    return 1;
  }

  // Input xxx to initialise components of the matrix A and check for errors
  cout << "Enter value for xxx: ";
  cin >> xxx;

  if (xxx < 0.0 || xxx > 1.0) {
    cerr << "ERROR|Input: xxx must be in the interval (0,1)." << endl;
    return 2;
  }

  cout << endl;
    
  // Generate our matrix A as specified
  A = eye<mat>(n,n);
  el = xxx;
  for (i=1;i<(n+1)/2;i++) {
    for (j=0;j<n;j++) {
      k = (i+j) % n;
      A(j,k) = el;
      A(k,j) = el;
    }
    el *= xxx;
  }

  // Generate column vector b with random values
  b = RAND<colvec>(n);


  // Use Lapack to determine solution for later comparison
  xx = solve(A,b);

  /****************************************************************************/
  // Perform Steepest Descents routine
  /****************************************************************************/
  cout << "==========================================================" << endl;
  cout << "STEEPEST DESCENTS" << endl;
  cout << "----------------------------------------------------------" << endl;
  
  // Initialise x as random vector for first input 
  x = RAND<colvec>(n);

  // Perform the Steepest Descents routine
  Steepest_Descents_Routine(A,x,b);

  // Determine and print out RMS error
  cout << "RMS Error    : "
       << RMS_Error(x,xx)/sqrt(n) << endl;
  cout << endl;

  
  /****************************************************************************/
  // Perform Conjugate Gradients routine
  /****************************************************************************/
  cout << "==========================================================" << endl;
  cout << "CONJUGATE GRADIENTS" << endl;
  cout << "----------------------------------------------------------" << endl;

  // Reset x as random vector for first input
  x = RAND<colvec>(n);

  // Perform the Conjugate Gradients routine
  Conjugate_Gradients_Routine(A,x,b);

  // Determine and print out RMS error
  cout << "RMS Error    : "
       << RMS_Error(x,xx)/sqrt(n) << endl;

  cout << "==========================================================" << endl;

  return 0;
}






/*============================================================================*/
// FUNCTIONS AND ROUTINES
/*============================================================================*/

// Function to determine (unscaled) rms error between two vectors a and b
// Note: this needs to be scaled by sqrt(n) after it has been called to get RMS
double RMS_Error(colvec a, colvec b)
{
  double rms_error = norm(a-b,2);
  return rms_error;
}





/*****************************************************************************/
// Steepest Descents Subroutines
/*****************************************************************************/
/* Steepest_Descent_Step: perform a single steepest descent iterative step.
   
   Function Inputs: > A     : n x n symmetric square matrix
                    > x     : reference to "initial" n-dimensional colvec x
		    > b     : n-dimensional colvec
		    > p     : reference to search direction vector to be
                              updated; necessary for conjugate gradients
		    > g     : reference to gradient vector to be updated
		    > alpha : reference to double to be updated

   Function Outputs: N/A

   Comments: > Error check required for symmetric property of A
             > Error check required for inconsistent dimensionalities of
               A, x, b, p, g
*/
void Steepest_Descents_Step (mat A, colvec& x, colvec b, colvec& p,
			     colvec& g, double& alpha)
{
  // Update gradient vector and update search direction
  g = A*x - b;
  p = -1.0*g;
  
  // Determine and update alpha using the colvec g
  double alpha_numerator = dot(g,g);
  double alpha_denominator = dot(g,(A*g));
  alpha = alpha_numerator/alpha_denominator;

  // Update colvec x as defined by the Steepest Descents method 
  x = x + alpha*p;
}

/*----------------------------------------------------------------------------*/
/* Steepest_Descent_Routine: perform multiple Steepest Descent steps until
                             convergence
   
   Function Inputs: > A     : n x n symmetric square matrix
                    > x     : reference to "initial" n-dimensional colvec x
		    > b     : n-dimensional colvec
   
   Function Outputs: print outs number of iterations required to complete
   
   Comments: > Error check for failed convergence with maximum number of
               iterations
		    
*/
void Steepest_Descents_Routine (mat A, colvec& x, colvec b)
{
  int i;
  colvec p,g;
  double alpha, norm_g;

  for (i=1; i<MAXIMUM_NUMBER_OF_ITERATIONS; i++)
    {
      Steepest_Descents_Step(A, x, b, p, g, alpha);
      norm_g = norm(g,2);
      
      if (norm_g < STEEPEST_DESCENTS_TOLERANCE)
	{
	  cout << "SD Completion: "
	       << i << " iterations" << endl;
	  break;
	}

    }


  if (i==MAXIMUM_NUMBER_OF_ITERATIONS)
    {
      cerr << "ERROR|Steepest Descents: failure of convergence." << endl;
    }

}



/******************************************************************************/
// Conjugate Gradients Subroutines
/******************************************************************************/
/* Conjugate_Gradients_Step: perform a single Conjugate Gradeints iterative
                             step.
   
   Function Inputs: > A     : n x n symmetric square matrix
                    > x     : reference to "initial" n-dimensional colvec x
		    > b     : n-dimensional colvec
		    > p     : reference to search direction to be updated
		    > r     : residual vector at current step
		    > r_new : reference to new residual vector to be updated
		    > alpha : reference to double to be updated
		    > beta  : reference to double to be updated

   Function Outputs: N/A

   Comments: > Error check required for symmetric property of A
             > Error check required for inconsistent dimensionalities of
               A, x, b, p, r, r_new
*/
void Conjugate_Gradients_Step(mat A, colvec& x, colvec b, colvec& p,
			      colvec& r, colvec& r_new, double& alpha,
			      double& beta)
{
  // Determine and update alpha using the colvec g and p
  /* WARNING: this is the more general form of what is implemented in Steepest
              Descents.*/
  double alpha_numerator = dot(r,r);
  double alpha_denominator = dot(p,(A*p));
  alpha = alpha_numerator/alpha_denominator;


  // Update x
  x = x + alpha*p;
  
  // Update r_new
  r_new = r - alpha*A*p;


  // Determine and update beta using g and the previous step g_0
  double beta_numerator = dot(r_new,r_new);
  double beta_denominator = dot(r,r);
  beta = beta_numerator/beta_denominator;

  // Update search direction using colvec g and the current step p
  p = r_new + beta*p;

  r = r_new;

}


/*----------------------------------------------------------------------------*/

/* Conjugate_Gradients_Routine: perform multiple Conjugate Gradient steps until
                                convergence
   
   Function Inputs: > A     : n x n symmetric square matrix
                    > x     : reference to "initial" n-dimensional colvec x
		    > b     : n-dimensional colvec
   
   Function Outputs: print outs number of iterations required to complete
   
   Comments: > Error check for failed convergence with maximum number of
               iterations
		    
*/
void Conjugate_Gradients_Routine (mat A, colvec& x, colvec b)
{
  int i;
  double alpha, beta, norm_res, norm_diff;
  colvec x_0, p, r_old, r;


  // Perform an initial Steepest Descent type step
  r = b - A*x;
  p = r;
  r_old = r;
  
  for (i=2; i<MAXIMUM_NUMBER_OF_ITERATIONS; i++)
    {
      // Set x_0 to x for later comparison; but has to be set before the step
      x_0 = x;

      // Perform the Conjugate Gradients step, noting the updates that happen
      Conjugate_Gradients_Step(A, x, b, p, r_old, r, alpha, beta);
     
      // Define appropriate doubles and check if tolerance has been exceeded
      norm_res = norm(p,2);
      norm_diff = norm(x_0-x,2);

      
      if (norm_res < CONJUGATE_GRADIENTS_TOLERANCE &&
	  norm_diff < CONJUGATE_GRADIENTS_TOLERANCE)
	{
	  cout << "CG Completion: "
	       << i << " iterations" << endl;
	  break;
	}

      
    }

  if (i==MAXIMUM_NUMBER_OF_ITERATIONS)
    {
      cerr << "ERROR|Conjugate Gradients: failure of convergence." << endl;
    }


}
