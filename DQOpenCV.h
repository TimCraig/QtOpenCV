/*
 * DQOpenCV.h
 *
 *  Created on: May 26, 2014
 *      Author: Tim
 */

#ifndef DQOPENCV_H_
#define DQOPENCV_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <opencv2/core.hpp>
#include <QRect>
#include <QColor>

#include <algorithm>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

//#include "DConfig.h"

#include "CVImage.h"

/*****************************************************************************
 *******************************  D E F I N E  ********************************
 *****************************************************************************/

/*****************************************************************************
 ******************************* class DCVRect ********************************
 *****************************************************************************/

/*****************************************************************************
 *
 ***  class DCVRect
 *
 * cv::Rect is based on the top left corner coordinates and width and height.
 * This class provides some interoperability with QRect.  Since instances of
 * this class are expected to be used as direct replacements for cv::Rect,
 * don't add any virtual members which will cause a vTable to be added.
 *
 *****************************************************************************/

class DCVRect : public cv::Rect
   {
   public:
      DCVRect() : cv::Rect()
         {
         return;
         }

      DCVRect(int nX, int nY, int nWidth, int nHeight) :
            cv::Rect(nX, nY, nWidth, nHeight)
         {
         return;
         }

      DCVRect(const QRect& src) :
            cv::Rect(src.x(), src.y(), src.width(), src.height())
         {
         return;
         }

      // Initialize from a QRect
      DCVRect(const QRect* pSrc)
         {
         x = pSrc->x();
         y = pSrc->y();
         width = pSrc->width();
         height = pSrc->height();
         return;
         }

      DCVRect(const DCVRect& src) :
            cv::Rect(src)
         {
         return;
         }

      ~DCVRect() = default;

      DCVRect& operator=(const DCVRect& rhs)
         {
         if (this != &rhs)
            {
            cv::Rect::operator=(rhs);
            } // end if

         return (*this);
         }

      DCVRect& operator=(const QRect& rhs)
         {
         x = rhs.x();
         y = rhs.y();
         width = rhs.width();
         height = rhs.height();

         return (*this);
         }

      operator QRect() const
         {
         QRect rect(x, y, width, height);

         return (rect);
         }

      // Move the left edge keeping the right edge fixed
      void AdjustXLeft(int nNewX)
         {
         width += (x - nNewX);
         x = nNewX;

         return;
         }

      // Move the right edge keeping the left edge fixed
      void AdjustXRight(int nNewX)
         {
         width = nNewX - x;

         return;
         }

      // Move the top edge of keeping the bottom fixed
      void AdjustYTop(int nNewY)
         {
         height += (y - nNewY);
         y = nNewY;

         return;
         }

      // Move the bottom edge of keeping the top fixed
      void AdjustYBottom(int nNewY)
         {
         height = nNewY - y;

         return;
         }

      // Move the left edge keeping the right edge fixed
      // only if width increases
      void InflateXLeft(int nNewX)
         {
         if (nNewX < x)
            {
            width += (x - nNewX);
            x = nNewX;
            } // end if

         return;
         }

      // Move the right edge keeping the left edge fixed
      // only if width increases
      void InflateXRight(int nNewX)
         {
         int nNewWidth = nNewX - x;
         if (nNewWidth > width)
            {
            width = nNewWidth;
            } // end if

         return;
         }

      // Move the top edge of keeping the bottom fixed
      // only if height increases
      void InflateYTop(int nNewY)
         {
         if (nNewY < y)
            {
            height += (y - nNewY);
            y = nNewY;
            } // end if

         return;
         }

      // Move the bottom edge of keeping the top fixed
      // only if height increases
      void InflateYBottom(int nNewY)
         {
         int nNewHeight = nNewY - y;
         if (nNewHeight > height)
            {
            height = nNewHeight;
            } // end if

         return;
         }

      // Move the left edge keeping the right edge fixed
      // only if width decreases
      void DeflateXLeft(int nNewX)
         {
         if (nNewX > x)
            {
            width += (x - nNewX);
            x = nNewX;
            } // end if

         return;
         }

      // Move the right edge keeping the left edge fixed
      // only if width decreases
      void DeflateXRight(int nNewX)
         {
         int nNewWidth = nNewX - x;
         if (nNewWidth < width)
            {
            width = nNewWidth;
            } // end if

         return;
         }

      // Move the top edge of keeping the bottom fixed
      // only if height decreases
      void DeflateYTop(int nNewY)
         {
         if (nNewY > y)
            {
            height += (y - nNewY);
            y = nNewY;
            } // end if
         return;
         }

      // Move the bottom edge of keeping the top fixed
      // only if height decreases
      void DeflateYBottom(int nNewY)
         {
         int nNewHeight = nNewY - y;
         if (nNewHeight < height)
            {
            height = nNewHeight;
            } // end if

         return;
         }

   protected:

   private:

   }; // end of class DCVRect

