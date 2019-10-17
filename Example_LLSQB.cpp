#include <iostream>
#include <TMath.h>   //TMath::Prob
#include <Eigen/Dense>
#include <vector>
#include <iomanip>
using namespace Eigen;

int main()
{

// Solve LinearLSQ problem. Namely find the vector x, that minimizes the 
// chi-squared associated with A x = b where A has NDATA rows 
// and MPARS columns

const int NDATA = 3;    // N
const int MPARS = 2;    // M

// Here as a simple example we have three data-points (xi,yi,dyi) 
// namely
// i=0 (0.1, 2.0, 0.1)
// i=1 (0.2, 2.5, 0.12)
// i=2 (0.3, 2.8, 0.145)
// and we want to fit two models, A, a straight line f(x) = a0 + a1 x
// and B, a parabola                                 f(x) = a0 + a1 x^2
// There are of course simpler ways to solve this simple problem, but 
// the methods here can be extended to any linear problem and much 
// larger data-sets and other models.

// First set up some std::vectors with the data 
// - overkill for this simple problem - but arguably better for more 
// complex problems
std::vector<double> xdata;
std::vector<double> ydata;
std::vector<double> dydata;
xdata.push_back(0.1);
xdata.push_back(0.2);
xdata.push_back(0.3);
ydata.push_back(2.0);
ydata.push_back(2.5);
ydata.push_back(2.8);
dydata.push_back(0.1);
dydata.push_back(0.12);
dydata.push_back(0.145);

// Set up the normalized data vector, b.
VectorXd b(NDATA);
for (int i=0; i<NDATA; i++){
    b(i) = ydata[i]/dydata[i];
    std::cout << " i " << i << " b(i) " << b(i) << std::endl;
}

// Set up the design matrix, A, of the problem
MatrixXd A(NDATA,MPARS);

for (int i=0; i<NDATA; i++){
    for (int j=0; j<MPARS; j++){
        if(j==0)A(i,j) = 1.0/dydata[i];
//       if(j==1)A(i,j) = xdata[i]/dydata[i];
        if(j==1)A(i,j) = xdata[i]*xdata[i]/dydata[i];
//        if(j==1)A(i,j) = 0.0;
    }
}

std::cout << "Here is the matrix A:" << std::endl << A << std::endl;

// Carry out the SVD decomposition of the matrix, A such that,
// A = U S V^T where A is NxM, and 
//             U is NxM, S=diag(w_{0}, w_{1} ...) is MxM, and V is MxM.
// U is column-orthogonal
// D is the diagonal matrix with the singular values
// V is orthogonal
// If the system is over-constrained as in least-squares fitting with at 
// least one degree of freedom, then N > M.
// For the LSQ fitting problem, N corresponds to the data points, and M 
// to the parameters, such that the number of degrees of freedom is N-M.

JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);

std::cout << "Its singular values are:" << std::endl 
          << svd.singularValues() << std::endl;

std::cout << "Its left singular vectors are the columns of the thin U matrix:" 
          << std::endl << svd.matrixU() << std::endl;

std::cout << "Its right singular vectors are the columns of the thin V matrix:" 
          << std::endl << svd.matrixV() << std::endl;

std::cout << "Now solve the LLSQ problem with rhs vector:" << std::endl << b << std::endl;
std::cout << "A least-squares solution of A*x = b is:" 
          << std::endl << svd.solve(b) << std::endl;

// You might want to check explicitly that the solve method does return the LSQ solution 
// as given by Equation 15.4.17 in Numerical Recipes, 3rd edition

VectorXd a(MPARS); //The solution vector
a = svd.solve(b);

std::cout << " a(0) = " << a(0) << std::endl;
std::cout << " a(1) = " << a(1) << std::endl;

// let's check the fitted function values at each data point
VectorXd fval(NDATA);
VectorXd chi(NDATA);
double chisq = 0.0;
for (int i=0; i<NDATA; i++){
//    fval(i) = a(0) + a(1)*xdata[i];
    fval(i) = a(0) + a(1)*xdata[i]*xdata[i];
    chi(i)  = (ydata[i]-fval[i])/dydata[i];
    std::cout << " i " << i << " x(i) " << xdata[i] << " yi " 
              << std::setw(4) << ydata[i] << " +- " 
              << std::setw(5) << dydata[i] << " fval " << fval(i) 
              << " (yi-fval)/dyi " << std::setw(10) << chi(i) << " chi^2 " 
              << std::setw(10) << chi(i)*chi(i) << std::endl;
    chisq += chi(i)*chi(i);
}
std::cout << "Total chi-squared = " << chisq << std::endl;
int ndof = NDATA-MPARS;
std::cout << "Problem has " << ndof << " degree(s) of freedom " 
          << std::endl;
std::cout << "Corresponding fit probability is " 
          << TMath::Prob(chisq,ndof) <<std::endl;

// Now calculate the full covariance matrix of the fitted parameters 
// using Equation 15.4.20 of Numerical Recipes, 3rd edition.
// Need V and the singular values.

MatrixXd V(MPARS,MPARS);      // V from the SVD decomposition
MatrixXd Cov(MPARS,MPARS);    // The calculated Covariance matrix
V=svd.matrixV();
VectorXd s(MPARS);            // Form a vector with the singular values
for (int i=0; i<MPARS; i++){
   s(i) = (svd.singularValues())[i];
}
for (int j=0; j<MPARS; j++){
    for (int k=0; k<MPARS; k++){
        Cov(j,k) = 0.0;
        for (int i=0; i<MPARS; i++){
            Cov(j,k) += V(j,i)*V(k,i)/(s(i)*s(i));
        }
    }
}

std::cout << "Covariance matrix = " << std::endl;
std::cout << Cov << std::endl;

std::cout << " a(0) = " << a(0) << " +- " << sqrt(Cov(0,0)) << std::endl;
std::cout << " a(1) = " << a(1) << " +- " << sqrt(Cov(1,1)) << std::endl;
std::cout << " 0-1 Correlation Coefficient " 
          << Cov(0,1)/sqrt(Cov(0,0)*Cov(1,1)) << std::endl; 

}
