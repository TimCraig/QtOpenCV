/*
 * DQOpenCV.cpp
 *
 *  Created on: May 26, 2014
 *      Author: Tim
 */

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include "DQOpenCV.h"

/*****************************************************************************
*********************** Class DColorHSV Implementation ***********************
*****************************************************************************/

/*****************************************************************************
*
*  DColorHSV::SetFromRGB
*
*  Convert from a normalized [0,1] RGB color space to HSV.
*
*****************************************************************************/

void DColorHSV::SetFromRGB(double dRed /* = 0.0 */, double dGreen /* = 0.0 */,
      double dBlue /* = 0.0 */)
   {
   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   // Calulate the Hue
   if (dMax == dMin)
      {
      m_dHue = 0; // Undefined
      } // end if
   else if (dMax == dRed)
      {
      m_dHue = 60.0 * (dGreen - dBlue) / (dMax - dMin);
      if (m_dHue < 0.0)
         {
         m_dHue += 360.0;
         } // end if
      } // end else if
   else if (dMax == dGreen)
      {
      m_dHue = 60.0 * (dBlue - dRed) / (dMax - dMin) + 120.0;
      } // end else if
   else // (dMax == dBlue)
      {
      m_dHue = 60.0 * (dRed - dGreen) / (dMax - dMin) + 240.0;
      } // end else

   // Calculate the Saturation
   if (dMax == 0.0)
      {
      m_dSat = 0.0;
      } // end if
   else
      {
      m_dSat = (dMax - dMin) / dMax;
      } // end else

   // Calculate the Value
   m_dValue = dMax;

   return;

   } // End of function DColorHSV::SetFromRGB

/*****************************************************************************
*
*  DColorHSV::CalcHueFromRGB
*
*****************************************************************************/

double DColorHSV::CalcHueFromRGB(double dRed, double dGreen,
      double dBlue)
   {
   double dHue;

   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   // Calulate the Hue
   if (dMax == dMin)
      {
      dHue = 0; // Undefined
      } // end if
   else if (dMax == dRed)
      {
      dHue = 60.0 * (dGreen - dBlue) / (dMax - dMin);
      if (dHue < 0.0)
         {
         dHue += 360.0;
         } // end if
      } // end else if
   else if (dMax == dGreen)
      {
      dHue = 60.0 * (dBlue - dRed) / (dMax - dMin) + 120.0;
      } // end else if
   else // (dMax == dBlue)
      {
      dHue = 60.0 * (dRed - dGreen) / (dMax - dMin) + 240.0;
      } // end else

   return (dHue);

   } // End of function DColorHSV::CalcHueFromRGB

/*****************************************************************************
*
*  DColorHSV::CalcSatFromRGB
*
*****************************************************************************/

double DColorHSV::CalcSatFromRGB(double dRed, double dGreen,
      double dBlue)
   {
   double dSat;

   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   // Calculate the Saturation
   if (dMax == 0.0)
      {
      dSat = 0.0;
      } // end if
   else
      {
      dSat = (dMax - dMin) / dMax;
      } // end else

   return (dSat);

   } // End of function DColorHSV::CalcSatFromRGB

/*****************************************************************************
*********************** Class DColorHSL Implementation ***********************
*****************************************************************************/

/*****************************************************************************
*
*  DColorHSL::SetFromRGB
*
*  Convert from a normalized [0,1] RGB color space to HSL.
*
*****************************************************************************/