/*****************************************************************************
 *
 ***  class DCVSize
 *
 * This class provides a bit of interoperability between cv::Size and Qt's QSize.
 *
 *****************************************************************************/

class DCVSize : public cv::Size
   {
   public:
      DCVSize() : cv::Size()
         {
         return;
         }

      DCVSize(int nWidth, int nHeight) : cv::Size(nWidth, nHeight)
         {
         return;
         }

      // Initialize from a QSize
      DCVSize(const QSize& src) : cv::Size(src.width(), src.height())
         {
         return;
         }

      DCVSize(const QSize* pSrc) : cv::Size(pSrc->width(), pSrc->height())
         {
         return;
         }

      DCVSize(const DCVSize& src) : cv::Size(src)
         {
         return;
         }

      ~DCVSize() = default;

      DCVSize& operator=(const DCVSize& rhs)
         {
         cv::Size::operator=(rhs);
         return (*this);
         }

      DCVSize& operator=(const QSize& rhs)
         {
         width = rhs.width();
         height = rhs.height();

         return (*this);
         }

      operator QSize() const
         {
         QSize sz(width, height);

         return (sz);
         }

      void Set(int nWidth, int nHeight)
         {
         width = nWidth;
         height = nHeight;

         return;
         }

   protected:

   private:

   }; // end of class DCVSize

/*****************************************************************************
 *
 ***  class DCVPoint
 *
 * This class essentially provides a bit of interoperability between
 * cv::Point and Qt's QPoint.  Since instances of this class are expected to
 * be used as direct replacements for cv::Point, don't add any virtual
 * members which will cause a vTable to be added.
 *
 *****************************************************************************/

class DCVPoint : public cv::Point
   {
   public:
      DCVPoint() : cv::Point()
         {
         return;
         }

      DCVPoint(int nX, int nY) : cv::Point(nX, nY)
         {
         return;
         }

      // Initialize from a QPOint
      DCVPoint(const QPoint& src)
         {
         x = src.x();
         y = src.y();

         return;
         }

      DCVPoint(const QPoint* pSrc)
         {
         x = pSrc->x();
         y = pSrc->y();
         return;
         }

      DCVPoint(const DCVPoint& src) :
            cv::Point(src)
         {
         return;
         }

      ~DCVPoint() = default;

      DCVPoint& operator=(const DCVPoint& rhs)
         {
         cv::Point::operator=(rhs);

         return (*this);
         }

      DCVPoint& operator=(const QPoint& rhs)
         {
         x = rhs.x();
         y = rhs.y();

         return (*this);
         }

      operator QPoint() const
         {
         QPoint pt(x, y);

         return (pt);
         }

      int GetX() const
         {
         return (x);
         }

      int GetY() const
         {
         return (y);
         }

      void SetX(int v)
         {
         x = v;
         return;
         }

      void SetY(int v)
         {
         y = v;
         return;
         }

// For some reason the compiler is crapping on this function!!!!
      int Distance(const DCVPoint& Pt) const
         {
         return (DCVPoint::Distance(*this, Pt));
         }

      static int Distance(const DCVPoint& P1, const DCVPoint& P2)
         {
         int dx = P1.x - P2.x;
         int dy = P1.y - P2.y;

         return (static_cast<int>(sqrt(static_cast<double>((dx * dx) + (dy * dy)))));
         }

   protected:

   private:

   }; // end of class DCVPoint

