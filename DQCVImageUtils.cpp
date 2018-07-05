/*
 * DQCVImageUtils.cpp
 *
 *  Created on: May 10, 2014
 *      Author: Tim
 */

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QDebug>
#include "DQCVImageUtils.h"

static void QImageCleanupCVMat(void* pMat);

/*****************************************************************************
*
***  cvMatToQImage
*
* If bCloneData is true, the resulting QImage is completely independent of
* the original cv::Mat.  The image data is cloned into a new cv::Mat allocated
* on the heap.  The QImage is given a pointer to this copy and it is deleted
* by the cleanup function, QImageCleanupCVMat, when the QImage destructor is
* called.  bCloneData is defaulted to true as this is the safest option.
*
* If bCloneData is false, the resulting QImage and cv::Mat share the same
* memory buffer for the image data.  The cv::Mat MUST EXIST for the lifetime
* of the QImage and the programmer is responsible for managing this.  Also,
* changes to either are reflected in the other.  For the CV_8UC3 case, the
* cv::Mat image data is modified on creation because it is RGB swapped.  This
* is fine for cases where the cv::Mat isn't used and the QImage is just
* created for display.  BE ADVISED, if continued processing is going to take
* place on the cv::Mat, this must be taken into account by reswapping RGB.
*
*****************************************************************************/

QImage cvMatToQImage(const cv::Mat& MatIn, bool bCloneData /* = true */)
   {
   QImage ImageOut;

   switch (MatIn.type())
      {
      case CV_8UC4:
         {
         // 8-bit, 4 channel
         if (bCloneData)
            {
            cv::Mat* pTempMat = new cv::Mat(MatIn.clone());
            ImageOut = QImage(pTempMat->data, static_cast<int>(pTempMat->cols), static_cast<int>(pTempMat->rows),
                  static_cast<int>(pTempMat->step), QImage::Format_ARGB32, QImageCleanupCVMat, pTempMat);
            } // end if
         else
            {
            ImageOut = QImage(MatIn.data, static_cast<int>(MatIn.cols), static_cast<int>(MatIn.rows),
                  static_cast<int>(MatIn.step), QImage::Format_ARGB32);
            } // end else
         break;
         }

      case CV_8UC3:
         {
         // 8-bit, 3 channel
         if (bCloneData)
            {
            cv::Mat* pTempMat = new cv::Mat(MatIn.clone());
            ImageOut = QImage(pTempMat->data, static_cast<int>(pTempMat->cols), static_cast<int>(pTempMat->rows),
                  static_cast<int>(pTempMat->step), QImage::Format_RGB888, QImageCleanupCVMat, pTempMat).rgbSwapped();
            } // end if
         else
            {
            ImageOut = QImage(MatIn.data, static_cast<int>(MatIn.cols), static_cast<int>(MatIn.rows),
                  static_cast<int>(MatIn.step), QImage::Format_RGB888).rgbSwapped();
            } // end else
         break;
         }

      case CV_8UC1:
         {
         // 8 bit, 1 channel
         if (bCloneData)
            {
            cv::Mat* pTempMat = new cv::Mat(MatIn.clone());
            DQImage DImage(pTempMat->data, static_cast<int>(pTempMat->cols), static_cast<int>(pTempMat->rows),
                  static_cast<int>(pTempMat->step), QImage::Format_Indexed8, QImageCleanupCVMat, pTempMat);
            DImage.SetLinearColorTable(true, true, true);
            ImageOut = DImage;
            } // end if
         else
            {
            DQImage DImage(MatIn.data, static_cast<int>(MatIn.cols), static_cast<int>(MatIn.rows),
                  static_cast<int>(MatIn.step), QImage::Format_Indexed8);
            DImage.SetLinearColorTable(true, true, true);
            ImageOut = DImage;
            } // end else
         break;
         }

      default:
         {
         qWarning() << "cvMatToQImage() - cv::Mat image type not handled in switch:" << MatIn.type();
         break;
         }
      } // end switch

   return (ImageOut);

   } // end of function cvMatToQImage

/******************************************************************************
*
***  QImageCleanupCVMat
*
* Cleanup function used by cvMatToQImage to delete the copy of a cv::Mat it is
* managing for its image buffer.
*
******************************************************************************/

static void QImageCleanupCVMat(void* pMat)
   {
   delete reinterpret_cast<cv::Mat*>(pMat);

   return;

   } // end of function QImageCleanupCVMat


