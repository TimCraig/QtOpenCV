/*****************************************************************************
********************************* DMatrix.h **********************************
*****************************************************************************/

#if !defined(__DMATRIX_H__)
#define __DMATRIX_H__

/*
   Templated classes providing Vector, 2D Array, and Matrix classes.  The 2D
   Array classes are separated from the Matrix classes so different 2D Array
   storage schemes can be used as the basis of the Matrix classes.
   
   None of the "standard" libraries I found satisfied my needs for something
   understandable and documented well enough to be usable.  One of the driving
   factors in the design of these classes was to make the element access
   methods as "natural" as possible, ie using the double subscript operation
   to access individual elements of a 2D Array and hence matrix.  Other
   operator "eye candy" is provided with the caveats that some of those
   operations are inefficient compared to their functional counterparts.
*/

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "DVector.h"

/*****************************************************************************
******************************* class DArray2D *******************************
*****************************************************************************/

/* Dense 2D Array.  The elements are all stored in a single vector and another
   vector of row pointers is provided for efficiency.  This efficiency comes
   at the cost of additional storage for the pointers, some initial
   computation of the pointer values, maintenance of the pointers should the
   arrangment change, and a bit of structural complexity.  However, access
   efficiency is increased since the row offset need not be computed every
   time an element is accessed.
*/

#define DArray2DTemplate template <typename T>

DArray2DTemplate
class DArray2D
   {
   public :
      DArray2D()
         {
         m_bRowsSwapped = false;
         return;
         }

      DArray2D(size_t nRows, size_t nCols) : m_nRows(nRows), m_nCols(nCols),
            m_E(nRows * nCols), m_R(nRows)
         {
         InitRowPtrs();
         return;
         }
      
      DArray2D(const DArray2D& src)
         {
         Copy(src);
         return;
         }
      
      ~DArray2D()
         {
         return;
         }

      DArray2D<T>& operator=(const DArray2D<T>& rhs)
         {
         Copy(rhs);
         return (*this);
         }
      
      size_t NumRows() const
         {
         return (m_nRows);
         }
  
      size_t NumCols() const
         {
         return (m_nCols);
         }

      T* operator[](size_t nRow)
         {
         return (m_R[nRow]);
         }
      
      const T* operator[](size_t nRow) const
         {
         return (m_R[nRow]);
         }

      // Initialize with a native array trusting it's large enough
      void Initialize(const T Init[])
         {
         for (size_t i = 0 ; i < NumRows() * NumCols() ; i++)
            {
            m_E[i] = Init[i];
            } // end for
         return;
         }
   
      void Resize(size_t nRows, size_t nCols);      

      // Exchange two rows
      void SwapRows(size_t r1, size_t r2)
         {
         // Just exchange the row pointers
         std::swap(m_R[r1], m_R[r2]);
         m_bRowsSwapped = true;
         return;
         }
       
   protected :
      // Array size information
      size_t m_nRows;
      size_t m_nCols;
      
      // Storage vector for the individual elements
      DVector<T> m_E;
      
      // Vector of pointers to each row of the matrix
      std::vector<T*> m_R;
      
      // A flag indicating of any of the rows of the array have been swapped
      // by exchanging row pointers such that the element vector cannot be
      // considered continuous
      bool m_bRowsSwapped;
      
      // Initialize the row pointers
      void InitRowPtrs()
         {
         for (size_t r = 0 ; r < NumRows() ; r++)
            {
            m_R[r] = &m_E[r * NumCols()];
            } // end for
         m_bRowsSwapped = false;
         return;      
         }

      // Make a copy
      void Copy(const DArray2D<T>& src);
     
   private :
   };  // End of class DArray2D

/*****************************************************************************
*********************** Class DArray2D Implementation ************************
*****************************************************************************/

/*****************************************************************************
*
*  DArray2D::Resize
*
*  Resize the array to conform to a new number of rows and/or columns.  Does
*  NOT preserve element values after the resize.  Safe to call with the same
*  sizes.
*
*****************************************************************************/

DArray2DTemplate
void DArray2D<T>::Resize(size_t nRows, size_t nCols)
   {
   if ((nRows != NumRows()) || (nCols != NumCols()))
      {
      m_E.resize(nRows * nCols);
      m_nCols = nCols;
      if (nRows != NumRows())
         {
         m_R.resize(nRows);
         m_nRows = nRows;
         } // end if
         
      InitRowPtrs();   
      } // end if
   
   return;

   } // End of function DArray2D::Resize 

/*****************************************************************************
*
*  DArray2D::Copy
*
*  Make a copy of an array.  Since the row pointers in the source may have
*  been manipulated by row exchange operations, we can't simply use the 
*  vector::operator=() to copy the elements.
*
*****************************************************************************/