/*****************************************************************************
 *
 ***  class DCVScalar
 *
 * This class extends cv::Scalar to provide support for Boost::Serialization.
 *
 *****************************************************************************/

class DCVScalar : public cv::Scalar
   {
   public:
      static const unsigned int m_nVersion = 0;

      DCVScalar() : cv::Scalar()
         {
         return;
         }

      DCVScalar(double v0, double v1, double v2 = 0, double v3 = 0)
            : cv::Scalar(v0, v1, v2, v3)
         {
         return;
         }

      DCVScalar(const DCVScalar& src) : cv::Scalar(src)
         {
         return;
         }

      DCVScalar(const cv::Scalar& src) : cv::Scalar(src)
         {
         return;
         }

      ~DCVScalar() = default;

      DCVScalar& operator=(const DCVScalar& rhs)
         {
         cv::Scalar::operator=(rhs);

         return (*this);
         }

      DCVScalar& operator=(const cv::Scalar& rhs)
         {
         cv::Scalar::operator=(rhs);

         return (*this);
         }

      operator QColor() const
         {
         // QColor has opacity and the default is full opaque at 255.
         QColor color(static_cast<int>(val[2]), static_cast<int>(val[1]), static_cast<int>(val[0]),
               static_cast<int>(255.0 - val[3]));

         return (color);

         }

      template<class Archive>
      void serialize(Archive &ar, const unsigned int /* nVersion */)
         {
         ar & boost::serialization::make_nvp("Red", val[2]);
         ar & boost::serialization::make_nvp("Green", val[1]);
         ar & boost::serialization::make_nvp("Blue", val[0]);
         ar & boost::serialization::make_nvp("Alpha", val[3]);

         return;
         }

   protected:

   private:

   }; // end of class DCVScalar

BOOST_CLASS_VERSION(DCVScalar, DCVScalar::m_nVersion)

/*****************************************************************************
 *
 ***  class DCVRGBA
 *
 * Drop in extension of cv::Scalar for RGBA colors.  Adds no data and is
 * passable everywhere that takes a cv::Scalar.
 *
 *****************************************************************************/

class DCVRGBA : public cv::Scalar
   {
   public:
      static const unsigned int m_nVersion = 0;

      DCVRGBA() : cv::Scalar()
         {
         return;
         }

      DCVRGBA(uchar uRed, uchar uGreen, uchar uBlue, uchar uAlpha = 255) :
            cv::Scalar(uBlue, uGreen, uRed, uAlpha)
         {
         return;
         }

      DCVRGBA(QColor Color) : cv::Scalar(Color.blue(), Color.green(), Color.red(), Color.alpha())
         {
         return;
         }

      DCVRGBA(const DCVRGBA& src) : cv::Scalar(src)
         {
         return;
         }

      ~DCVRGBA() = default;

      DCVRGBA& operator=(const DCVRGBA& rhs)
         {
         cv::Scalar::operator=(rhs);
         return (*this);
         }

      uchar Red() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](2)));
         }

      void SetRed(uchar uRed)
         {
         cv::Scalar::operator[](2) = uRed;
         return;
         }

      uchar Green() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](1)));
         }

      void SetGreen(uchar uGreen)
         {
         cv::Scalar::operator[](1) = uGreen;
         return;
         }

      uchar Blue() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](0)));
         }

      void SetBlue(uchar uBlue)
         {
         cv::Scalar::operator[](0) = uBlue;
         return;
         }

      uchar Alpha() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](0)));
         }

      void SetAlpha(uchar uAlpha)
         {
         cv::Scalar::operator[](0) = uAlpha;
         return;
         }

      operator QColor() const
         {
         // QColor has opacity and the default is full opaque at 255.
         QColor color(static_cast<int>(val[2]), static_cast<int>(val[1]), static_cast<int>(val[0]),
               static_cast<int>(val[3]));

         return (color);

         }

      template<class Archive>
      void serialize(Archive &ar, const unsigned int /* nVersion */)
         {
         ar & boost::serialization::make_nvp("Red", val[2]);
         ar & boost::serialization::make_nvp("Green", val[1]);
         ar & boost::serialization::make_nvp("Blue", val[0]);
         ar & boost::serialization::make_nvp("Alpha", val[3]);

         return;
         }

   protected:

   private:

   }; // end of class DCVRGBA

