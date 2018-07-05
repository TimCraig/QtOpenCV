/*****************************************************************************
******************************* DHistogram.cpp *******************************
*****************************************************************************/

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include "DHistogram.h"

#include <memory.h>

/*****************************************************************************
********************** Class DHistogram Implementation ***********************
*****************************************************************************/

/*****************************************************************************
*
*  DHistogram::DHistogram
*
*****************************************************************************/

DHistogram::DHistogram()
   {
   Clear();
         
   return;

   } // End of function DHistogram::DHistogram 
   
/*****************************************************************************
*
*  DHistogram::Clear
*
*****************************************************************************/

void DHistogram::Clear()
   {
   m_nMaxBin = 0;
   
   memset(m_nBins, 0, m_nBinCount * sizeof(m_nBins[0]));

   return;

   } // End of function DHistogram::Clear 
      
/*****************************************************************************
*
*  DHistogram::IncValue
*
*****************************************************************************/

void DHistogram::IncValue(unsigned char nValue)
   {
   m_nBins[nValue]++;
   
   if (m_nBins[nValue] > m_nBins[m_nMaxBin])
      {
      m_nMaxBin = nValue;
      } // end if
      
   return;

   } // End of function DHistogram::IncValue 
   