void DColorHSL::SetFromRGB(double dRed /* = 0.0 */, double dGreen /* = 0.0 */,
      double dBlue /* = 0.0 */)
   {
   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   // Calulate the Hue
   if (dMax == dMin)
      {
      m_dHue = 0; // Undefined
      } // end if
   else if (dMax == dRed)
      {
      m_dHue = 60.0 * (dGreen - dBlue) / (dMax - dMin);
      if (m_dHue < 0.0)
         {
         m_dHue += 360.0;
         } // end if
      } // end else if
   else if (dMax == dGreen)
      {
      m_dHue = 60.0 * (dBlue - dRed) / (dMax - dMin) + 120.0;
      } // end else if
   else // (dMax == dBlue)
      {
      m_dHue = 60.0 * (dRed - dGreen) / (dMax - dMin) + 240.0;
      } // end else

   // Calculate the Lightness
   m_dLight = (dMax + dMin) / 2.0;

   // Calculate the Saturation
   if (dMax == dMin)
      {
      m_dSat = 0;
      } // end if
   else if (m_dLight <= 0.5)
      {
      m_dSat = (dMax - dMin) / (dMax + dMin);
      } // end else if
   else
      {
      m_dSat = (dMax - dMin) / (2.0 - (dMax + dMin));
      } // end else

   return;

   } // End of function DColorHSL::SetFromRGB

/*****************************************************************************
*
*  DColorHSL::CalcHueFromRGB
*
*****************************************************************************/

double DColorHSL::CalcHueFromRGB(double dRed, double dGreen,
      double dBlue)
   {
   double dHue;

   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   // Calulate the Hue
   if (dMax == dMin)
      {
      dHue = 0; // Undefined
      } // end if
   else if (dMax == dRed)
      {
      dHue = 60.0 * (dGreen - dBlue) / (dMax - dMin);
      if (dHue < 0.0)
         {
         dHue += 360.0;
         } // end if
      } // end else if
   else if (dMax == dGreen)
      {
      dHue = 60.0 * (dBlue - dRed) / (dMax - dMin) + 120.0;
      } // end else if
   else // (dMax == dBlue)
      {
      dHue = 60.0 * (dRed - dGreen) / (dMax - dMin) + 240.0;
      } // end else

   return (dHue);

   } // End of function DColorHSL::CalcHueFromRGB

/*****************************************************************************
*
*  DColorHSL::CalcSatFromRGB
*
*****************************************************************************/

double DColorHSL::CalcSatFromRGB(double dRed, double dGreen,
      double dBlue)
   {
   double dSat;

   double dMax = std::max(dRed, std::max(dGreen, dBlue));
   double dMin = std::min(dRed, std::min(dGreen, dBlue));

   if (dMax == dMin)
      {
      dSat = 0;
      } // end if
   else if (((dMax + dMin) / 2.0) <= 0.5)
      {
      dSat = (dMax - dMin) / (dMax + dMin);
      } // end else if
   else
      {
      dSat = (dMax - dMin) / (2.0 - (dMax + dMin));
      } // end else

   return (dSat);

   } // End of function DColorHSL::CalcSatFromRGB

/*****************************************************************************
*********************** Class DColorYUV Implementation ***********************
*****************************************************************************/

/*****************************************************************************
*
*  DColorYUV::SetFromRGB
*
*  Convert from a normalized [0,1] RGB color space to YUV.
*
*****************************************************************************/

void DColorYUV::SetFromRGB(double dRed /* = 0.0 */, double dGreen /* = 0.0 */,
      double dBlue /* = 0.0 */)
   {
   m_dY = 0.298 * dRed + 0.587 * dGreen + 0.115 * dBlue;
   m_dU = -0.14713 * dRed - 0.28886 * dGreen + 0.436 * dBlue;
   m_dV = 0.615 * dRed - 0.51499 * dGreen - 0.10001 * dBlue;

   return;

   } // End of function DColorYUV::SetFromRGB

/*****************************************************************************
*********************** Class DColorYCbCr Implementation ***********************
*****************************************************************************/

/*****************************************************************************
*
*  DColorYCbCr::SetFromRGB
*
*  Convert from a normalized [0,1] RGB color space to YCbCr.
*
*****************************************************************************/

void DColorYCbCr::SetFromRGB(double dRed /* = 0.0 */, double dGreen /* = 0.0 */,
      double dBlue /* = 0.0 */)
   {
   m_dY = dKr * dRed + (1.0 - dKr - dKb) * dGreen + dKb * dBlue;
   m_dCb = 0.5 * (dBlue - m_dY) / (1.0 - dKb);
   m_dCr = 0.5 * (dRed - m_dY) / (1.0 - dKr);

   return;

   } // End of function DColorYCbCr::SetFromRGB