BOOST_CLASS_VERSION(DCVRGBA, DCVRGBA::m_nVersion)

/*****************************************************************************
 *
 ***  class DCVRGB
 *
 * Drop in extension of cv::Scalar for RGB colors.  Adds no data and is
 * passable everywhere that takes a cv::Scalar.
 *
 *****************************************************************************/

class DCVRGB : public DCVRGBA
   {
   public:
      static const unsigned int m_nVersion = 0;

      DCVRGB() = default;


      DCVRGB(uchar uRed, uchar uGreen, uchar uBlue) :
            DCVRGBA(uBlue, uGreen, uRed, 255.0)
         {
         return;
         }

      DCVRGB(QColor Color) : DCVRGBA(Color)
         {
         return;
         }

      DCVRGB(const DCVRGB& src) : DCVRGBA(src)
         {
         return;
         }

      ~DCVRGB() = default;

      DCVRGB& operator=(const DCVRGB& rhs)
         {
         cv::Scalar::operator=(rhs);
         return (*this);
         }

      uchar Red() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](2)));
         }

      void SetRed(uchar uRed)
         {
         cv::Scalar::operator[](2) = uRed;
         return;
         }

      uchar Green() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](1)));
         }

      void SetGreen(uchar uGreen)
         {
         cv::Scalar::operator[](1) = uGreen;
         return;
         }

      uchar Blue() const
         {
         return (static_cast<uchar>(cv::Scalar::operator[](0)));
         }

      void SetBlue(uchar uBlue)
         {
         cv::Scalar::operator[](0) = uBlue;
         return;
         }

      operator QColor() const
         {
         // QColor has opacity and the default is full opaque at 255.
         QColor color(static_cast<int>(val[2]), static_cast<int>(val[1]), static_cast<int>(val[0]),
               static_cast<int>(val[3]));

         return (color);

         }

      template<class Archive>
      void serialize(Archive &ar, const unsigned int /* nVersion */)
         {
         ar & boost::serialization::make_nvp( "DCVRGBA",
               boost::serialization::base_object<DCVRGBA>(*this));

         return;
         }

   protected:

   private:
   }; // end of class DCVRGB

BOOST_CLASS_VERSION(DCVRGB, DCVRGB::m_nVersion)

/*****************************************************************************
 *
 ***  class DColorHSV
 *
 * A class for simplified handling of colors by Hue, Saturation, and Value (HSV)
 *
 *****************************************************************************/