DArray2DTemplate
void DArray2D<T>::Copy(const DArray2D<T>& src)
   {
   m_bRowsSwapped = src.m_bRowsSwapped;
   
   // Allocate storage and initialize row pointers
   Resize(src.NumRows(), src.NumCols());
   
   // Now copy the elements
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      // Optimize access to the rows
      T* R = (*this)[r];
      const T* RS = src[r];
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] = RS[c];
         } // end for
      } // end for
        
   return;

   } // End of function DArray2D::Copy 

/*****************************************************************************
**************************** class DFixedArray2D *****************************
*****************************************************************************/

/*
   A 2D Array where the storage is based on a fixed size native 2D Array.
   Because of the storage scheme, some operations, particularly resizing,
   cannot be performed.
*/

#define DFixedArray2DTemplate template <typename T, size_t ROWS, size_t COLS>

DFixedArray2DTemplate
class DFixedArray2D
   {
   public :
      DFixedArray2D()
         {
         return;
         }

      // Dummy to allow this as template parameter for DMatrix
      DFixedArray2D(size_t nRows, size_t nCols)
         {
         assert((nRows == ROWS)  && (nCols == COLS));
         // Throw an exception or just do nothing?
         return;
         }

      DFixedArray2D(const DFixedArray2D<T, ROWS, COLS>& src)
         {
         Copy(src);
         return;
         }
       
      ~DFixedArray2D()
         {
         return;
         }

      DFixedArray2D& operator=(const DFixedArray2D<T, ROWS, COLS>& rhs)
         {
         Copy(rhs);
         return (*this);
         }

      size_t NumRows() const
         {
         return (ROWS);
         }
  
      size_t NumCols() const
         {
         return (COLS);
         }
 
       // Initialize with a native array trusting it's large enough
      void Initialize(const T Init[])
         {
         for (size_t i = 0, r = 0 ; r < NumRows() ; r++)
            {
            for (size_t c = 0 ; c < NumCols() ; c++)
               {
               m_E[r][c] = Init[i++];
               } // end for
            } // end for
         return;
         }
 
      T* operator[](size_t nRow)
         {
         assert(nRow < ROWS);
         return (m_E[nRow]);
         }
      
      const T* operator[](size_t nRow) const
         {
         assert(nRow < ROWS);
         return (m_E[nRow]);
         }
       
      void Resize(size_t nRows, size_t nCols)
         {
         if ((nRows != ROWS) || (nCols != COLS))
            {
            // throw an exception
            } // end if
         return;
         }
 
      // Exchange two rows 
      void SwapRows(size_t r1, size_t r2)
         {
         for (size_t c = 0 ; c < COLS ; c++)
            {
            // Exchange each column in the two rows
            std::swap(m_E[r1][c], m_E[r2][c]);
            } // end for
         return;
         }
      
   protected :
      T m_E[ROWS][COLS];
      
      // Make a copy
      void Copy(const DFixedArray2D<T, ROWS, COLS>& src);
      
   private :
   };  // End of class DFixedArray2D

/*****************************************************************************
********************* Class DFixedArray2D Implementation *********************
*****************************************************************************/

/*****************************************************************************
*
*  DFixedArray2D::Copy
*
*  Make a copy of an array.  Since the row pointers in the source may have
*  been manipulated by row exchange operations, we can't simply use the 
*  vector::operator=() to copy the elements.
*
*****************************************************************************/

DFixedArray2DTemplate
void DFixedArray2D<T, ROWS, COLS>::Copy(
      const DFixedArray2D<T, ROWS, COLS>& src)
   {
   // Can't be resized but Resize will check and handle the error
   Resize(src.NumRows(), src.NumCols());
   
   // Now copy the elements
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      // Optimize access to the rows
      T* R = (*this)[r];
      const T* RS = src[r];
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] = RS[c];
         } // end for
      } // end for
        
   return;

   } // End of function DFixedArray2D::Copy 

/*****************************************************************************
******************************* class DMatrix ********************************
*****************************************************************************/

/*
   A simple 2D Matrix implementation.  The matrix is dense and uses a single
   allocated array to store the data.  The underlying 2D Array type used to 
   store the elements is specified by the template parameter ARRAY.
   (I suppose another option is to use ARRAY as a base class.)
*/

// Make it easy to change the template parameters
#define DMatrixTemplate template <typename T, typename ARRAY>

