/*****************************************************************************
******************************** CVImage.cpp *********************************
*****************************************************************************/

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include "CVImage.h"

/*****************************************************************************
*********************** Class DCVImage Implementation ************************
*****************************************************************************/
#if 0
/*****************************************************************************
*
*  DCVImage::CopyHeader
*
*****************************************************************************/

void DCVImage::CopyHeader(const IplImage* pImage)
   {
   if (m_pImage != nullptr)
      {
      m_pImage->nSize = pImage->nSize;
      m_pImage->ID = pImage->ID;
      m_pImage->nChannels = pImage->nChannels;
      m_pImage->alphaChannel = pImage->alphaChannel;
      m_pImage->depth = pImage->depth;
      memcpy(m_pImage->colorModel, pImage->colorModel,
            sizeof(m_pImage->colorModel));
      memcpy(m_pImage->channelSeq, pImage->channelSeq,
            sizeof(m_pImage->channelSeq));
      m_pImage->dataOrder = pImage->dataOrder;
      m_pImage->origin = pImage->origin;
      m_pImage->align = pImage->align;
      m_pImage->width = pImage->width;
      m_pImage->height = pImage->height;
      m_pImage->roi = nullptr;
      m_pImage->maskROI = nullptr;
      m_pImage->imageId = nullptr;
      m_pImage->tileInfo = nullptr;
      m_pImage->imageSize = pImage->imageSize;
      m_pImage->widthStep = pImage->widthStep;
      memcpy(m_pImage->BorderMode, pImage->BorderMode,
            sizeof(m_pImage->BorderMode));
      memcpy(m_pImage->BorderConst, pImage->BorderConst,
            sizeof(m_pImage->BorderConst));
      } // end if
   
   return;

   } // End of function DCVImage::CopyHeader 
     
#endif
 

#if 0
/*****************************************************************************
*
*  DCVImage::Show
*
*****************************************************************************/

void DCVImage::Show(wxDC& DC, int x, int y, int /* w */, int /* h */,
      int /* FromX = 0 */, int /* FromY = 0 */)
   {
   if (!IsEmpty() && (GetDepth() == IPL_DEPTH_8U))
      {
      wxBitmap Bitmap(wxImage(GetWidth(), GetHeight(), GetPixels(), true));
      DC.DrawBitmap(Bitmap, x, y, false);
      } // end if

   return;

   } // End of function DCVImage::Show
#endif
/*****************************************************************************
*
*  DCVImage::Combine
*
*****************************************************************************/

bool DCVImage::Combine(const DCVImage* pImage1, const DCVImage* pImage2,
      DCVImage::ECombine eCombine /* = eSideBySide */, int nDivider /* = 0 */,
      cv::Scalar FillColor /* = CV_RGB(0, 0, 0) */)
   {
   bool bRet = true;
   
   // Size the image to fit both input images
   if (eCombine == ECombine::eSideBySide)
      {
      Create(pImage1->GetWidth() + pImage2->GetWidth() + nDivider,
            MAX(pImage1->GetHeight(), pImage2->GetHeight()),
            pImage1->GetType());
         } // end if
   else if (eCombine == ECombine::eStacked)
      {
      Create(MAX(pImage1->GetWidth(), pImage2->GetWidth()),
            pImage1->GetHeight() + pImage2->GetHeight() + nDivider,
            pImage1->GetType());
   } // end else if

   // Copy the pixels into the appropriate spots
   if (bRet)
      {
      // Copy image 1 on the left or top
      for (int r = 0 ; r < pImage1->GetHeight() ; r++)
         {
         unsigned char* pRow = GetRow(r);
         const unsigned char* pRow1 = pImage1->GetRow(r);
         memcpy(pRow, pRow1, pImage1->GetPixelSize() * pImage1->GetWidth());
         } // end for

      // Copy image 2 to the proper position
      if (eCombine == ECombine::eSideBySide)
         {
         // Copy image 2 on the right
         size_t nOffset = pImage1->GetPixelSize() * (pImage1->GetWidth() + nDivider);
         for (int r = 0 ; r < pImage2->GetHeight() ; r++)
            {
            unsigned char* pRow = GetRow(r) + nOffset;
            const unsigned char* pRow2 = pImage2->GetRow(r);
            memcpy(pRow, pRow2, pImage2->GetPixelSize() *
            pImage2->GetWidth());
            } // end for

         // Color the divider
         if (nDivider > 0)
            {
            RectangleFilled(cv::Point(pImage1->GetWidth(), 0),
                  cv::Point(pImage1->GetWidth() + nDivider - 1,
                  GetHeight() - 1), FillColor);
            } // end if
         } // end if
      else if (eCombine == ECombine::eStacked)
         {
         // Copy image 2 on the bottom
         for (int r = 0 ; r < pImage2->GetHeight() ; r++)
            {
            unsigned char* pRow = GetRow(r + pImage1->GetHeight() + nDivider);
            const unsigned char* pRow2 = pImage2->GetRow(r);
            memcpy(pRow, pRow2, pImage2->GetPixelSize() *
                  pImage2->GetWidth());
            } // end for

         // Color the divider
         if (nDivider > 0)
            {
            RectangleFilled(cv::Point(0, pImage1->GetHeight()),
                  cv::Point(GetWidth() - 1, pImage1->GetHeight() + nDivider),
                  FillColor);
            } // end if
         } // end else if
      } // end if
 
   return (bRet);

   } // End of function DCVImage::Combine 