class DColorHSV
   {
   public:
      DColorHSV(double dHue = 0.0, double dSat = 0.0, double dValue = 0.0)
         {
         Set(dHue, dSat, dValue);
         }

      DColorHSV(unsigned char nRed, unsigned char nGreen,
            unsigned char nBlue)
         {
         SetFromRGB(nRed, nGreen, nBlue);
         return;
         }

      DColorHSV(const DColorHSV& src)
         {
         m_dHue = src.m_dHue;
         m_dSat = src.m_dSat;
         m_dValue = src.m_dValue;

         return;
         }

      ~DColorHSV() = default;

      DColorHSV& operator=(const DColorHSV& rhs)
         {
         if (this != &rhs)
            {
            m_dHue = rhs.m_dHue;
            m_dSat = rhs.m_dSat;
            m_dValue = rhs.m_dValue;
            } // end if

         return (*this);
         }

      void Set(double dHue = 0.0, double dSat = 0.0, double dValue = 0.0)
         {
         m_dHue = dHue;
         m_dSat = dSat;
         m_dValue = dValue;
         return;
         }

      void SetFromRGB(double dRed = 0.0, double dGreen = 0.0,
            double dBlue = 0.0);
      void SetFromRGB(unsigned char nRed, unsigned char nGreen,
            unsigned char nBlue)
         {
         SetFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0);
         return;
         }

      static unsigned char ScaleHueTo255(double dHue)
         {
         return (static_cast<unsigned char>(dHue / 360.0 * 255));
         }

      static unsigned char ScaleSatTo255(double dSat)
         {
         return (static_cast<unsigned char>(dSat * 255));
         }

      static unsigned char ScaleValueTo255(double dValue)
         {
         return (static_cast<unsigned char>(dValue * 255));
         }

      void ScaleTo255(unsigned char& nC1, unsigned char& nC2,
            unsigned char& nC3)
         {
         nC1 = ScaleHueTo255(m_dHue);
         nC2 = ScaleSatTo255(m_dSat);
         nC3 = ScaleValueTo255(m_dValue);
         return;
         }

      void ScaleTo255(unsigned char Pixel[3])
         {
         Pixel[0] = ScaleHueTo255(m_dHue);
         Pixel[1] = ScaleSatTo255(m_dSat);
         Pixel[2] = ScaleValueTo255(m_dValue);
         return;
         }

      static double CalcHueFromRGB(double dRed, double dGreen, double dBlue);
      static double CalcHueFromRGB(unsigned char nRed, unsigned char nGreen,
            unsigned char nBlue)
         {
         return (CalcHueFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0));
         }

      static double CalcSatFromRGB(double dRed, double dGreen, double dBlue);
      static double CalcSatFromRGB(unsigned char nRed, unsigned char nGreen,
            unsigned char nBlue)
         {
         return (CalcSatFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0));
         }

      static double CalcValueFromRGB(double dRed, double dGreen, double dBlue)
         {
         return (std::max(dRed, std::max(dGreen, dBlue)));
         }

      static double CalcValueFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         return (CalcValueFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0));
         }

      double m_dHue;
      double m_dSat;
      double m_dValue;

   protected:

   private:

   }; // end of class DColorHSV

/*****************************************************************************
 *
 ***  class DColorHSL
 *
 * A class for simplified handling of colors by Hue, Saturation, and
 * Lightness (HSL)
 *
 *****************************************************************************/

class DColorHSL
   {
   public:
      DColorHSL(double dHue = 0.0, double dSat = 0.0, double dLight = 0.0)
         {
         Set(dHue, dSat, dLight);
         return;
         }

      DColorHSL(unsigned char nRed, unsigned char nGreen,
            unsigned char nBlue)
         {
         SetFromRGB(nRed, nGreen, nBlue);
         return;
         }

      DColorHSL(const DColorHSL& src)
         {
         m_dHue = src.m_dHue;
         m_dSat = src.m_dSat;
         m_dLight = src.m_dLight;

         return;
         }

      ~DColorHSL() = default;

      DColorHSL& operator=(const DColorHSL& rhs)
         {
         if (this != &rhs)
            {
            m_dHue = rhs.m_dHue;
            m_dSat = rhs.m_dSat;
            m_dLight = rhs.m_dLight;
            } // end if

         return (*this);
         }

      void Set(double dHue = 0.0, double dSat = 0.0, double dLight = 0.0)
         {
         m_dHue = dHue;
         m_dSat = dSat;
         m_dLight = dLight;

         return;
         }

      void SetFromRGB(double dRed = 0.0, double dGreen = 0.0, double dBlue = 0.0);

      void SetFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         SetFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0);

         return;
         }

      static unsigned char ScaleHueTo255(double dHue)
         {
         return (static_cast<unsigned char>(dHue / 360.0 * 255));
         }

      static unsigned char ScaleSatTo255(double dSat)
         {
         return (static_cast<unsigned char>(dSat * 255));
         }

      static unsigned char ScaleValueTo255(double dValue)
         {
         return (static_cast<unsigned char>(dValue * 255));
         }

      void ScaleTo255(unsigned char& nC1, unsigned char& nC2, unsigned char& nC3)
         {
         nC1 = ScaleHueTo255(m_dHue);
         nC2 = ScaleSatTo255(m_dSat);
         nC3 = ScaleValueTo255(m_dLight);

         return;
         }

      void ScaleTo255(unsigned char Pixel[3])
         {
         Pixel[0] = ScaleHueTo255(m_dHue);
         Pixel[1] = ScaleSatTo255(m_dSat);
         Pixel[2] = ScaleValueTo255(m_dLight);

         return;
         }

      static double CalcHueFromRGB(double dRed, double dGreen, double dBlue);
      static double CalcHueFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         return (CalcHueFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0));
         }

      static double CalcSatFromRGB(double dRed, double dGreen, double dBlue);
      static double CalcSatFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         return (CalcSatFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0));
         }

      static double CalcLightFromRGB(double dRed, double dGreen, double dBlue)
         {
         double dMax = std::max(dRed, std::max(dGreen, dBlue));
         double dMin = std::min(dRed, std::min(dGreen, dBlue));

         return ((dMax + dMin) / 2.0);
         }

      static double CalcLightFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         return (CalcLightFromRGB(nRed / 255.0, nGreen / 255.0,  nBlue / 255.0));
         }

      double m_dHue;
      double m_dSat;
      double m_dLight;

   protected:

   private:

   }; // end of class DColorHSL

