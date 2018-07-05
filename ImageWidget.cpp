/*
 * ImageWidget.cpp
 *
 *  Created on: Mar 29, 2014
 *      Author: Tim
 */

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "ImageWidget.h"
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>

/*****************************************************************************
***  class DImageWidget
*****************************************************************************/

/*****************************************************************************
 *
 ***  DImageWidget::DImageWidget
 *
 *****************************************************************************/

DImageWidget::DImageWidget(QWidget* pParent /* = nullptr */) :
      QWidget(pParent)
   {
   m_pRubberBand = new DQRubberBand(QRubberBand::Rectangle, this);
   m_pRubberBand->Disable();
   m_eTipType = eTipRGB;
   setMouseTracking(true);

   return;

   } // end of DImageWidget::DImageWidget

/*****************************************************************************
 *
 ***  DImageWidget::SetImage
 *
 *****************************************************************************/

bool DImageWidget::SetImage(const QImage& Image)
   {
   bool bRet = true;

   // Work with a copy until we figure out exactly how the copy on change works
   m_Image = Image.copy();

   // Keep the existing ROI between frames (make a setting?)
   m_Image.SetROI(m_pRubberBand->geometry());

   update();

   return (bRet);

   } // end of method DImageWidget::SetImage

/*****************************************************************************
 *
 ***  DImageWidget::SetImage
 *
 *****************************************************************************/

bool DImageWidget::SetImage(const DQImage& Image)
   {
   bool bRet = true;

   // Work with a copy until we figure out exactly how the copy on change works
   m_Image = Image.copy();

   // Keep the existing ROI between frames (make a setting?)
   m_Image.SetROI(m_pRubberBand->geometry());

   update();

   return (bRet);

   } // end of method DImageWidget::SetImage

/*****************************************************************************
 *
 ***  DImageWidget::paintEvent
 *
 *****************************************************************************/

void DImageWidget::paintEvent(QPaintEvent* /* pEvent */)
   {
   QPainter Painter(this);
   Painter.drawImage(0, 0, m_Image);

   return;

   } // end of method DImageWidget::paintEvent

/*****************************************************************************
 *
 ***  DImageWidget::mousePressEvent
 *
 *****************************************************************************/

void DImageWidget::mousePressEvent(QMouseEvent *pEvent)
   {
   if (m_pRubberBand->IsEnabled())
      {
      ClearROI();
      m_pRubberBand->Begin(pEvent->pos());

      update();
      } // end if

   return;

   } // end of method DImageWidget::mousePressEvent

/*****************************************************************************
 *
 ***  DImageWidget::mouseMoveEvent
 *
 *****************************************************************************/

void DImageWidget::mouseMoveEvent(QMouseEvent *pEvent)
   {
   if (m_pRubberBand->IsBanding())
      {
      m_pRubberBand->Move(pEvent->pos());
      update();
      } // end if
   else
      {
      // Display pixel data for pixel under mouse pointer
      QString strData;
      if (m_Image.rect().contains(pEvent->pos(), false))
         {
         if ((m_eTipType == eTipIndex) && (colorCount() > 0))
            {
            int nIndex = m_Image.pixelIndex(pEvent->pos());

            strData.sprintf("Pos(%d, %d)-I(%d)", pEvent->x(), pEvent->y(),
                  nIndex);
            } // end if
         else
            {
            QColor Color(m_Image.pixel(pEvent->pos()));

            strData.sprintf("Pos(%d, %d)-RGB(%u, %u, %u)", pEvent->x(),
                  pEvent->y(),
                  Color.red(), Color.green(), Color.blue());
            } // end else

         QToolTip::showText(pEvent->globalPos(), strData, this);
         } // end if
      } // end else

   return;

   } // end of method DImageWidget::mouseMoveEvent

/*****************************************************************************
 *
 ***  DImageWidget::mouseReleaseEvent
 *
 *****************************************************************************/

void DImageWidget::mouseReleaseEvent(QMouseEvent *pEvent)
   {
   if (m_pRubberBand->IsBanding())
      {
      m_pRubberBand->End(pEvent->pos());

      // Set the image region of interest from rubberband box
      SetROI(m_pRubberBand->geometry());

      update();
      } // end if

   return;

   } // end of method DImageWidget::mouseReleaseEvent

/*****************************************************************************
*
***  DImageWidget::SetROI
*
*****************************************************************************/

void DImageWidget::SetROI(const QRect& ROI)
   {
   if (ROI != m_Image.GetROI())
      {
      m_Image.SetROI(ROI);
      m_pRubberBand->hide();
      m_pRubberBand->setGeometry(ROI);
      m_pRubberBand->show();

      // Let anyone interested know our ROI has changed
      emit ROIChanged(ROI);

      update();
      } // end if

   return;

   } // end of method DImageWidget::SetROI

/*****************************************************************************
*
***  DImageWidget::ClearROI
*
*****************************************************************************/

void DImageWidget::ClearROI()
   {
   SetROI(QRect());

   return;

   } // end of method DImageWidget::ClearROI

/*****************************************************************************
*
***  DImageWidget::EnableRubberBand
*
*****************************************************************************/

void DImageWidget::EnableRubberBand()
   {
   m_pRubberBand->Enable();

   return;

   } // end of method DImageWidget::EnableRubberBand

/*****************************************************************************
*
***  DImageWidget::DisableRubberBand
*
*****************************************************************************/

void DImageWidget::DisableRubberBand()
   {
   m_pRubberBand->Disable();

   return;

   } // end of method DImageWidget::DisableRubberBand
