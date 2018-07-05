/*****************************************************************************
 **************************** DWXXHistogramWindow.h ***************************
 *****************************************************************************/

#if !defined(__DQHISTOGRAMWIDGET_H__)
#define __DQHISTOGRAMWIDGET_H__
#pragma once

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QFrame>

#include "DHistogram.h"

/*****************************************************************************
 *
 ***  class DQHistogramWidget
 *
 * Class providing a basic Qt Widget for displaying the contents of a
 * DHistogram.
 *
 *****************************************************************************/

class DQHistogramWidget : public QFrame
   {
   Q_OBJECT

   public:
      DQHistogramWidget(QRgb clrLine = qRgb(0, 0, 0),
            QWidget* pParent = nullptr);
      DQHistogramWidget(const DQHistogramWidget& src) = delete;

      ~DQHistogramWidget() = default;

      DQHistogramWidget& operator=(const DQHistogramWidget& rhs) = delete;

      DHistogram& GetHistogram()
         {
         return (m_Histogram);
         }

      void SetLineColor(QRgb clrLine)
         {
         m_RGBLine = clrLine;
         return;
         }

   protected:
      QRgb m_RGBLine;
      DHistogram m_Histogram;

      void paintEvent(QPaintEvent* pEvent);
      void mouseMoveEvent(QMouseEvent* pEvent);

   private:

   }; // end of class DQHistogramWidget

#endif // __DQHISTOGRAMWIDGET_H__
