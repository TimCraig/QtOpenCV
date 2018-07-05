/*
 * DQRubberBand.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: Tim
 */

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QDebug>

#include "DQRubberBand.h"

/*****************************************************************************
 ***  class DQRubberBand
 *****************************************************************************/

/*****************************************************************************
 *
 ***  DQRubberBand::Begin
 *
 *****************************************************************************/

void DQRubberBand::Begin(const QPoint& pt)
   {
   if (m_bEnabled && !m_bBanding)
      {
      m_bBanding = true;
      hide();

      m_Origin = pt;
      setGeometry(QRect(m_Origin, QSize()));
      show();
      } // end if

   return;

   } // end of method DQRubberBand::Begin

/*****************************************************************************
 *
 ***  DQRubberBand::Move
 *
 *****************************************************************************/

void DQRubberBand::Move(const QPoint& pt)
   {
   if (m_bEnabled && m_bBanding)
      {
      setGeometry(QRect(m_Origin, pt).normalized());
      } // end if

   return;

   } // end of method DQRubberBand::Move

/*****************************************************************************
 *
 ***  DQRubberBand::End
 *
 *****************************************************************************/

void DQRubberBand::End(const QPoint& pt)
   {
   if (m_bEnabled && m_bBanding)
      {
      setGeometry(QRect(m_Origin, pt).normalized());

      m_bBanding = false;
      } // end if

   return;

   } // end of method DQRubberBand::End