/*****************************************************************************
*
***  QImageToCvMat
*
* If ImageIn exists for the lifetime of the resulting cv::Mat, pass false to
* inCloneImageData to share ImageIn's data with the cv::Mat directly
*
* NOTE: Format_RGB888 is an exception since we need to use a local QImage and
* thus must clone the data regardless
*
*****************************************************************************/

cv::Mat QImageToCvMat(const QImage &ImageIn, bool bCloneImageData /* = true */)
   {
   cv::Mat MatOut;

   switch (ImageIn.format())
      {
      case QImage::Format_RGB32:
      case QImage::Format_ARGB32:
         {
         // 8-bit, 4 channel
         cv::Mat TempMat = cv::Mat(ImageIn.height(), ImageIn.width(), CV_8UC4, const_cast<uchar*>(ImageIn.bits()),
               ImageIn.bytesPerLine());
         if (bCloneImageData)
            {
            MatOut = TempMat.clone();
            } // end if
         else
            {
            MatOut = TempMat;
            } // end else
         break;
         }

      case QImage::Format_RGB888:
         {
         // 8-bit, 3 channel
         if (!bCloneImageData)
            {
            qWarning() << "QImageToCvMat()-Conversion requires cloning";
            } // end if

         QImage Swapped = ImageIn.rgbSwapped();
         MatOut = cv::Mat(Swapped.height(), Swapped.width(), CV_8UC3, const_cast<uchar*>(Swapped.bits()),
               Swapped.bytesPerLine()).clone();
         break;
         }


      case QImage::Format_Indexed8:
         {
         // 8-bit, 1 channel
         cv::Mat TempMat =  cv::Mat(ImageIn.height(), ImageIn.width(), CV_8UC1, const_cast<uchar*>(ImageIn.bits()),
               ImageIn.bytesPerLine());
         if (bCloneImageData)
            {
            MatOut = TempMat.clone();
            } // end if
         else
            {
            MatOut = TempMat;
            } // end else
         break;
         }

      default:
         {
         qWarning() << "QImageToCvMat(const QImage &ImageIn, bool bCloneImageData) - QImage format not handled:"
               << ImageIn.format();
         break;
         }
      } // end switch

   return (MatOut);

   } // end of function QImageToCvMat

/******************************************************************************
*
***  QImageToCvMat(const QImage &ImageIn, int nCVType)
*
* Convert the QImage to a cv::Mat in the specified format.  For cases where
* the pixel sizes are incompatible so the previous function can't be used.
*
******************************************************************************/

cv::Mat QImageToCvMat(const QImage& ImageIn, int nCVType)
   {
   cv::Mat MatOut(ImageIn.height(), ImageIn.width(), nCVType);

   switch (nCVType)
      {
      case CV_8UC3 :
         for (int r = 0 ; r < ImageIn.height() ; r++)
            {
            const QRgb* pQRow = reinterpret_cast<const QRgb*>(ImageIn.scanLine(r));
            unsigned char* pCVRow = MatOut.data + (r * MatOut.step);
            for (int c = 0 ; c < ImageIn.width() ; c++)
               {
               *pCVRow++ = static_cast<unsigned char>(qBlue(*pQRow));
               *pCVRow++ = static_cast<unsigned char>(qGreen(*pQRow));
               *pCVRow++ = static_cast<unsigned char>(qRed(*pQRow));
               pQRow++;
               } // end for
            } // end for
         break;

      case CV_8UC4 :
         for (int r = 0 ; r < ImageIn.height() ; r++)
            {
            const QRgb* pQRow = reinterpret_cast<const QRgb*>(ImageIn.scanLine(r));
            unsigned char* pCVRow = MatOut.data + (r * MatOut.step);
            for (int c = 0 ; c < ImageIn.width() ; c++)
               {
               *pCVRow++ = static_cast<unsigned char>(qBlue(*pQRow));
               *pCVRow++ = static_cast<unsigned char>(qGreen(*pQRow));
               *pCVRow++ = static_cast<unsigned char>(qRed(*pQRow));
               *pCVRow++ = static_cast<unsigned char>(qAlpha(*pQRow));
               pQRow++;
               } // end for
            } // end for
         break;

      default:
         qWarning() << "QImageToCvMat(const QImage& ImageIn, int nCVType) - CV Image Format not handled: "
               << ImageIn.format();
         break;
      } // end switch


   return(MatOut);

   } // end of function QImageToCvMat



