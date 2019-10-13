#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;

int main()
{

// Instantiate a matrix filled with random numbers
const int NROWS = 3;    // N
const int MCOLS = 2;    // M
MatrixXd A = MatrixXd::Random(NROWS,MCOLS);
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

// Hmm really. With all matrix decompositions especially when starting 
// out we should test that this is indeed true.
// Let's set up the diagonal matrix, S with the singular values
MatrixXd S(MCOLS,MCOLS);
S << (svd.singularValues())[0], 0.0, 0.0, (svd.singularValues())[1];
std::cout << S << std::endl;

MatrixXd B(NROWS,MCOLS);    // N x M
MatrixXd V(MCOLS,MCOLS);    // M x M
MatrixXd VT(MCOLS,MCOLS);   // Transpose of V
MatrixXd U(NROWS,MCOLS);    // N x M

V=svd.matrixV();
U=svd.matrixU();
VT=(svd.matrixV()).transpose();

B = U*S*VT; // Recompose the matrix from the decomposition

std::cout << "B " << std::endl;
std::cout << B << std::endl;

MatrixXd C(NROWS,MCOLS);
C = A - B;
std::cout << "Difference matrix " << std::endl;
std::cout << C << std::endl;
std::cout << C.norm() << std::endl;

MatrixXd D(MCOLS,MCOLS);
//D = ( (svd.matrixV()).transpose() ) * svd.matrixV();
//D = V*VT;
D = ((svd.matrixU()).transpose())*U;
std::cout << "D  " << std::endl;
std::cout << D << std::endl;
std::cout << D.norm() << std::endl;

Vector3d rhs(1.0, 0.0, 0.0);
std::cout << "Now consider this rhs vector:" << std::endl << rhs << std::endl;
std::cout << "A least-squares solution of A*x = rhs is:" 
          << std::endl << svd.solve(rhs) << std::endl;

}