DMatrixTemplate
class DMatrix
   {
   public :
      DMatrix(T ZeroTest = 100 * std::numeric_limits<T>::epsilon())
         {
         m_ZeroTest = ZeroTest;
         return;
         }                
      
      DMatrix(size_t nRows, size_t nCols,
            T ZeroTest = 100 * std::numeric_limits<T>::epsilon())
            : m_A(nRows , nCols)
         {
         m_ZeroTest = ZeroTest;
         return;
         }

      DMatrix(const DMatrix<T, ARRAY>& src)
            : m_A(src.NumRows(), src.NumCols())
         {
         Copy(src);
         return;
         }
      
      ~DMatrix()
         {
         return;
         }

      DMatrix<T, ARRAY>& operator=(const DMatrix<T, ARRAY>& rhs)
         {
         Copy(rhs);
         return (*this);
         }
      
      bool operator==(const DMatrix<T, ARRAY>& rhs);

      size_t NumRows() const
         {
         return (m_A.NumRows());
         }
  
      size_t NumCols() const
         {
         return (m_A.NumCols());
         }

      void Resize(size_t nRows, size_t nCols)
         {
         m_A.Resize(nRows, nCols);
         return;
         }
       
      // Initialize with a native array trusting it's large enough
      void Initialize(const T Init[])
         {
         m_A.Initialize(Init);
         return;
         }
            
      T& GetZeroTest()
         {
         return (m_ZeroTest);
         }
      
      const T& GetZeroTest() const
         {
         return (m_ZeroTest);
         }
      
      void SetZeroTest(const T& ZeroTest)
         {
         m_ZeroTest = ZeroTest;
         return;
         }
      
      T* operator[](size_t nRow)
         {
         return (m_A.operator[](nRow));
         }
      
      const T* operator[](size_t nRow) const
         {
         return (m_A.operator[](nRow));
         }
       
      // Make this matrix the same size as the source
      void MakeSameSize(const DMatrix<T, ARRAY>& src)
         {
         Resize(src.NumRows(), src.NumCols());
         return;
         }
      
      bool IsSquare() const
         {
         return (NumRows() == NumCols());
         }
      
      bool IsSameSize(const DMatrix<T, ARRAY>& a) const
         {
         return (IsSameSize(*this, a));
         }
      
      static bool IsSameSize(const DMatrix<T, ARRAY>& a,
            const DMatrix<T, ARRAY>& b)
         {
         return ((a.NumRows() == b.NumRows())&& (a.NumCols() == b.NumCols()));
         }
      
      void Set(T v = 0)
         {
         for (size_t i = 0 ; i < m_A.size() ; i++)
            {
            m_A[i] = v;
            } // end for
         return;
         }
      
      void Zero()
         {
         Set(0);
         return;
         }
      
      // Make this matrix a diagonal matrix with d on the diagonal
      bool Diagonal(const T& d);
      
      // Make this matrix the Identity matrix
      bool Identity()
         {
         return (Diagonal(1));
         }
      
      void Add(T s);
      static void Add(DMatrix<T, ARRAY>& Result,
            const DMatrix<T, ARRAY>& A, T s);
      void Add(const DMatrix<T, ARRAY>& A, T s)
         {
         Add(*this, A, s);
         return;
         }
            
      bool Add(const DMatrix<T, ARRAY>& A);
      static bool Add(DMatrix<T, ARRAY>& Result, const DMatrix<T, ARRAY>& A,
            const DMatrix<T, ARRAY>& B);
      bool Add(const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
         {
         return (Add(*this, A, B));
         }
      
      void Sub(T s);
      static void Sub(DMatrix<T, ARRAY>& Result,
            const DMatrix<T, ARRAY>& A, T s);
      void Sub(const DMatrix<T, ARRAY>& A, T s)
         {
         Sub(*this, A, s);
         return;
         }
 
      bool Sub(const DMatrix<T, ARRAY>& A);
      static bool Sub(DMatrix<T, ARRAY>& Result, const DMatrix<T, ARRAY>& A,
            const DMatrix<T, ARRAY>& B);
      bool Sub(const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
         {
         return (Sub(*this, A, B));
         }

      // Multiply by a scalar         
      void Mul(T s);
      static void Mul(DMatrix<T, ARRAY>& Result,
            const DMatrix<T, ARRAY>& A, T s);
      void Mul(const DMatrix<T, ARRAY>& A, T s)
         {
         Mul(*this, A, s);
         return;
         }
 
      // Matrix multiplication
      static bool Mul(DMatrix<T, ARRAY>& Result, const DMatrix<T, ARRAY>& A,
            const DMatrix<T, ARRAY>& B);
      bool Mul(const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
         {
         return (Mul(*this, A, B));
         }
       
      // Transpose multiplication
      bool MulAxBTranspose(const DMatrix<T, ARRAY>& A,
            const DMatrix<T, ARRAY>& B);
      bool MulATransposexB(const DMatrix<T, ARRAY>& A,
            const DMatrix<T, ARRAY>& B);
      
      // Arithmetic operators
      // Possibly introduces temporaries, copy ops, & exceptions)
      
      DMatrix<T, ARRAY> operator+(DMatrix<T, ARRAY>& rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), NumCols());
         if (!DMatrix<T, ARRAY>::Add(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }

      void operator+=(const DMatrix<T, ARRAY>& rhs)
         {
         Add(*this, rhs);
         return;
         }
      
      DMatrix<T, ARRAY> operator+(T rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), NumCols());
         if (!DMatrix<T, ARRAY>::Add(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }

      void operator+=(T s)
         {
         Add(s);
         return;
         }
      
      DMatrix<T, ARRAY> operator-(DMatrix<T, ARRAY>& rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), NumCols());
         if (!DMatrix<T, ARRAY>::Sub(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }

      void operator-=(const DMatrix<T, ARRAY>& rhs)
         {
         Sub(*this, rhs);
         return;
         }
 
      DMatrix<T, ARRAY> operator-(T rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), NumCols());
         if (!DMatrix<T, ARRAY>::Sub(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }

      void operator-=(T s)
         {
         Sub(s);
         return;
         }
 
      DMatrix<T, ARRAY> operator*(DMatrix<T, ARRAY>& rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), rhs.NumCols());
         if (!DMatrix<T, ARRAY>::Mul(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }
        
      DMatrix<T, ARRAY> operator*(T rhs) const
         {
         DMatrix<T, ARRAY> C(NumRows(), NumCols());
         if (!DMatrix<T, ARRAY>::Mul(C, (*this), rhs))
            {
            // Throw
            } // end if
         return (C);
         }

      void operator*=(T s)
         {
         Mul(s);
         return;
         }
         
      // Transpose this matrix in place
      void Transpose();
      
      // Make this matrix the transpose of A
      void Transpose(const DMatrix<T, ARRAY>& A);
      
      // Compute the inverse of A
      static bool Invert(DMatrix<T, ARRAY>& A, DMatrix<T, ARRAY>& AI);
      bool Invert(DMatrix<T, ARRAY>& AI)
         {
         return (Invert((*this), AI));
         }
      
      // Solve simultaneous equations
      static bool GaussElim(DMatrix<T, ARRAY>& A, std::vector<T>& b,
            std::vector<T>& x);
      // Given an upper triangular matix, back solve equations      
      static bool SolveTriangular(DMatrix<T, ARRAY>& A, std::vector<T>& b,
            std::vector<T>& x);
      
      // Multiply vector rhs by the matrix A      
      static bool MulVector(DMatrix<T, ARRAY>& A, std::vector<T>& rhs,
            std::vector<T>& out);
      
      // Solve simultaneous equations with this matrix
      bool GaussElim(std::vector<T>& b, std::vector<T>& x)
         {
         return (GaussElim(*this, b, x));
         }
      
      // Multiply the vector rhs by this matrix
      bool MulVector(std::vector<T>& rhs, std::vector<T>& out)
         {
         return (MulVector(*this, rhs, out));
         }
         
      // Exchange two rows in the matrix
      void SwapRows(size_t r1, size_t r2)
         {
         m_A.SwapRows(r1, r2);
         return;
         }
      
   protected :
      ARRAY m_A;  // Elements
      T m_ZeroTest;
      
      // Make a copy
      void Copy(const DMatrix<T, ARRAY>& src)
         {
         m_ZeroTest = src.m_ZeroTest;
         m_A = src.m_A;
         return;
         }
      
      // Find the largest column element to use as the pivot row
      size_t FindMaxPivot(size_t nCol, T& MaxPivot) const
         {
         T AbsMaxPivot = std::abs((*this)[nCol][nCol]);
         size_t nPivotRow = nCol;
         for (size_t r = nCol + 1 ; r < NumRows() ; r++)
            {
            T Test = std::abs((*this)[r][nCol]);
            if (Test > AbsMaxPivot)
               {
               AbsMaxPivot = Test;
               nPivotRow = r;
               } // end if
            } // end for
         MaxPivot = (*this)[nPivotRow][nCol];
         return (nPivotRow);
         }
      
   private :
   };  // End of class DMatrix
 
/*****************************************************************************
************************ Class DMatrix Implementation ************************
*****************************************************************************/

/*****************************************************************************
*
*  DMatrix::operator==
*
*  Test the passed right hand side matrix with this matrix for equality.
*  We accept the matrices are equal if they're the same size and the element
*  differences are within the Zero Test.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::operator==(const DMatrix<T, ARRAY>& rhs)
   {
   bool bRet = (NumRows() == rhs.NumRows()) && (NumCols() == rhs.NumCols());

   for (size_t r = 0 ; (r < NumRows()) && bRet ; r++)
      {
      T* R = (*this)[r];
      const T* RT = rhs[r];
      for (size_t c = 0 ; (c < NumCols()) && bRet ; c++)
         {
         bRet = std::abs(R[c] - RT[c]) < m_ZeroTest;
         } // end for
      } // end for
   
   return (bRet);

   } // End of function DMatrix::operator== 
  
/*****************************************************************************
*
*  DMatrix::Diagonal
*
*  Set this matrix to a diagonal matrix.  This matrix must be square.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Diagonal(const T& d)
   {
   bool bRet = IsSquare();
   if (bRet)
      {
      for (size_t r = 0 ; r < NumRows() ; r++)
         {
         T* R = (*this)[r];
         for (size_t c = 0 ; c < NumCols() ; c++)
            {
            R[c] = (r == c) ? d : static_cast<T>(0);
            } // end for
         } // end for
      } // end if
      
   return (bRet);

   } // End of function DMatrix::Diagonal 
   
/*****************************************************************************
*
*  DMatrix::Add
*
*  Add a scalar to this matrix.
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Add(T s)
   {
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      T* R = (*this)[r];
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] += s;
         } // end for
      } // end for
      
   return;

   } // End of function DMatrix::Add 
      
/*****************************************************************************
*
*  DMatrix::Add
*
*  Make the Result matrix equal to the sum of the matrix A and the scalar s.
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Add(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, T s)
   {
   Result.MakeSameSize(A);
      
   for (size_t r = 0 ; r < Result.NumRows() ; r++)
      {
      T* R = Result[r];
      const T* RA = A[r];
      for (size_t c = 0 ; c < Result.NumCols() ; c++)
         {
         R[c] = RA[c] + s;
         } // end for
      } // end for
    
   return;

   } // End of function DMatrix::Add 
         
/*****************************************************************************
*
*  DMatrix::Add
*
*  Add the matrix a to this matrix.  Sizes must be the same.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Add(const DMatrix<T, ARRAY>& A)
   {
   bool bRet = IsSameSize(A);
   if (bRet)
      {
      for (size_t r = 0 ; r < NumRows() ; r++)
         {
         T* R = (*this)[r];
         T* RA = A[r];
         for (size_t c = 0 ; c < NumCols() ; c++)
            {
            R[c] += RA[c];
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::Add 
      
/*****************************************************************************
*
*  DMatrix::Add
*
*  Make the Result matrix the sum of the matrices A & B.  The input matrices
*  must be the same size.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Add(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
   {
   bool bRet = B.IsSameSize(A);
   if (bRet)
      {
      Result.MakeSameSize(A);
       
      for (size_t r = 0 ; r < Result.NumRows() ; r++)
         {
         T* R = Result[r];
         const T* RA = A[r];
         const T* RB = B[r];
         for (size_t c = 0 ; c < Result.NumCols() ; c++)
            {
            R[c] = RA[c] + RB[c];
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::Add 

/*****************************************************************************
*
*  DMatrix::Sub
*
*  Subtract a scalar from this matrix.
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Sub(T s)
   {
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      T* R = (*this)[r];
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] -= s;
         } // end for
      } // end for
      
   return;

   } // End of function DMatrix::Sub 
      
/*****************************************************************************
*
*  DMatrix::Sub
*
*  Make the Result matrix equal to the differece of the matrix A and the
*  scalar s.
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Sub(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, T s)
   {
   Result.MakeSameSize(A);
      
   for (size_t r = 0 ; r < Result.NumRows() ; r++)
      {
      T* R = Result[r];
      const T* RA = A[r];
      for (size_t c = 0 ; c < Result.NumCols() ; c++)
         {
         R[c] = RA[c] - s;
         } // end for
      } // end for
    
   return;

   } // End of function DMatrix::Sub 
          
/*****************************************************************************
*
*  DMatrix::Sub
*
*  Subtract the matrix A from this matrix.  Sizes must be the same.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Sub(const DMatrix<T, ARRAY>& A)
   {
   bool bRet = IsSameSize(A);
   if (bRet)
      {
      for (size_t r = 0 ; r < NumRows() ; r++)
         {
         T* R = (*this)[r];
         T* RA = A[r];
         for (size_t c = 0 ; c < NumCols() ; c++)
            {
            R[c] -= RA[c];
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::Sub 
         
/*****************************************************************************
*
*  DMatrix::Sub
*
*  Make the Result matrix the difference of the matrices A & B.  The input
*  matrices must be the same size.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Sub(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
   {
   bool bRet = B.IsSameSize(A);
   if (bRet)
      {
      Result.MakeSameSize(A);
       
      for (size_t r = 0 ; r < Result.NumRows() ; r++)
         {
         T* R = Result[r];
         const T* RA = A[r];
         const T* RB = B[r];
         for (size_t c = 0 ; c < Result.NumCols() ; c++)
            {
            R[c] = RA[c] - RB[c];
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::Sub 

/*****************************************************************************
*
*  DMatrix::Mul
*
*  Multiply this matrix by a scalar.
*
*****************************************************************************/

template <typename T, typename ARRAY>
void DMatrix<T, ARRAY>::Mul(T s)
   {
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      T* R = (*this)[r];
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] *= s;
         } // end for
      } // end for
      
   return;

   } // End of function DMatrix::Mul 
      
