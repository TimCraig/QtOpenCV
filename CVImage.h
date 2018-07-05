/*****************************************************************************
********************************* CVImage.h **********************************
*****************************************************************************/

#if !defined(__CVIMAGE_H__)
#define __CVIMAGE_H__

#pragma once

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#include "DConfig.h"

/*****************************************************************************
******************************* class DCVImage *******************************
*****************************************************************************/

class DCVImage : public cv::Mat
   {
   public :
      // Image combination styles
      enum class ECombine
         {
         eSideBySide,
         eStacked
         };

      DCVImage() = default;

      DCVImage(int nWidth, int nHeight, int nType)
            : cv::Mat(nHeight, nWidth, nType)
         {
         return;
         }

      DCVImage(const DCVImage& src) : cv::Mat(src)
         {
         return;
         }

      DCVImage(const cv::Mat& src) : cv::Mat(src)
         {
         return;
         }

      DCVImage(const DCVImage* pImage1, const DCVImage* pImage2, ECombine eCombine = ECombine::eSideBySide,
            int nDivider = 0, cv::Scalar FillColor = CV_RGB(0, 0, 0))
         {
         Combine(pImage1, pImage2, eCombine, nDivider, FillColor);

         return;
         }

      ~DCVImage() = default;

      // Assignment operators
      DCVImage& operator=(const DCVImage& rhs)
         {
         cv::Mat::operator=(rhs);

         return (*this);
         }

      DCVImage& operator=(const cv::Mat& rhs)
         {
         cv::Mat::operator=(rhs);

         return (*this);
         }

      DCVImage& operator=(const cv::MatExpr& rhs)
         {
         cv::Mat::operator=(rhs);

         return (*this);
         }

      DCVImage& operator=(const cv::Scalar& s)
         {
         cv::Mat::operator=(s);

         return (*this);
         }

      // Create the image if it hasn't been already
      void Create(int nWidth, int nHeight, int nType)
         {
         cv::Mat::create(nHeight, nWidth, nType);

         return;
         }

      // Handle a deep copy
      cv::Mat Clone() const
         {
         return (cv::Mat::clone());
         }

      bool IsEmpty() const
         {
         return (empty());
         }

      bool IsValid() const
         {
         return (!IsEmpty());
         }

      // Copy this image to another one converting to this type
      // plus possible scaling and shifting
      void ConvertScale(cv::Mat& Dst, int nType, double dScale = 1.0, double dShift = 0.0) const
         {
         cv::Mat::convertTo(Dst, nType, dScale, dShift);

         return;
         }

       // Copy this image to another and convert the color space
       void CvtColor(cv::Mat& Dst, int nConversion, int nDstCn = 0)
         {
         // !!!TTC This has a new parameter, check it out
         cv::cvtColor(*this, Dst, nConversion, nDstCn);

         return;
         }

      /*
      Copy this image to the destination and add a border of the specified
      type.  If the image is to be centered, the size of this image should be
      increased by twice the offset  values.
      */
      enum class EBorderType
         {
         eBorderConstant = IPL_BORDER_CONSTANT,
         eBorderReplicate = IPL_BORDER_REPLICATE,
#if 0 // Not implemented currently
         eBorderReflect = IPL_BORDER_REFLECT,
         eBorderWrap = IPL_BORDER_WRAP,
#endif
         };
 #if 0
      void CopyMakeBorder(cv::Mat& Dst, int nTop, int nBottom, int nLeft,
            int nRight, EBorderType eType,
            const cv::Scalar& Value = cv::Scalar()) const
         {
         cv::copyMakeBorder(*this, Dst, nTop, nBottom, nLeft, nRight, eType,
               Value);
         return;
         }
#endif
      /***********************************************************************
      *************************** File Operations ****************************
      ***********************************************************************/

      enum EReadParam
         {
         eReadUnchanged = -1,
         eReadGrayScale = CV_LOAD_IMAGE_GRAYSCALE,
         eReadColor = CV_LOAD_IMAGE_COLOR,
         eReadAnyDepth = CV_LOAD_IMAGE_ANYDEPTH,
         eReadAnyColor = 4,
         };

      bool ReadImage(const std::string& szFileName, int nFlags = EReadParam::eReadUnchanged)
         {
         DCVImage Temp = cv::imread(szFileName, nFlags);
         bool bRet = Temp.IsValid();
         // Only modify the current image if read was successful
         if (bRet)
            {
            *this = Temp;
            } // end if

         return (bRet);
         }

      bool ReadImage(const char* szFileName, int nFlags = eReadColor)
         {
         return (ReadImage(std::string(szFileName), nFlags));
         }

      enum EWriteParam
         {
         eJPGQuality = cv::IMWRITE_JPEG_QUALITY,
         ePNGCompression = cv::IMWRITE_PNG_COMPRESSION,
         ePXMBinary = cv::IMWRITE_PXM_BINARY,
         };

      /***********************************************************************
      ************************** class DWriteParams **************************
      ***********************************************************************/

      class DWriteParams : public std::vector<int>
         {
         public :
            DWriteParams() : std::vector<int>()
               {
               return;
               }

            DWriteParams(const DWriteParams& src) : std::vector<int>(src)
               {
               return;
               }

            ~DWriteParams() = default;

            DWriteParams& operator=(const DWriteParams& rhs)
               {
               std::vector<int>::operator=(rhs);

               return (*this);
               }

            bool IsValid() const
               {
               // Size must be even including zero
               return ((size() & 1) == 0);
               }

            // Parameter "name" is in odd index followed by value
            void SetParameter(EWriteParam Param, int nValue)
               {
               push_back(Param);
               push_back(nValue);

               return;
               }

         };  // End of class DWriteParams

      /***********************************************************************
      ************************ class DJPGWriteParams *************************
      ***********************************************************************/

      class DJPGWriteParams : public DWriteParams
         {
         public :
            DJPGWriteParams()
               {
               // Make the OpenCV default obvious
               SetQuality(95);

               return;
               }

            DJPGWriteParams(int nQuality)
               {
               SetQuality(nQuality);

               return;
               }

            DJPGWriteParams(const DJPGWriteParams& src) : DWriteParams(src)
               {
               return;
               }

            ~DJPGWriteParams() = default;

            DJPGWriteParams& operator=(const DJPGWriteParams& rhs)
               {
               DWriteParams::operator=(rhs);

               return (*this);
               }

            void SetQuality(int nQuality)
               {
               SetParameter(eJPGQuality, nQuality);

               return;
               }

         };  // End of class DJPGWriteParams

      /***********************************************************************
      ************************ class DPNGWriteParams *************************
      ***********************************************************************/

      class DPNGWriteParams : public DWriteParams
         {
         public :
            DPNGWriteParams()
               {
               // Make the OpenCV default obvious
               SetCompression(0);

               return;
               }

            DPNGWriteParams(int nCompression)
               {
               SetCompression(nCompression);

               return;
               }

            DPNGWriteParams(const DPNGWriteParams& src) : DWriteParams(src)
               {
               return;
               }

            ~DPNGWriteParams() = default;

            DPNGWriteParams& operator=(const DPNGWriteParams& rhs)
               {
               DWriteParams::operator=(rhs);

               return (*this);
               }

            void SetCompression(int nCompression)
               {
               SetParameter(ePNGCompression, nCompression);

               return;
               }

         };  // End of class DPNGWriteParams

      bool WriteImage(const std::string& szFileName, const std::vector<int>& Params = std::vector<int>()) const
        {
        return (cv::imwrite(szFileName, *this, Params));
        }

      bool WriteImage(const char* szFileName, const std::vector<int>& Params = std::vector<int>()) const
        {
        return (WriteImage(std::string(szFileName), Params));
        }

      /***********************************************************************
      *************************** Image Properties ***************************
      ***********************************************************************/

      int GetHeight() const
         {
         return (rows);
         }

      int GetNumRows() const
         {
         return (rows);
         }

      int GetWidth() const
         {
         return (cols);
         }

      int GetNumCols() const
         {
         return (cols);
         }

      size_t GetWidthStep() const
         {
         return (step);
         }

      // Returns CV_8U, CV_8S, CV_16U,....
      int GetDepth() const
         {
         return (depth());
         }

      // Returns CV_8UC1, CV_8SC3, CV_16UC1, CV_MAKETYPE(depth, cn), ....
      int GetType() const
         {
         return (type());
         }

      int GetNumChannels() const
         {
         return (channels());
         }

      bool IsPixelDataSigned() const
         {
         int nDepth = GetDepth();

         return (!((nDepth == CV_8U) || (nDepth == CV_16U)));
         }

      // Number of bits in each color channel
      size_t GetPixelDepth() const
         {
         return ((elemSize() / GetNumChannels()) * 8);
         }

      // Pixel size in bytes
      size_t GetPixelSize() const
         {
         return (elemSize());
         }

      // Data size in bytes
      size_t GetDataSize() const
         {
         return (GetNumRows() * GetWidthStep());
         }

      // Total number of elements in matrix or pixels in image
      size_t GetNumElements() const
         {
         return (total());
         }

      /***********************************************************************
      ***************************** Pixel Access *****************************
      ***********************************************************************/
      /* Deprecated in favor of the ptr template functions built into cv::Mat */

      unsigned char* GetPixels()
         {
         return (data);
         }

      const unsigned char* GetPixels() const
         {
         return (data);
         }

      unsigned char* GetRow(int nRow)
         {
         return (GetPixels() + nRow * GetWidthStep());
         }

      const unsigned char* GetRow(int nRow) const
         {
         return (GetPixels() + nRow * GetWidthStep());
         }

       unsigned char* GetPixel(int nRow, int nCol)
         {
         return (GetRow(nRow) + nCol * GetPixelSize());
         }

      const unsigned char* GetPixel(int nRow, int nCol) const
         {
         return (GetRow(nRow) + nCol * GetPixelSize());
         }

      unsigned char* GetPixel(unsigned char* pRow, int nCol)
         {
         return (pRow + nCol * GetPixelSize());
         }

      const unsigned char* GetPixel(const unsigned char* pRow, int nCol) const
         {
         return (pRow + nCol * GetPixelSize());
         }

      bool CopyPixels(const IplImage* pImage)
         {
         bool bRet = (pImage->imageData != nullptr);
         if (bRet)
            {
            memcpy(GetPixels(), pImage->imageData, GetDataSize());
            } // end if

         return (bRet);
         }

      // Copy BRG to RGB (primarily for display support)
      void CopyPixelsToRGB(unsigned char* pPixelsOut) const;

      /***********************************************************************
      ************************* Algebraic Operations *************************
      ***********************************************************************/
      /* Generally deprecated in favor of cv::Mat native operations */

      // This matrix contains the result of adding two matrices
      void Add(const cv::Mat& Mat1, const cv::Mat& Mat2, const cv::Mat& Mask = Mat())
         {
         cv::add(Mat1, Mat2, *this, Mask);

         return;
         }

      // Add a matrix to this matrix
      void Add(const cv::Mat& Mat, const cv::Mat& Mask = Mat())
         {
         cv::add(Mat, *this, *this, Mask);

         return;
         }

      // This matrix contains the results of adding a scalar to a matrix
      void Add(const cv::Mat& Mat, const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::add(Mat, Value, *this, Mask);

         return;
         }

      // Add a scalar to this matrix
      void Add(const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::add(*this, Value, *this, Mask);

         return;
         }

      // This matrix contains the results of a weighted and scaled addition
      // of two matrices
      void AddWeighted(const cv::Mat& Mat1, double dAlpha, const cv::Mat& Mat2, double dBeta, double dGamma)
         {
         cv::addWeighted(Mat1, dAlpha, Mat2, dBeta, dGamma, *this);

         return;
         }

      // This matrix contains the result of subtracting two matrices
      void Subtract(const cv::Mat& Mat1, const cv::Mat& Mat2, const cv::Mat& Mask = Mat())
         {
         cv::subtract(Mat1, Mat2, *this, Mask);

         return;
         }

      // Subtract a matrix to this matrix
      void Subtract(const cv::Mat& Mat, const cv::Mat& Mask = Mat())
         {
         cv::subtract(Mat, *this, *this, Mask);

         return;
         }

      // This matrix contains the results of subtracting a scalar to a matrix
      void Subtract(const cv::Mat& Mat, const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::subtract(Mat, Value, *this, Mask);

         return;
         }

      // Subtract a scalar to this matrix
      void Subtract(const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::subtract(*this, Value, *this, Mask);

         return;
         }

      /***********************************************************************
      ************************** Logical Operations **************************
      ***********************************************************************/

      // This image is computed from the logical and of the inputs plus mask
      void And(const cv::Mat& Mat1, const cv::Mat& Mat2, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_and(Mat1, Mat2, *this, Mask);

         return;
         }

      // This image is anded with the supplied image and mask
      void And(const cv::Mat& Mat, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_and(*this, Mat, *this, Mask);

         return;
         }

      // This matrix contins the result of logical anding a matrix and a
      // scalar
      void And(const cv::Mat& Mat, const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_and(Mat, Value, *this, Mask);

         return;
         }

      // Logically and this matrix and a scalar
      void And(const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_and(*this, Value, *this, Mask);

         return;
         }

      // This image is computed from the logical or of the inputs plus mask
      void Or(const cv::Mat& Mat1, const cv::Mat& Mat2, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_or(Mat1, Mat2, *this, Mask);

         return;
         }

      // This image is ored with the supplied image and mask
      void Or(const cv::Mat& Mat, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_or(*this, Mat, *this, Mask);

         return;
         }

      // This matrix contins the result of logical oring a matrix and a
      // scalar
      void Or(const cv::Mat& Mat, const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_or(Mat, Value, *this, Mask);

         return;
         }

      // Logically or this matrix and a scalar
      void Or(const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_or(*this, Value, *this, Mask);

         return;
         }

#
      // This image is computed from the logical xor of the inputs plus mask
      void Xor(const cv::Mat& Mat1, const cv::Mat& Mat2, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_xor(Mat1, Mat2, *this, Mask);

         return;
         }

      // This image is xored with the supplied image and mask
      void Xor(const cv::Mat& Mat, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_xor(*this, Mat, *this, Mask);

         return;
         }

      // This matrix contins the result of logical xoring a matrix and a
      // scalar
      void Xor(const cv::Mat& Mat, const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_xor(Mat, Value, *this, Mask);

         return;
         }

      // Logically xor this matrix and a scalar
      void Xor(const cv::Scalar& Value, const cv::Mat& Mask = Mat())
         {
         cv::bitwise_xor(*this, Value, *this, Mask);

         return;
         }

      // This matrix contains the bitwise not of the source matrix
      void Not(const cv::Mat& Mat)
         {
         cv::bitwise_not(Mat, *this);

         return;
         }

      /***********************************************************************
      ************************ Block Memory Operations ***********************
      ***********************************************************************/

      // Copy the matrix contents to the destination
      void CopyTo(cv::Mat& Dst) const
         {
         copyTo(Dst);

         return;
         }

      void CopyTo(cv::Mat& Dst, const cv::Mat& Mask) const
         {
         copyTo(Dst, Mask);

         return;
         }

      // Clear the image to black
      void Clear()
         {
         memset(GetPixels(), 0, GetDataSize());

         return;
         }

      // Flip the layout of this matrix and store in the destination
      enum EFlipMode { eFlipX = 0, eFlipY = 1, eFlipXY = -1 };
      void Flip(cv::Mat& Dst, EFlipMode eMode)
         {
         cv::flip(*this, Dst, eMode);

         return;
         }

      /***********************************************************************
      ************************** Drawing Functions ***************************
      ***********************************************************************/

      void Line(cv::Point pt1, cv::Point pt2, const cv::Scalar& Color,
            int nThickness = 1, int nLineType = 8, int nShift = 0)
         {
         cv::line(*this, pt1, pt2, Color, nThickness, nLineType, nShift);

         return;
         }

      void Rectangle(cv::Point pt1, cv::Point pt2, const cv::Scalar& Color,
            int nThickness = 1, int nLineType = 8, int nShift = 0)
         {
         cv::rectangle(*this, pt1, pt2, Color, nThickness, nLineType, nShift);

         return;
         }

      void Rectangle(cv::Rect rect, const cv::Scalar& Color,
            int nThickness = 1, int nLineType = 8, int nShift = 0)
         {
         cv::rectangle(*this, rect, Color, nThickness, nLineType, nShift);

         return;
         }

      void RectangleFilled(cv::Point Pt1, cv::Point Pt2,
            const cv::Scalar& Color, int nLineType = 8, int nShift = 0)
         {
         Rectangle(Pt1, Pt2, Color, CV_FILLED, nLineType, nShift);

         return;
         }

      void Circle(cv::Point Center, int nRadius, const cv::Scalar& Color,
            int nThickness = 1, int nLineType = 8, int nShift = 0)
         {
         cv::circle(*this, Center, nRadius, Color, nThickness, nLineType, nShift);

         return;
         }

      void CircleFilled(cv::Point Center, int nRadius, const cv::Scalar& Color, int nLineType = 8, int nShift = 0)
         {
         Circle(Center, nRadius, Color, CV_FILLED, nLineType, nShift);

         return;
         }

      // Angles are in degrees
      void Ellipse(cv::Point Center, cv::Size Axes, double dAngle, double dStartAngle, double dEndAngle,
            const cv::Scalar& Color, int nThickness = 1, int nLineType = 8, int nShift = 0)
         {
         cv::ellipse(*this, Center, Axes, dAngle, dStartAngle, dEndAngle, Color, nThickness, nLineType, nShift);

         return;
         }

      void EllipseFilled(cv::Point Center, cv::Size Axes, double dAngle, double dStartAngle, double dEndAngle,
            const cv::Scalar& Color, int nLineType = 8, int nShift = 0)
         {
         Ellipse(Center, Axes, dAngle, dStartAngle, dEndAngle, Color, CV_FILLED, nLineType, nShift);

         return;
         }

      void PolygonsFilled(const cv::Point** ppPts, const int* pnPts, int nContours, const cv::Scalar& Color,
               int nLineType = 8, int nShift = 0, cv::Point Offset = cv::Point())
            {
            cv::fillPoly(*this, ppPts, pnPts, nContours, Color, nLineType, nShift, Offset);

            return;
            }

      void PolygonConvexFilled(const cv::Point* pPts, int nPts, const cv::Scalar& Color, int nLineType = 8,
               int nShift = 0)
            {
            cv::fillConvexPoly(*this, pPts, nPts, Color, nLineType, nShift);

            return;
            }

      void PolyLines(const cv::Point** ppPts, const int* pnPts, int nContours, bool bIsClosed, const cv::Scalar& Color,
               int nThickness, int nLineType = 8, int nShift = 0)
            {
            cv::polylines(*this, ppPts, pnPts, nContours, bIsClosed, Color, nThickness, nLineType, nShift);

            return;
            }

      // Origin is the lower left corner of the bounding box
      void PutText(const std::string& szText, cv::Point Origin, int nFontFace, double dFontScale,
            const cv::Scalar& Color, int nThickness = 1, int nLineType = 8, bool bBottomLeftOrigin = false)
         {
         cv::putText(*this, szText, Origin, nFontFace, dFontScale, Color, nThickness, nLineType, bBottomLeftOrigin);

         return;
         }
#if 0
      enum EInterpolation
         {
         eInterNearestNeighbor = cv::INTER_NN,
         eInterBilinear = cv::INTER_LINEAR,
         eInterArea = cv::INTER_AREA,
         eInterBicubic = cv::INTER_CUBIC,
         };

      /***********************************************************************
      ********************** Gaussian Pyramid Operations *********************
      ***********************************************************************/

       void PyramidUp(cv::Mat& Dst, const cv::Size& DstSize= cv::Size())
         {
         cv::pyrUp(*this, Dst, DstSize);

         return;
         }

      void PyramidDown(cv::Mat& Dst, const cv::Size& DstSize= cv::Size())
         {
         cv::pyrDown(*this, Dst, DstSize);

         return;
         }
#endif
      // Color order for color images
      enum EColor { eBlue, eGreen, eRed };

      bool Combine(const DCVImage* pImage1, const DCVImage* pImage2, ECombine eCombine = ECombine::eSideBySide,
            int nDivider = 0, cv::Scalar FillColor = CV_RGB(0, 0, 0));

   protected :
      // Pixel conversion functions
      // (8 bit unsigned is the most common display)
      static unsigned char S8ToU8(signed char nIn)
         {
         short int nTemp = static_cast<short int>(nIn) + 128;

         return (static_cast<unsigned char>(nTemp));
         }

      static unsigned char S16ToU8(signed short nIn)
         {
         int nTemp = static_cast<int>(nIn) + 32768;

         return (static_cast<unsigned char>(nTemp / 256));
         }

      static unsigned char U16ToU8(unsigned short nIn)
         {
         return (static_cast<unsigned char>(nIn / 256));
         }

   private :
   };  // End of class DCVImage


