/*
 * DQImage.cpp
 *
 *  Created on: Apr 6, 2014
 *      Author: Tim
 */

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DQImage.h"
#include <QVector>
#include <QColor>
#include <QDebug>

/*****************************************************************************
 ***  class DQImage
 *****************************************************************************/

/*****************************************************************************
 *
 ***  DQImage::RedChannel
 *
 * Return an 8 bit indexed image containing the red channel of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::RedChannel() const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(true, false, false);
   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, qRed(pixel(x, y)));
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::RedChannel

/*****************************************************************************
 *
 ***  DQImage::GreenChannel
 *
 * Return an 8 bit indexed image containing the green channel of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::GreenChannel() const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(false, true, false);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, qGreen(pixel(x, y)));
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::GreenChannel

/*****************************************************************************
 *
 ***  DQImage::BlueChannel
 *
 * Return an 8 bit indexed image containing the blue channel of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::BlueChannel() const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(false, false, true);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, qBlue(pixel(x, y)));
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::BlueChannel

/*****************************************************************************
 *
 ***  DQImage::HSVHue8Channel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSV Hue of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::HSVHue8Channel(bool bRed /* = true */, bool bGreen /* = false */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         int nHue8 = (QColor(pixel(x, y)).hsvHue() * 255) / 359;
         Image.setPixel(x, y, nHue8);
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSVHue8Channel

/*****************************************************************************
 *
 ***  DQImage::HSVSaturationChannel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSV Saturation of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::HSVSaturationChannel(bool bRed /* = false */, bool bGreen /* = true */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, QColor(pixel(x, y)).hsvSaturation());
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSVSaturationChannel

/*****************************************************************************
 *
 ***  DQImage::HSVValueChannel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSV Value of 'this' image.  This is NOT the same as GrayChannel().
 *
 *****************************************************************************/

DQImage DQImage::HSVValueChannel(bool bRed /* = true */, bool bGreen /* = true */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, QColor(pixel(x, y)).value());
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSVValueChannel

/*****************************************************************************
 *
 ***  DQImage::HSLHue8Channel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSL Hue of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::HSLHue8Channel(bool bRed /* = true */, bool bGreen /* = false */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         int nHue8 = (QColor(pixel(x, y)).hslHue() * 255) / 359;
         Image.setPixel(x, y, nHue8);
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSLHue8Channel

/*****************************************************************************
 *
 ***  DQImage::HSLSaturationChannel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSL Saturation of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::HSLSaturationChannel(bool bRed /* = false */, bool bGreen /* = true */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, QColor(pixel(x, y)).hslSaturation());
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSLSaturationChannel

/*****************************************************************************
 *
 ***  DQImage::HSLLightnessChannel
 *
 * Return an 8 bit indexed image containing a false color representation for
 * the HSL Lightness of 'this' image.  This is NOT the same as GrayChannel().
 *
 *****************************************************************************/

DQImage DQImage::HSLLightnessChannel(bool bRed /* = true */, bool bGreen /* = true */, bool bBlue /* = true */) const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(bRed, bGreen, bBlue);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         Image.setPixel(x, y, QColor(pixel(x, y)).lightness());
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::HSLLightnessChannel

/*****************************************************************************
 *
 ***  DQImage::Gray
 *
 * Return an 8 bit indexed image containing the blue channel of 'this' image.
 *
 *****************************************************************************/

DQImage DQImage::Gray() const
   {
   DQImage Image(size(), QImage::Format_Indexed8);
   Image.SetROI(GetROI());

   // Construct the color table
   Image.SetLinearColorTable(true, true, true);

   for (int y = 0 ; y < height() ; y++)
      {
      for (int x = 0 ; x < width() ; x++)
         {
         QRgb Pixel = pixel(x, y);
         Image.setPixel(x, y,
               static_cast<unsigned char>(0.115 * qRed(Pixel) +
                     0.587 * qBlue(Pixel) + 0.298 * qGreen(Pixel)));
         } // end for
      } // end for

   return (Image);

   } // end of method DQImage::Gray

/*****************************************************************************
 *
 ***  DQImage::SetLinearColorTable
 *
 *  Set up a linear gradient for the color table of an 8 bit indexed image.
 *  Function fails if applied to an image with another format.
 *
 *****************************************************************************/

bool DQImage::SetLinearColorTable(bool bRed, bool bGreen, bool bBlue)
   {
   bool bRet = (format() == QImage::Format_Indexed8);
   if (bRet)
      {
      QVector<QRgb> ColorTable;
      int r = 0;
      int g = 0;
      int b = 0;
      for (int i = 0 ; i < 256 ; i++)
         {
         ColorTable.push_back(qRgb(r, g, b));
         if (bRed)
            {
            r++;
            } // end if

         if (bGreen)
            {
            g++;
            } // end if

         if (bBlue)
            {
            b++;
            } // end if
         } // end for

      setColorTable(ColorTable);
      } // end if

   return (bRet);

   } // end of method DQImage::SetLinearColorTable

/*****************************************************************************
 *
 ***  DQImage::CalcHistogram
 *
 *****************************************************************************/

bool DQImage::CalcHistogram(DHistogram& Histogram,  bool bUseROI /* = true */) const
   {
   bool bRet = true;

   Histogram.Clear();

   QRect r = (bUseROI && (m_ROI != QRect())) ? m_ROI : rect();

   if (format() == QImage::Format_Indexed8)
      {
      for (int y = r.top() ; y <= r.bottom() ; y++)
         {
         for (int x = r.left() ; x <= r.right() ; x++)
            {
            Histogram.IncValue(pixelIndex(x, y));
            } // end for
         } // end for
      } // end if
   else
      {
      bRet = false;
      } // end else

   return (bRet);

   } // end of method DQImage::CalcHistogram