#if 0
/*****************************************************************************
*
*  DCVImage::Combine
*
*****************************************************************************/

bool DCVImage::Combine(const DCVImage* pImage1, const DCVImage* pImage2,
      ECombine eCombine /* = eSideBySide */, int nDivider /* = 0 */,
      CvScalar FillColor /* = CV_RGB(0, 0, 0) */)
   {
   bool bRet = false;
   
   // Size the image to fit both input images
   if (eCombine == eSideBySide)
      {
      bRet = Create(pImage1->GetWidth() + pImage2->GetWidth() + nDivider,
            MAX(pImage1->GetHeight(), pImage2->GetHeight()),
            pImage1->GetDepth(), pImage1->GetNumChannels(),
            pImage1->GetOrigin());
      } // end if
   else if (eCombine == eStacked)
      {
      bRet = Create(MAX(pImage1->GetWidth(), pImage2->GetWidth()),
            pImage1->GetHeight() + pImage2->GetHeight() + nDivider,
            pImage1->GetDepth(), pImage1->GetNumChannels(),
            pImage1->GetOrigin());
      } // end else if

   // Copy the pixels into the appropriate spots
   if (bRet)
      {
      // Copy image 1 on the left or top
      for (int r = 0 ; r < pImage1->GetHeight() ; r++)
         {
         unsigned char* pRow = GetRow(r);
         const unsigned char* pRow1 = pImage1->GetRow(r);
         memcpy(pRow, pRow1,
               pImage1->GetPixelSize() * pImage1->GetWidth());
         } // end for

      // Copy image 2 to the proper position
      if (eCombine == eSideBySide)
         {
         // Copy image 2 on the right
         int nOffset = pImage1->GetPixelSize() * (pImage1->GetWidth() +
               nDivider);
         for (int r = 0 ; r < pImage2->GetHeight() ; r++)
            {
            unsigned char* pRow = GetRow(r) + nOffset;
            const unsigned char* pRow2 = pImage2->GetRow(r);
            memcpy(pRow, pRow2,
                  pImage2->GetPixelSize() * pImage2->GetWidth());
            } // end for

         // Color the divider
         if (nDivider > 0)
            {
            cvRectangle(m_pImage, cvPoint(pImage1->GetWidth(), 0),
                  cvPoint(pImage1->GetWidth() + nDivider - 1,
                  GetHeight() - 1), FillColor, CV_FILLED);   
            } // end if
         } // end if
      else if (eCombine == eStacked)
         {
         // Copy image 2 on the bottom
         for (int r = 0 ; r < pImage2->GetHeight() ; r++)
            {
            unsigned char* pRow = GetRow(r + pImage1->GetHeight() + nDivider);
            const unsigned char* pRow2 = pImage2->GetRow(r);
            memcpy(pRow, pRow2,
                  pImage2->GetPixelSize() * pImage2->GetWidth());
            } // end for

         // Color the divider
         if (nDivider > 0)
            {
            cvRectangle(m_pImage, cvPoint(0, pImage1->GetHeight()),
                  cvPoint(GetWidth() - 1, pImage1->GetHeight() + nDivider),
                  FillColor, CV_FILLED);   
            } // end if
         } // end else if
      } // end if   
      
   return (bRet);

   } // End of function DCVImage::Combine 
#endif
#if 0
/*****************************************************************************
*
*  DCVImage::CopyPixelsToRGB
*
*  Copy the pixels to the format RGB with 3 bytes per pixel and one byte
*  per channel.  Probably should make it more robust to other depths and
*  number of channels.
*
*****************************************************************************/