/*****************************************************************************
**************************** class DCVBinaryImage ****************************
*****************************************************************************/

/*
   Binary image class.  Basically add some useful functions for dealing with
   binary images to DCVImage.  No checking is done that the image is actually
   a binary image!!!
*/

class DCVBinaryImage : public DCVImage
   {
   public :
      enum EColor { eBlack, eWhite };

      DCVBinaryImage() : DCVImage()
         {
         return;
         }

      DCVBinaryImage(int nWidth, int nHeight)
            : DCVImage(nWidth, nHeight, CV_8UC1)
         {
         return;
         }

      DCVBinaryImage(const DCVBinaryImage& src) : DCVImage(src)
         {
         return;
         }

      ~DCVBinaryImage() = default;

      DCVBinaryImage& operator=(const DCVBinaryImage& rhs)
         {
         DCVImage::operator=(rhs);

         return (*this);
         }

      // Is the specified pixel black?
      bool IsBlack(int nRow, int nCol) const
         {
         return (*GetPixel(nRow, nCol) == eBlack);
         }

      // Is the specified pixel black?
      bool IsBlack(unsigned char nPixel) const
         {
         return (nPixel == eBlack);
         }

      // Is the specified pixel white?  Little latitude here
      bool IsWhite(int nRow, int nCol) const
         {
         return (*GetPixel(nRow, nCol) != eBlack);
         }

      // Is the specified pixel white?  Little latitude here
      bool IsWhite(unsigned char nPixel) const
         {
         return (nPixel != eBlack);
         }

      // Count pixel runs

      int CountBlackRight(int nRow, int nStartCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nRow, nStartCol);
         while ((nStartCol < GetWidth()) && IsBlack(*pPixel))
            {
            nStartCol++;
            pPixel++;
            nCount++;
            } // end while

         return (nCount);
         }

      int CountBlackLeft(int nRow, int nStartCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nRow, nStartCol);
         while ((nStartCol >= 0) && IsBlack(*pPixel))
            {
            nStartCol--;
            pPixel--;
            nCount++;
            } // end while

         return (nCount);
         }

      int CountWhiteRight(int nRow, int nStartCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nRow, nStartCol);
         while ((nStartCol < GetWidth()) && IsWhite(*pPixel))
            {
            nStartCol++;
            pPixel++;
            nCount++;
            } // end while

         return (nCount);
         }

      int CountWhiteLeft(int nRow, int nStartCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nRow, nStartCol);
         while ((nStartCol >= 0) && IsWhite(*pPixel))
            {
            nStartCol--;
            pPixel--;
            nCount++;
            } // end while

         return (nCount);
         }

      int CountBlackDown(int nStartRow, int nCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nStartRow, nCol);
         while ((nStartRow < GetHeight()) && IsBlack(*pPixel))
            {
            nStartRow++;
            pPixel += GetWidthStep();
            nCount++;
            } // end while

         return (nCount);
         }

      int CountBlackUp(int nStartRow, int nCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nStartRow, nCol);
         while ((nStartRow >= 0) && IsBlack(*pPixel))
            {
            nStartRow--;
            pPixel -= GetWidthStep();
            nCount++;
            } // end while

         return (nCount);
         }

      int CountWhiteDown(int nStartRow, int nCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nStartRow, nCol);
         while ((nStartRow < GetHeight()) && IsWhite(*pPixel))
            {
            nStartRow++;
            pPixel += GetWidthStep();
            nCount++;
            } // end while

         return (nCount);
         }

      int CountWhiteUp(int nStartRow, int nCol) const
         {
         int nCount = 0;
         const unsigned char* pPixel = GetPixel(nStartRow, nCol);
         while ((nStartRow >= 0) && IsWhite(*pPixel))
            {
            nStartRow--;
            pPixel -= GetWidthStep();
            nCount++;
            } // end while

         return (nCount);
         }

      /***********************************************************************
      *************************** class DPixelRun ****************************
      ***********************************************************************/

      /*
         Class to keep statistics on runs of pixels in a binary image
      */

      class DPixelRun
         {
         public :
            DPixelRun() : m_eColor(eBlack), m_nStart(0), m_nCount(0)
               {
               return;
               }

            DPixelRun(EColor eColor, int nStart, int nCount)
                  : m_eColor(eColor), m_nStart(nStart), m_nCount(nCount)
               {
               return;
               }

            DPixelRun(const DPixelRun& src) : m_eColor(src.m_eColor),
                  m_nStart(src.m_nStart), m_nCount(src.m_nCount)
               {
               return;
               }

            ~DPixelRun() = default;

            DPixelRun& operator=(const DPixelRun& rhs)
               {
               m_eColor = rhs.m_eColor;
               m_nStart = rhs.m_nStart;
               m_nCount = rhs.m_nCount;

               return (*this);
               }

            EColor GetColor() const
               {
               return (m_eColor);
               }

            int GetStart() const
               {
               return (m_nStart);
               }

            int GetCount() const
               {
               return (m_nCount);
               }

            bool IsBlack() const
               {
               return (m_eColor == eBlack);
               }

            bool IsWhite() const
               {
               return (m_eColor == eWhite);
               }

            protected :
               EColor m_eColor;
               int m_nStart;
               int m_nCount;

            private :

         };  // End of class DPixelRun

      using DPixelRunVector = std::vector<DPixelRun>;

   protected :

   private :

   };  // End of class DCVBinaryImage

/******************************************************************************
*
***  RemapRangeAndType
*
******************************************************************************/

template<typename SRCTYPE, typename DSTTYPE>
bool RemapRangeAndType(const cv::Mat& src, cv::Mat& dst, double dNewMin, double dNewMax)
   {
   bool bRet = true;

   cv::MatConstIterator_<SRCTYPE> sit = src.begin<SRCTYPE>();
   cv::MatIterator_<DSTTYPE> dit = dst.begin<DSTTYPE>();
   cv::MatConstIterator_<SRCTYPE> send = src.end<SRCTYPE>();

   double dMin, dMax;
   cv::minMaxIdx(src, &dMin, &dMax);
   double dOldRange = dMax - dMin;
   double dFac = (dOldRange != 0.0) ? ((dNewMax - dNewMin) / dOldRange) : 1.0;

   while (sit != send)
      {
      *dit = cv::saturate_cast<DSTTYPE>(((*sit - dMin) * dFac) + dNewMin);
      ++sit;
      ++dit;
      } // end while

   cv::minMaxIdx(dst, &dMin, &dMax);

   return (bRet);

   } // end of function RemapRangeAndType


#endif // __CVIMAGE_H__
