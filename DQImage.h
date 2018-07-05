/*
 * DQImage.h
 *
 *  Created on: Apr 6, 2014
 *      Author: Tim
 */

#ifndef DQIMAGE_H_
#define DQIMAGE_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QImage>
#include <opencv2/core.hpp>

#include "DHistogram.h"

/*****************************************************************************
 *
 ***  class DQImage
 *
 * This class extends QImage by adding to functionality while still allowing
 * interoperability with functions taking a QImage as a parameter.
 *
 *****************************************************************************/

class DQImage : public QImage
   {
   public:
      DQImage() : QImage(), m_ROI(QRect())
         {
         return;
         }

      DQImage(const QSize& Size, QImage::Format eFormat) : QImage(Size, eFormat), m_ROI(QRect())
         {
         return;
         }

      DQImage(int nWidth, int nHeight, QImage::Format eFormat) : QImage(nWidth, nHeight, eFormat), m_ROI(QRect())
         {
         return;
         }

      DQImage(uchar* pData, int nWidth, int nHeight, Format eFormat, QImageCleanupFunction CleanupFunction = nullptr,
            void* pCleanupInfo = nullptr) :
            QImage(pData, nWidth, nHeight, eFormat, CleanupFunction, pCleanupInfo), m_ROI(QRect())
         {
         return;
         }

      DQImage(const uchar* pData, int nWidth, int nHeight, Format eFormat,
            QImageCleanupFunction CleanupFunction = nullptr, void* pCleanupInfo = nullptr) :
            QImage(pData, nWidth, nHeight, eFormat, CleanupFunction, pCleanupInfo), m_ROI(QRect())
         {
         return;
         }

      DQImage(uchar* pData, int nWidth, int nHeight, int nBytesPerLine, Format eFormat,
            QImageCleanupFunction CleanupFunction = nullptr, void* pCleanupInfo = nullptr) :
            QImage(pData, nWidth, nHeight, nBytesPerLine, eFormat, CleanupFunction, pCleanupInfo), m_ROI(QRect())
         {
         return;
         }

      DQImage(const uchar* pDdata, int nWidth, int nHeight, int nBytesPerLine, Format eFormat,
            QImageCleanupFunction CleanupFunction = nullptr, void* pCleanupInfo = nullptr) :
            QImage(pDdata, nWidth, nHeight, nBytesPerLine, eFormat, CleanupFunction, pCleanupInfo), m_ROI(QRect())
         {
         return;
         }

      explicit DQImage(const char** pXPM) : QImage(pXPM), m_ROI(QRect())
         {
         return;
         }

      DQImage(const QString& strFileName, const char* pFormat = nullptr) : QImage(strFileName, pFormat), m_ROI(QRect())
         {
         return;
         }

      DQImage(const DQImage& src) : QImage(src), m_ROI(src.m_ROI)
         {
         return;
         }

      DQImage(const QImage& src) : QImage(src), m_ROI(QRect())
         {
         return;
         }

      DQImage(DQImage&& Other) : QImage(Other), m_ROI(Other.m_ROI)
         {
         return;
         }

      ~DQImage() = default;


      DQImage& operator=(const DQImage& rhs)
         {
         if (this != &rhs)
            {
            QImage::operator=(rhs);
            m_ROI = rhs.m_ROI;
            } // end if

         return (*this);
         }

      DQImage& operator=(const QImage& rhs)
         {
         if (this != &rhs)
            {
            QImage::operator=(rhs);
            m_ROI = QRect();
            } // end if

         return (*this);
         }

      DQImage& operator=(const DQImage&& rhs)
         {
         if (this != &rhs)
            {
            QImage::operator=(rhs);
            m_ROI = rhs.m_ROI;
            } // end if

         return (*this);
         }

      DQImage& operator=(const QImage&& rhs)
         {
         if (this != &rhs)
            {
            QImage::operator=(rhs);
            m_ROI = QRect();
            } // end if

         return (*this);
         }

      // Deep copy
      DQImage copy(const QRect& Rect = QRect()) const
         {
         DQImage Image(QImage::copy(Rect));
         Image.m_ROI = m_ROI;
         return (Image);
         }

      // Create a new indexed 8 bit image from a single color plane
      DQImage RedChannel() const;
      DQImage GreenChannel() const;
      DQImage BlueChannel() const;

      // Create a new inexed 8 bit image from the color planes
      // in HSV.  Hue is scaled to the range 0-255 from 0-359.
      DQImage HSVHue8Channel(bool bRed = true, bool bGreen = false, bool bBlue = true) const;
      DQImage HSVSaturationChannel(bool bRed = false, bool bGreen = true, bool bBlue = true) const;
      DQImage HSVValueChannel(bool bRed = true, bool bGreen = true, bool bBlue = true) const;

      // Create a new inexed 8 bit image from the color planes
      // in HSL.  Hue is scaled to the range 0-255 from 0-359.
      DQImage HSLHue8Channel(bool bRed = true, bool bGreen = false, bool bBlue = true) const;
      DQImage HSLSaturationChannel(bool bRed = false, bool bGreen = true, bool bBlue = true) const;
      DQImage HSLLightnessChannel(bool bRed = true, bool bGreen = true, bool bBlue = true) const;

      // Set up a linear color table for 8 bit indexed images
      bool SetLinearColorTable(bool bRed, bool bGreen, bool bBlue);

      // Create a new grayscale image from this image
      DQImage Gray() const;

      // Region of interest functions
      const QRect& GetROI() const
         {
         return (m_ROI);
         }

      void ClearROI()
         {
         m_ROI = QRect();
         return;
         }

      void SetROI(const QRect& ROI)
         {
         m_ROI = ROI;
         return;
         }

      // Get the histogram of single plane images
      bool CalcHistogram(DHistogram& Histogram, bool bUseROI = true) const;

   protected:
      // Add a Region of Interest rectangle
      QRect m_ROI;

   private:

   }; // end of class DQImage

#endif /* DQIMAGE_H_ */
