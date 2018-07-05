/*
 * DQRubberBand.h
 *
 *  Created on: Apr 28, 2014
 *      Author: Tim
 */

#ifndef DQRUBBERBAND_H_
#define DQRUBBERBAND_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QRubberBand>

/*****************************************************************************
 *
 ***  class DQRubberBand
 *
 *  This class extends QRubberBand by handling the mouse events in a
 *  consistent way.
 *
 *  In the containing widget, call the Begin() method in the mousePressEvent.
 *  Call the Move() method in the mouseMoveEvent.  Finally, call the End()
 *  method in the mouseReleaseEvent.
 *
 *****************************************************************************/

class DQRubberBand : public QRubberBand
   {
   Q_OBJECT

   public:
      DQRubberBand(QRubberBand::Shape Shape = QRubberBand::Rectangle, QWidget* pParent = nullptr) :
            QRubberBand(Shape, pParent)
         {
         m_bEnabled = true;
         m_bBanding = false;
         // Geometry isn't nulled by the base class
         setGeometry(QRect());

         return;
         }

      DQRubberBand(const DQRubberBand& src) = delete;

      ~DQRubberBand() = default;

      DQRubberBand& operator=(const DQRubberBand& rhs) = delete;

      virtual void Begin(const QPoint& pt);
      virtual void Move(const QPoint& pt);
      virtual void End(const QPoint& pt);

      bool IsBanding() const
         {
         return (m_bBanding);
         }

      void Clear()
         {
         setGeometry(QRect());
         m_bBanding = false;
         return;
         }

      bool IsEnabled() const
         {
         return (m_bEnabled);
         }

      void Enable(bool bEnable = true)
         {
         m_bEnabled = bEnable;
//         m_bEnabled ? show() : hide();
         return;
         }

      void Disable(bool bDisable = true)
         {
         m_bEnabled = !bDisable;
 //        m_bEnabled ? show() : hide();
         return;
         }

   protected:
      bool m_bEnabled;
      bool m_bBanding;
      QPoint m_Origin;

   private:

   }; // end of class QRubberBand

#endif /* DQRUBBERBAND_H_ */
