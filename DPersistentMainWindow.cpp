/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DPersistentMainWindow.h"

#include <QMessageBox>
#include <QDir>

/*****************************************************************************
 ***  class DPersistentMainWindow
 *****************************************************************************/

/*****************************************************************************
 *
 ***  DPersistentMainWindow::DPersistentMainWindow
 *
 *****************************************************************************/

DPersistentMainWindow::DPersistentMainWindow(QWidget* pParent) :
      Base(pParent),
      m_strAppName("DruaiApp"),
      m_bPersistenceEnabled(true)
   {

   return;

   } // end of DPersistentMainWindow::DPersistentMainWindow

/*****************************************************************************
 *
 ***  DPersistentMainWindow::~DPersistentMainWindow
 *
 *****************************************************************************/

DPersistentMainWindow::~DPersistentMainWindow()
   {

   return;

   } // end of DPersistentMainWindow::~DPersistentMainWindow

/*****************************************************************************
 *
 ***  DPersistentMainWindow::Initialize
 *
 *****************************************************************************/

void DPersistentMainWindow::Initialize()
   {
   if (m_bPersistenceEnabled)
      {
      if (!LoadParameters())
         {
         QMessageBox::warning(this, QString(tr("Load Parameters")),
               QString(tr("Parameter Load Failed! Using Defaults")));
         } // end if
      } // end if

   return;

   } // end of method DPersistentMainWindow::Initialize

/*****************************************************************************
 *
 ***  DPersistentMainWindow::SetupCentralWidget
 *
 *****************************************************************************/

void DPersistentMainWindow::SetupCentralWidget()
   {

   return;

   } // end of method DPersistentMainWindow::SetupCentralWidget

/******************************************************************************
*
***  DPersistentMainWindow::closeEvent
*
******************************************************************************/

void DPersistentMainWindow::closeEvent(QCloseEvent* pEvent)
   {
   if (m_bPersistenceEnabled && QMessageBox::question(this,
         QString(tr("Save Parameters")),
         QString(tr("Save Program Parameters Before Closing?")))
         == QMessageBox::Yes)
      {
      if (!SaveParameters())
         {
         QMessageBox::warning(this, QString(tr("Save Parameters")),
               QString(tr("Parameter Save Failed!")));
         } // end if
      } // end if

   pEvent->accept();

   return;

   } // end of method DPersistentMainWindow::closeEvent

/*****************************************************************************
 *
 ***  DPersistentMainWindow::LoadParameters
 *
 * Do nothing version to avoid the hassle of dealing with a pure virtual
 * function.
 *
 *****************************************************************************/

bool DPersistentMainWindow::LoadParameters()
   {

   return (true);

   } // end of method DPersistentMainWindow::LoadParameters

/*****************************************************************************
 *
 ***  DPersistentMainWindow::SaveParameters
 *
 * Do nothing version to avoid the hassle of dealing with a pure virtual
 * function.
 *
 *****************************************************************************/

bool DPersistentMainWindow::SaveParameters()
   {

   return (true);

   } // end of method DPersistentMainWindow::SaveParameters

/******************************************************************************
*
***  DPersistentMainWindow::GetParameterPath
*
******************************************************************************/

QString DPersistentMainWindow::GetParameterPath(const QString& strExt) const
   {
   QDir Path(QDir::current());
   QString strFileName = m_strAppName;
   if (strExt[0] != '.')
      {
      strFileName += '.';
      } // end if
   strFileName += strExt;

   return (Path.filePath(strFileName));

   } // end of method DPersistentMainWindow::GetParameterPath