/*****************************************************************************
*
*  DMatrix::Mul
*
*  Make the Result matrix equal to the product of the matrix A and the
*  scalar s.
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Mul(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, T s)
   {
   Result.MakeSameSize(A);
      
   for (size_t r = 0 ; r < Result.NumRows() ; r++)
      {
      T* R = Result[r];
      const T* RA = A[r];
      for (size_t c = 0 ; c < Result.NumCols() ; c++)
         {
         R[c] = RA[c] * s;
         } // end for
      } // end for
    
   return;

   } // End of function DMatrix::Mul 
 
/*****************************************************************************
*
*  DMatrix::Mul
*
*  Make the Result matrix the product of the matrices A & B.  The input
*  matrices must be conformable.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::Mul(DMatrix<T, ARRAY>& Result,
      const DMatrix<T, ARRAY>& A, const DMatrix<T, ARRAY>& B)
   {
   // All matrices have to have conformable dimensions
   bool bRet = (A.NumCols() == B.NumRows());
   if (bRet)
      {
      Result.Resize(A.NumRows(), B.NumCols());
      
      // Dot each row of A with each column of B  
      for (size_t r = 0 ; r < Result.NumRows() ; r++)
         {
         T* R = Result[r];
         const T* RA = A[r];
         for (size_t c = 0 ; c < Result.NumCols() ; c++)
            {
            R[c] = 0;
            for (size_t i = 0 ; i < A.NumCols() ; i++)
               {
               R[c] += RA[i] * B[i][c];
               } // end for
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::Mul 

/*****************************************************************************
*
*  DMatrix::MulMulATransposexB
*
*  Make this matrix the product of the matrices A transpose & B.  The input
*  matrices must be conformable.  Faster and less memory than taking the
*  transpose and then multiplying.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::MulATransposexB(const DMatrix<T, ARRAY>& A,
      const DMatrix<T, ARRAY>& B)
   {
   // All matrices have to have conformable dimensions
   bool bRet = (A.NumRows() == B.NumRows());
   if (bRet)
      {
      // Make sure dest is conformable
      if ((NumRows() != A.NumCols()) || (NumCols() != B.NumCols()))
         {
         Resize(A.NumCols(), B.NumCols());
         } // end if
        
      for (size_t r = 0 ; r < NumRows() ; r++)
         {
         T* R = (*this)[r];
         for (size_t c = 0 ; c < NumCols() ; c++)
            {
            R[c] = 0;
            for (size_t i = 0 ; i < B.NumRows() ; i++)
               {
               R[c] += A[i][r] * B[i][c];
               } // end for
            } // end for
            
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::MulMulATransposexB 

/*****************************************************************************
*
*  DMatrix::MulAxBTranspose
*
*  Make this matrix the product of the matrices A & B transpose.  The input
*  matrices must be conformable.  Faster and less memory than taking the
*  transpose and then multiplying.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::MulAxBTranspose(const DMatrix<T, ARRAY>& A,
      const DMatrix<T, ARRAY>& B)
   {
   // All matrices have to have conformable dimensions
   bool bRet = (A.NumCols() == B.NumCols());
   if (bRet)
      {
      // Make sure dest is conformable
      if ((NumRows() != A.NumRows()) || (NumCols() != B.NumRows()))
         {
         Resize(A.NumRows(), B.NumRows());
         } // end if
        
      for (size_t r = 0 ; r < NumRows() ; r++)
         {
         T* R = (*this)[r];
         const T* RA = A[r];
         for (size_t c = 0 ; c < NumCols() ; c++)
            {
            R[c] = 0;
            const T* RB = B[c];
            for (size_t i = 0 ; i < A.NumCols() ; i++)
               {
               R[c] += RA[i] * RB[i];
               } // end for
            } // end for
         } // end for
      } // end if
   
   return (bRet);

   } // End of function DMatrix::MulAxBTranspose 

/*****************************************************************************
*
*  DMatrix::Transpose
*
*  Transpose this matrix in place.  TTC Need to look at this in terms of
*  matrices with row exchanges
*
*****************************************************************************/

