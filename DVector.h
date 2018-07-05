/*****************************************************************************
********************************* DVector.h **********************************
*****************************************************************************/

#if !defined(__DVECTOR_H__)
#define __DVECTOR_H__

#pragma once

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include <vector>
#include <cassert>

/*****************************************************************************
******************************* class DVector ********************************
*****************************************************************************/

template <typename T>
class DVector : public std::vector<T>
   {
   protected :
      typedef std::vector<T> BASE;
   
   public :
      typedef T T_numtype;
      
      DVector()
         {
         return;
         }
      
      DVector(size_t nDim) : BASE(nDim)
         {
         return;
         }

      DVector(const DVector& src) : BASE(src)
         {
         return;
         }
      
      DVector(const BASE& src) : BASE(src)
         {
         return;
         }
      
      ~DVector()
         {
         return;
         }
      
      DVector<T>& operator=(const DVector<T>& rhs)
         {
         BASE::operator=(rhs);
         return (*this);
         }

      DVector<T>& operator=(const BASE& rhs)
         {
         BASE::operator=(rhs);
         return (*this);
         }

      // Return the length of this vector
      T Length() const
         {
         return (sqrt(Dot(*this, *this)));
         }
      
      // Return the dot product of x and y
      static T Dot(const DVector<T>& x, const DVector<T>& y)
         {
         assert(x.size() == y.size());
         T D = 0;
         for (size_t i = 0 ; i < x.size() ; i++)
            {
            D += x[i] * y[i];
            } // end for
         return (D);
         }
      
      // Take the dot product of this vector and x
      T Dot(const DVector<T>& x) const
         {
         return (Dot(*this, x));
         }
      
      // Reduce this to a unit vector
      bool Normalize()
         {
         T L = Length();
         bool bRet = (L > 0);
         if (bRet)
            {
            for (size_t i = 0 ; i < this->size() ; i++)
               {
               (*this)[i] /= L;
               } // end for
            } // end if
         return (bRet);
         }

      // Make this vector the cross product of a and b (a x b)
      bool Cross(const DVector<T>& a, const DVector<T>& b);
            
   protected :
   
   private :
   };  // End of class DVector

/*****************************************************************************
************************ Class DVector Implementation ************************
*****************************************************************************/

/*****************************************************************************
*
*  DVector::Cross
*
*  Make this vector the cross product of a and b (a x b).  The cross product
*  produces a vector perpendicular to the crossed pair with a length
*  l = sin(theta)|a||b|.  The cross product only defined for dimensions 1, 2,
*  3, and 7.  Dimension 3 is the only one that's really useful so we'll fail
*  on everything else.
*
*****************************************************************************/

template <typename T>
bool DVector<T>::Cross(const DVector<T>& a, const DVector<T>& b)
   {
   // a and b must be the same size and have dimension 3
   bool bRet = (a.size() == b.size()) && (a.size() == 3);
   if (bRet)
      {
      // This vector will have dimension equal to a and b
      resize(a.size());
      
      if (a.size() == 3)
         {
         // Just do the 3D case directly
         (*this)[0] = (a[1] * b[2]) - (b[1] * a[2]);
         (*this)[1] = (a[2] * b[0]) - (b[1] * a[0]);
         (*this)[2] = (a[0] * b[1]) - (b[0] * a[1]);
         } // end if
      } // end if
      
   return (bRet);

   } // End of function DVector::Cross 
                        
#endif // __DVECTOR_H__