/*****************************************************************************
 *
 ***  class DColorYUV
 *
 *****************************************************************************/

class DColorYUV
   {
   public:
      DColorYUV(double dY = 0.0, double dU = 0.0, double dV = 0.0)
         {
         Set(dY, dU, dV);

         return;
         }

      DColorYUV(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         SetFromRGB(nRed, nGreen, nBlue);

         return;
         }

      DColorYUV(const DColorYUV& src)
         {
         m_dY = src.m_dY;
         m_dU = src.m_dU;
         m_dV = src.m_dV;

         return;
         }

      ~DColorYUV() = default;

      DColorYUV& operator=(const DColorYUV& rhs)
         {
         if (this != &rhs)
            {
            m_dY = rhs.m_dY;
            m_dU = rhs.m_dU;
            m_dV = rhs.m_dV;
            } // end if

         return (*this);
         }

      void Set(double dY = 0.0, double dU = 0.0, double dV = 0.0)
         {
         m_dY = dY;
         m_dU = dU;
         m_dV = dV;

         return;
         }

      void SetFromRGB(double dRed = 0.0, double dGreen = 0.0, double dBlue = 0.0);
      void SetFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         SetFromRGB(nRed / 255.0, nGreen / 255.0, nBlue / 255.0);

         return;
         }

      void ScaleTo255(unsigned char& nC1, unsigned char& nC2, unsigned char& nC3)
         {
         nC1 = static_cast<unsigned char>(m_dY * 255);
         nC2 = static_cast<unsigned char>((m_dU + 0.5) * 255);
         nC3 = static_cast<unsigned char>((m_dV + 0.5) * 255);

         return;
         }

      void ScaleTo255(unsigned char Pixel[3])
         {
         ScaleTo255(Pixel[0], Pixel[1], Pixel[2]);

         return;
         }

      double m_dY;
      double m_dU;
      double m_dV;

   protected:

   private:

   }; // end of class DColorYUV

/*****************************************************************************
 *
 ***  class DColorYCbCr
 *
 *****************************************************************************/

class DColorYCbCr
   {
   public:
      DColorYCbCr(double dY = 0.0, double dCb = 0.0, double dCr = 0.0)
         {
         Set(dY, dCb, dCr);

         return;
         }

      DColorYCbCr(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         SetFromRGB(nRed, nGreen, nBlue);

         return;
         }

      DColorYCbCr(const DColorYCbCr& src)
         {
         m_dY = src.m_dY;
         m_dCb = src.m_dCb;
         m_dCr = src.m_dCr;

         return;
         }

      ~DColorYCbCr() = default;

      DColorYCbCr& operator=(const DColorYCbCr& rhs)
         {
         if (this != &rhs)
            {
            m_dY = rhs.m_dY;
            m_dCb = rhs.m_dCb;
            m_dCr = rhs.m_dCr;
            } // end if

         return (*this);
         }

      void Set(double dY = 0.0, double dCb = 0.0, double dCr = 0.0)
         {
         m_dY = dY;
         m_dCb = dCb;
         m_dCr = dCr;

         return;
         }

      void SetFromRGB(double dRed = 0.0, double dGreen = 0.0, double dBlue = 0.0);
      void SetFromRGB(unsigned char nRed, unsigned char nGreen, unsigned char nBlue)
         {
         SetFromRGB(nRed / 255.0, nBlue / 255.0, nGreen / 255.0);

         return;
         }

      void ScaleTo255(unsigned char& nC1, unsigned char& nC2, unsigned char& nC3)
         {
         nC1 = static_cast<unsigned char>(m_dY * 255);
         nC2 = static_cast<unsigned char>((m_dCb + 0.5) * 255);
         nC3 = static_cast<unsigned char>((m_dCr + 0.5) * 255);

         return;
         }

      void ScaleTo255(unsigned char Pixel[3])
         {
         ScaleTo255(Pixel[0], Pixel[1], Pixel[2]);

         return;
         }

      double m_dY;
      double m_dCb;
      double m_dCr;

   protected:
      static constexpr double dKr{0.0722};
      static constexpr double dKb{0.2126};

   private:

   }; // end of class DColorYCbCr