DMatrixTemplate
void DMatrix<T, ARRAY>::Transpose()
   {
   if (IsSquare())
      {
      // Only work the upper trianglar elements (diagonal not moved)
      for (size_t r = 1 ; r < NumRows() ; r++)
         {
         // Make sure row access is optimized
         T* R = (*this)[r];
         for (size_t c = r ; c < NumCols() ; c++)
            {
            std::swap((*this)[c][r], R[c]);
            } // end for
         } // end for
      } // end if
   else
      {
      // Nontrivial to do in place, just bite the bullet for now
      DMatrix<T, ARRAY> Temp(*this);
      Transpose(Temp);
      } // end else

   return;

   } // End of function DMatrix::Transpose 
     
/*****************************************************************************
*
*  DMatrix::Transpose
*
*  Make this matrix the transpose of matrix A.
*
*****************************************************************************/

template <typename T, typename ARRAY>
void DMatrix<T, ARRAY>::Transpose(const DMatrix<T, ARRAY>& A)
   {
   Resize(A.NumCols(), A.NumRows());
      
   // Work this matrix a row at a time
   for (size_t r = 0 ; r < NumRows() ; r++)
      {
      // Make sure row access is optimized
      T* R = (*this)[r];
      
      // Work down the corresponding column of A
      for (size_t c = 0 ; c < NumCols() ; c++)
         {
         R[c] = A[c][r];
         } // end for
      } // end for
    
   return;

   } // End of function DMatrix::Transpose 

