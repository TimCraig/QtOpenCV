/*****************************************************************************
******************************** DHistogram.h ********************************
*****************************************************************************/

#if !defined(__DHISTOGRAM_H__)
#define __DHISTOGRAM_H__

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

/*****************************************************************************
****************************** class DHistogram ******************************
*****************************************************************************/

/*
   Very simple histrogram class.
*/

class DHistogram
   {
   public :
      DHistogram();
      DHistogram(const DHistogram& src) = delete;

      ~DHistogram() = default;

      DHistogram& operator=(const DHistogram& rhs) = delete;

      void Clear();
      void IncValue(unsigned char nValue);
      
      unsigned int GetBin(int nBin) const
         {
         return (m_nBins[nBin]);
         }
      
      int GetBinCount() const
         {
         return (m_nBinCount);
         }

      int GetMaxCount() const
         {
         return (m_nBins[m_nMaxBin]);
         }
      
      int GetMaxBin() const
         {
         return (m_nMaxBin);
         }
      
   protected :
      static const int m_nBinCount = 256;
      
      int m_nMaxBin;
      unsigned int m_nBins[m_nBinCount];
      
   private :

   };  // End of class DHistogram
                     
#endif // __DHISTOGRAM_H__