void DCVImage::CopyPixelsToRGB(unsigned char* pPixelsOut) const
   {
   for (int r = 0 ; r < GetHeight() ; r++)
      {
      const unsigned char* pRow = GetRow(r);
      
      for (int c = 0 ; c < GetWidth() ; c++)
         {
         const unsigned char* pPixel = GetPixel(pRow, c);
         
         if (GetNumChannels() == 1)
            {
            // Handle gray images
            if (GetPixelDepth() == 8)
               {
               if (IsPixelDataSigned())
                  {
                  // 8 bit signed (rescale 0 to 255)
                  short int nScaled = static_cast<short int>(*pPixel) + 128;
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] =
                        static_cast<unsigned char>(nScaled);   
                  } // end if
               else
                  {
                  // 8 bit unsigned
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] = *pPixel;
                  } // end else
               } // end if
            } // end if
         else
            {
            // 3 color images
            pPixelsOut[0] = pPixel[eRed];
            pPixelsOut[1] = pPixel[eGreen];
            pPixelsOut[2] = pPixel[eBlue];
            } // end else
         
         pPixelsOut += 3;
         } // end for
      } // end for
      
   return;

   } // End of function DCVImage::CopyPixelsToRGB 
#endif

/*****************************************************************************
*
*  DCVImage::CopyPixelsToRGB
*
*  Copy the pixels to the format RGB with 3 bytes per pixel and one byte
*  per channel.  Probably should make it more robust to other depths and
*  number of channels.
*
*****************************************************************************/

void DCVImage::CopyPixelsToRGB(unsigned char* pPixelsOut) const
   {
   if (GetNumChannels() == 1)
      {
      // Handle gray images
      if (GetPixelDepth() == 8)
         {
         // 8 bit pixels
         if (IsPixelDataSigned())
            {
            // 8 bit signed (rescale 0 to 255)
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const unsigned char* pPixel = GetPixel(pRow, c);
             
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] =
                        S8ToU8(*pPixel);
                  pPixelsOut += 3;   
                  } // end for column
               } // end for row
            } // end if
         else
            {
            // 8 bit unsigned
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const unsigned char* pPixel = GetPixel(pRow, c);
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] = *pPixel;
                  pPixelsOut += 3;
                  } // end for column
               } // end for row
            } // end else
         } // end if
      else if (GetPixelDepth() == 16)
         {
         if (IsPixelDataSigned())
            {
            // 16 bit signed (rescale 0 to 255)
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const short* pPixel =
                        reinterpret_cast<const short*>(GetPixel(pRow, c));
             
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] =
                        S16ToU8(*pPixel);
                  pPixelsOut += 3;   
                  } // end for column
               } // end for row
            
            } // end if
         else
            {
            // 16 bit unsigned
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const unsigned short* pPixel = reinterpret_cast<
                        const unsigned short*>(GetPixel(pRow, c));
                  pPixelsOut[0] = pPixelsOut[1] = pPixelsOut[2] =
                        U16ToU8(*pPixel);
                  pPixelsOut += 3;
                  } // end for column
               } // end for row
            
            } // end else
         } // end else if
      } // end if
   else if (GetNumChannels() == 3)
      {
      // 3 color images
      if (GetPixelDepth() == 8)
         {
         if (IsPixelDataSigned())
            {
            // 8 bit unsigned
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const unsigned char* pPixel = GetPixel(pRow, c);
                  pPixelsOut[0] = S8ToU8(pPixel[eRed]);
                  pPixelsOut[1] = S8ToU8(pPixel[eGreen]);
                  pPixelsOut[2] = S8ToU8(pPixel[eBlue]);
                  pPixelsOut += 3;
                  } // end for column
               } // end for row
            } // end if
         else
            {
            // 8 bit unsigned
            for (int r = 0 ; r < GetHeight() ; r++)
               {
               const unsigned char* pRow = GetRow(r);
      
               for (int c = 0 ; c < GetWidth() ; c++)
                  {
                  const unsigned char* pPixel = GetPixel(pRow, c);
                  pPixelsOut[0] = pPixel[eRed];
                  pPixelsOut[1] = pPixel[eGreen];
                  pPixelsOut[2] = pPixel[eBlue];
                  pPixelsOut += 3;
                  } // end for column
               } // end for row
            } // end else
         } // end if
      else if (GetPixelDepth() == 16)
         {
         
         } // end else if
      } // end else if
      
   return;

   } // End of function DCVImage::CopyPixelsToRGB 