/*****************************************************************************
*
*  DMatrix::Invert
*
*  Invert the passed matrix A and return the result in AI. On exit, A will
*  contain the Identity matrix within calculated precision. Gauss-Jordan
*  elimination is used with row pivoting.  The augmented matrix containing
*  both A and AI is not actually built but the row operations are done in
*  parallel.  For AI, a temporary vector of row pointers is made to track the
*  pivot row exchanges which means the original vector in the matrix is
*  intact and the row order doesn't need to be restored at the end of the
*  process.
*
*****************************************************************************/

template <typename T, typename ARRAY>
bool DMatrix<T, ARRAY>::Invert(DMatrix<T, ARRAY>& A, DMatrix<T, ARRAY>& AI)
   {
   // Matrix must be square and bigger than 0
   bool bRet = (A.NumRows() > 0) && A.IsSquare();
   if (bRet)
      {
      AI.MakeSameSize(A);
//      AI.InitRowPtrs();
      AI.Identity();
         
      // Check for the trivial 1x1 scalar case
      if (A.NumRows() == 1)
         {
         if (A[0][0] != 0)
            {
            // 1x1 matrix aka a scalar
            AI[0][0] = 1 / A[0][0];
            } // end if
         else
            {
            bRet = false;
            } // end else
         } // end if
      // A "real" matrix
      else
         {
         const size_t n = A.NumRows();
         
         // Now do the elimination
         // Eliminate each column
         for (size_t c = 0 ; (c < n) && bRet ; c++)
            {
            // Keep the compiler from making a row dereference every pass
            T* C = A[c];
                     
            // Find the largest column element to use as the pivot row
            T MaxP;
            size_t p = A.FindMaxPivot(c, MaxP);

            // Check for singular matrix
            bRet = (std::abs(MaxP) > A.m_ZeroTest);
            if (bRet)
               {
               // Swap rows with the largest pivot if necessary
               if (p != c)
                  {
                  // Swap the rows by excanging row pointers
                  A.SwapRows(c, p);
  
                   // Swaw the rows of AI, too
                  AI.SwapRows(c, p);
                  
                  // Need to reestablish the current row pointers
                  C = A[c];
                  } // end if   

               T* IC = AI[c];

               // Divide the pivot row by its diagonal
               // T Pivot = A[c][c];
               T Pivot = C[c];
               for (size_t j = c ; j < n ; j++)
                  {
                  // A[c][j] /= Pivot;
                  C[j] /= Pivot;
                  } // end for
               
               // Divide the pivot row in the augmented matrix      
               for (size_t j = 0 ; j < n ; j++)
                  {
                  // AI[c][j] /= Pivot;
                  IC[j] /= Pivot;
                  } // end for
            
               // Multiply and subtract the pivot row from the rows below
               for (size_t r = c + 1 ; r < n ; r++)
                  {
                  T* R = A[r];
                  T* IR = AI[r];
                  
                  // T m = A[r][c];
                  T m = R[c];
                  for (size_t j = c ; j < n ; j++)
                     {
                     // A[r][j] -= m * A[c][j];
                     R[j] -= m * C[j];
                     } // end for
                     
                  for (size_t j = 0 ; j < n ; j++)
                     {
                     // AI[r][j] -= m * AI[c][j];
                     IR[j] -= m * IC[j];
                     } // end for
                  } // end for Row Loop
               } // end if         
            } // end for Column Elimination upper diagonal

         // Final check for singularity
         bRet = (std::abs(A[n - 1][n - 1]) > A.m_ZeroTest);
         
         // Now eliminate the elements above the diagonal
         if (bRet)
            {
            // Eliminate each column
            for (size_t c = 1 ; c < n ; c++)
               {
               T* C = A[c];
               T* IC = AI[c];
               for (size_t r = 0 ; r < c ; r++)
                  {
                  T* R = A[r];
                  T m = R[c];
                  for (size_t j = c ; j < n ; j++)
                     {
                     R[j] -= m * C[j];
                     } // end for
                     
                  T* IR = AI[r];
                  for (size_t j = 0 ; j < n ; j++)
                     {
                     IR[j] -= m * IC[j];
                     } // end for
                  } // end for Row Loop
               } // end for Column Loop
            } // end if
         } // end else
      } // end if

   return (bRet);

   } // End of function DMatrix::Invert 