/*****************************************************************************
 *
 ***  class DCrossHairs
 *
 * Class to draw a crosshair on an OpenCV image.
 *
 *****************************************************************************/

class DCrossHairs
   {
   public:
      DCrossHairs() :
            m_nDiameter(20), m_Color(CV_RGB(0, 0, 0)), m_nThickness(1), m_nLineType(8)
         {
         return;
         }

      DCrossHairs(int nDiameter, cv::Scalar Color, int nThickness = 1, int nLineType = 8) :
            m_nDiameter(nDiameter), m_Color(Color), m_nThickness(nThickness), m_nLineType(nLineType)
         {
         return;
         }

      ~DCrossHairs() = default;

      DCrossHairs(const DCrossHairs& src)
         {
         m_nDiameter = src.m_nDiameter;
         m_Color = src.m_Color;
         m_nThickness = src.m_nThickness;
         m_nLineType = src.m_nLineType;

         return;
         }

      DCrossHairs& operator=(const DCrossHairs& rhs)
         {
         if (this != &rhs)
            {
            m_nDiameter = rhs.m_nDiameter;
            m_Color = rhs.m_Color;
            m_nThickness = rhs.m_nThickness;
            m_nLineType = rhs.m_nLineType;
            } // end if

         return (*this);
         }

      int GetDiameter() const
         {
         return (m_nDiameter);
         }

      void SetDiameter(int nDiameter)
         {
         m_nDiameter = nDiameter;

         return;
         }

      cv::Scalar GetColor() const
         {
         return (m_Color);
         }

      void SetColor(cv::Scalar Color)
         {
         m_Color = Color;

         return;
         }

      int GetThickness() const
         {
         return (m_nThickness);
         }

      void SetThickness(int nThickness)
         {
         m_nThickness = nThickness;

         return;
         }

      int GetLineType() const
         {
         return (m_nLineType);
         }

      void SetLineType(int nLineType)
         {
         m_nLineType = nLineType;

         return;
         }

      void Set(int nDiameter, cv::Scalar Color, int nThickness = 1, int nLineType = 8)
         {
         m_nDiameter = nDiameter;
         m_Color = Color;
         m_nThickness = nThickness;
         m_nLineType = nLineType;

         return;
         }

      void Draw(DCVImage& Image, const DCVPoint& Point, bool bCircle = true)
         {
         int nHS = m_nDiameter / 2;
         Image.Line(DCVPoint(Point.x - nHS, Point.y), DCVPoint(Point.x + nHS, Point.y), m_Color, m_nThickness,
               m_nLineType);
         Image.Line(DCVPoint(Point.x, Point.y - nHS), DCVPoint(Point.x, Point.y + nHS), m_Color, m_nThickness,
               m_nLineType);
         if (bCircle)
            {
            Image.Circle(Point, nHS, m_Color, m_nThickness, m_nLineType);
            } // end if

         return;
         }

   protected:
      int m_nDiameter;
      cv::Scalar m_Color;
      int m_nThickness;
      int m_nLineType;

   private:

   }; // end of class DCrossHairs


#endif /* DQOPENCV_H_ */
