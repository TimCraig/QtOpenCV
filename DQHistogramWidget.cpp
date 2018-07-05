/*****************************************************************************
 *************************** DQHistogramWidget.cpp ***************************
 *****************************************************************************/

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QToolTip>
#include <QPainter>
#include <QMouseEvent>

#include "DQHistogramWidget.h"

/*****************************************************************************
 ***  class DQHistogramWidget
 *****************************************************************************/

/*****************************************************************************
 *
 *  DQHistogramWidget::DQHistogramWidget
 *
 *****************************************************************************/

DQHistogramWidget::DQHistogramWidget(QRgb clrLine /* = qRgb(0, 0, 0) */,
      QWidget* pParent /* = nullptr */)
      : QFrame(pParent)
   {
   m_RGBLine = clrLine;

   setFrameStyle(QFrame::Box | QFrame::Plain);
   setLineWidth(1);

   setFixedWidth(m_Histogram.GetBinCount() + (2 * frameWidth()));

   setMouseTracking(true);

   show();

   return;

   } // End of function DQHistogramWidget::DQHistogramWidget

/*****************************************************************************
 *
 *  DQHistogramWidget::paintEvent
 *
 *****************************************************************************/

void DQHistogramWidget::paintEvent(QPaintEvent* pEvent)
   {
   QFrame::paintEvent(pEvent);

   QPainter Painter(this);

   // Drawing has to be adjusted to fit in the contents rectangle
   QRect CRect = contentsRect();
   QPoint BL = CRect.bottomLeft();

   QPen Pen(m_RGBLine);
   Painter.setPen(Pen);

   if (m_Histogram.GetMaxCount() > 0)
      {
      for (int i = 0 ; i < m_Histogram.GetBinCount() ; i++)
         {
         int x = CRect.x() + i;
         int nCount = m_Histogram.GetBin(i);

         // Scale the output so the largest bin is full scale
         int nBar = ((CRect.height() - 1) * nCount) /
               m_Histogram.GetMaxCount();

         if (nCount > 0)
            {
            Painter.drawLine(x, BL.y(), x, BL.y() - nBar);
            } // end if
         } // end for
      } // end if

   return;

   } // End of function DQHistogramWidget::paintEvent

/*****************************************************************************
 *
 *  DQHistogramWidget::mouseMoveEvent
 *
 * On mouse over show the bin number and count.
 *
 *****************************************************************************/

void DQHistogramWidget::mouseMoveEvent(QMouseEvent* pEvent)
   {
   QString strData;

   // Compenstate the x coordinate for the frame width
   int x = pEvent->x() - frameWidth();
   if ((x >= 0) && (x < m_Histogram.GetBinCount()))
      {
      // The string is the bin number and bin count.  The histogram fills
      // the control so this simple method works
      strData.sprintf("(%d, %u)", x, m_Histogram.GetBin(x));
      QToolTip::showText(pEvent->globalPos(), strData, this);
      } // end if

   return;

   } // End of function DQHistogramWidget::mouseMoveEvent