/*****************************************************************************
*
*  DMatrix::GaussElim
*
*  Solve a system of simultaneous equations Ax = b using Gaussian Elimination
*  with pivoting.  The input matrix and vector b will be trashed on exit.  The
*  matrix and b are returned in their triangularized form.
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::GaussElim(DMatrix<T, ARRAY>& A, std::vector<T>& b,
            std::vector<T>& x)
   {
   // Matrix must be square and b must be compliant
   bool bRet = A.IsSquare() && (A.NumCols() == b.size());
   if (bRet)
      {
      const size_t n = A.NumRows();
      
      // Eliminate each column
      for (size_t c = 0 ; (c < (n - 1)) && bRet ; c++)
         {
         // Make sure the compiler doesn't do the row dereference every pass
         T* C = A[c];
         
         // Find the largest column element to use as the pivot row
         T MaxP;
         size_t p = A.FindMaxPivot(c, MaxP);

         // Check for singular matrix
         bRet = (std::abs(MaxP) > A.m_ZeroTest);
         if (bRet)
            {
            // Swap rows with the largest pivot if necessary
            if (p != c)
               {
#if 0  // Less efficient but easier to understand code               
               // Values to the left of current column are zero.
               T* P = A[p];
               for (size_t j = c ; j < n ; j++)
                  {
                  // std::swap(A[c][j], A[p][j]);
                  std::swap(C[j], P[j]);
                  } // end for
#endif

               // Swap the rows
               A.SwapRows(c, p);
               
               // Reestablish the current row pointer
               C = A[c];
                  
               // The vector of constants has to be swapped, too   
               std::swap(b[c], b[p]);
               } // end if   
         
            // Divide the pivot row by its diagonal
            // T Pivot = A[c][c];
            T Pivot = C[c];
            for (size_t j = c ; j < n ; j++)
               {
               // A[c][j] /= Pivot;
               C[j] /= Pivot;
               } // end for
            b[c] /= Pivot;
            
            // Multiply and subtract the pivot row from the rows below
            for (size_t r = c + 1 ; r < n ; r++)
               {
               T* R = A[r];
               // T m = A[r][c];
               T m = R[c];
               for (size_t j = c ; j < n ; j++)
                  {
                  // A[r][j] -= m * A[c][j];
                  R[j] -= m * C[j];
                  } // end for
                  
               // Handle the constants
               b[r] -= m * b[c];
               } // end for
            } // end if         
         } // end for Column Elimination

      // Final check for singularity
      bRet = (std::abs(A[n - 1][n - 1]) > A.m_ZeroTest);
      
      // Do the back substitution
      if (bRet)
         {
         bRet = SolveTriangular(A, b, x);
         } // end if
     } // end if
         
   return (bRet);

   } // End of function DMatrix::GaussElim 

/*****************************************************************************
*
*  DMatrix::SolveTriangular
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::SolveTriangular(DMatrix<T, ARRAY>& A,
      std::vector<T>& b, std::vector<T>& x)
   {
   // Matrix must be square and b must be compliant
   bool bRet = A.IsSquare() && (A.NumCols() == b.size());
   if (bRet)
      {
      // Resize the solution vector if necessary
      if (x.size() != A.NumCols())
         {
         x.resize(A.NumCols());
         } // end if

      const size_t n = A.NumRows();
      
      // x[n] = b[n] to get started
      x[n - 1] = b[n - 1] / A[n - 1][n - 1];
      
      // Now back substitute to compute the rest of the x's
      for (int i = n - 2 ; i >= 0 ; i--)
         {
         x[i] = b[i];
         for (size_t j = i + 1 ; j < A.NumCols() ; j++)
            {
            x[i] -= A[i][j] * x[j];
            } // end for
            
         x[i] /= A[i][i];
         } // end for
      } // end if   

   return (bRet);

   } // End of function DMatrix::SolveTriangular 

/*****************************************************************************
*
*  DMatrix::MulVector
*
*****************************************************************************/

DMatrixTemplate
bool DMatrix<T, ARRAY>::MulVector(DMatrix<T, ARRAY>& A, std::vector<T>& rhs,
      std::vector<T>& out)
   {
   // Matrix A and vector b must be compliant
   bool bRet = (A.NumCols() == rhs.size());
   if (bRet)
      {
      // Resize the output vector if necessary
      if (out.size() != A.NumRows())
         {
         out.resize(A.NumRows());
         } // end if

      // Do the maxtrix row and rhs vector dot products
      for (size_t i = 0 ; i < A.NumRows() ; i++)
         {
         out[i] = 0;
         T* RA = A[i];
         for (size_t j = 0 ; j < A.NumCols() ; j++)
            {
            out[i] += RA[j] * rhs[j];
            } // end for
         } // end for
      } // end if   

   return (bRet);

   } // End of function DMatrix::MulVector 

#endif // __DMATRIX_H__
