/*
 * ImageWidget.h
 *
 *  Created on: Mar 29, 2014
 *      Author: Tim
 */

#ifndef IMAGEWIDGET_H_
#define IMAGEWIDGET_H_

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include <QWidget>

#include "DQRubberBand.h"
#include "DQImage.h"

/*****************************************************************************
*
***  class DImageWidget
*
*  Class to provide a dedicated way to display and interact with a DQImage
*  (enhanced QImage).
*
*****************************************************************************/

class DImageWidget : public QWidget
   {
   Q_OBJECT

   public:
      enum ETipType { eTipRGB, eTipIndex };

      DImageWidget(QWidget* pParent = nullptr);

      DImageWidget(const DImageWidget& src) = delete;

      ~DImageWidget() = default;

      DImageWidget& operator=(const DImageWidget& rhs) = delete;

      bool SetImage(const QImage& Image);
      bool SetImage(const DQImage& Image);

      bool IsRubberBand() const
         {
         return (m_pRubberBand->IsEnabled());
         }

      const DQImage& GetImage() const
         {
         return (m_Image);
         }

      DQImage& GetImage()
          {
          return (m_Image);
          }

      void SetTipType(ETipType eTipType)
         {
         m_eTipType = eTipType;
         return;
         }

      ETipType GetTipType() const
         {
         return (m_eTipType);
         }

   signals:
      void ROIChanged(const QRect& ROI);

   public slots:
      void SetROI(const QRect& ROI);
      void ClearROI();

      void EnableRubberBand();
      void DisableRubberBand();

   protected:
      DQImage m_Image;
      DQRubberBand* m_pRubberBand;
      ETipType m_eTipType;

      void paintEvent(QPaintEvent* pEvent);

      // Overridden for rubber banding and value display
      void mousePressEvent(QMouseEvent* pEvent);
      void mouseMoveEvent(QMouseEvent* pEvent);
      void mouseReleaseEvent(QMouseEvent *pEvent);

   private:

   }; // end of class DImageWidget

#endif /* IMAGEWIDGET_H_ */
