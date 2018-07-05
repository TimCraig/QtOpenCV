/*
 * DCVCameraHandler.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: Tim
 */

/******************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DCVCameraHandler.h"

#include <QCameraInfo>
#include <QList>
#include <QString>
#include <QMessageBox>

/*****************************************************************************
***  class DCVCameraHandler
*****************************************************************************/

/*****************************************************************************
 *
 ***  DCVCameraHandler::DCVCameraHandler
 *
 *****************************************************************************/

DCVCameraHandler::DCVCameraHandler(QWidget* pParent) :
      QObject(pParent),
      m_pParentWidget(pParent),
      m_CameraResolution(320, 240),
      m_bRunning(false),
      m_pActionStart(nullptr),
      m_pActionStop(nullptr),
      m_bMirrorHorizontal(false),
      m_bMirrorVertical(false),
      m_pActionMirrorHorizontal(nullptr),
      m_pActionMirrorVertical(nullptr)
   {
   m_CaptureTimer.setInterval(100);

   return;

   } // end of DCVCameraHandler::DCVCameraHandler

/*****************************************************************************
 *
 ***  DCVCameraHandler::DCVCameraHandler
 *
 *****************************************************************************/

DCVCameraHandler::DCVCameraHandler(QWidget* pParent, QSize CameraResolution) :
      QObject(pParent),
      m_pParentWidget(pParent),
      m_CameraResolution(CameraResolution),
      m_bRunning(false),
      m_pActionStart(nullptr),
      m_pActionStop(nullptr),
      m_bMirrorHorizontal(false),
      m_bMirrorVertical(false),
      m_pActionMirrorHorizontal(nullptr),
      m_pActionMirrorVertical(nullptr)
   {
   m_CaptureTimer.setInterval(100);

   // When the timer times out, capture an image
   connect(&m_CaptureTimer, SIGNAL(timeout()), this, SLOT(CaptureImage()));

   return;

   } // end of DCVCameraHandler::DCVCameraHandler

/*****************************************************************************
 *
 ***  DCVCameraHandler::CreateStartAction
 *
 *****************************************************************************/

void DCVCameraHandler::CreateStartAction()
   {
   m_pActionStart = new QAction(QIcon(":/images/camera_start.png"),
         tr("Start Camera"), this);
   m_pActionStart->setStatusTip(tr("Start the Camera"));

   connect(m_pActionStart, SIGNAL(triggered()), this,
         SLOT(StartCamera()));

   return;

   } // end of method DCVCameraHandler::CreateStartAction

/*****************************************************************************
 *
 ***  DCVCameraHandler::CreateStopAction
 *
 *****************************************************************************/

void DCVCameraHandler::CreateStopAction()
   {
   m_pActionStop = new QAction(QIcon(":/images/camera_stop.png"),
         tr("Stop Camera"), this);
   m_pActionStop->setStatusTip(tr("Stop the Camera"));

   connect(m_pActionStop, SIGNAL(triggered()), this, SLOT(StopCamera()));

   return;

   } // end of method DCVCameraHandler::CreateStopAction

/*****************************************************************************
 *
 ***  DCVCameraHandler::CreateMirrorHorizontalAction
 *
 *****************************************************************************/

void DCVCameraHandler::CreateMirrorHorizontalAction()
   {
   m_pActionMirrorHorizontal = new QAction(tr("Mirror Horizontal"), this);
   m_pActionMirrorHorizontal->setStatusTip(tr("Mirror the Captured Image Horizontally"));

   connect(m_pActionMirrorHorizontal, SIGNAL(triggered()), this, SLOT(MirrorHorizontal()));

   return;

   } // end of method DCVCameraHandler::CreateMirrorHorizontalAction

/*****************************************************************************
 *
 ***  DCVCameraHandler::CreateMirrorVerticalAction
 *
 *****************************************************************************/

void DCVCameraHandler::CreateMirrorVerticalAction()
   {
   m_pActionMirrorVertical = new QAction(tr("Mirror Vertical"), this);
   m_pActionMirrorVertical->setStatusTip(tr("Mirror the Captured Image Vertically"));

   connect(m_pActionMirrorVertical, SIGNAL(triggered()), this, SLOT(MirrorVertical()));

   return;

   } // end of method DCVCameraHandler::CreateMirrorVerticalAction

/*****************************************************************************
 *
 ***  DCVCameraHandler::AddCameraMenu
 *
 *  Add a full "standard" camera menu to the passed QMenuBar.  Adds the top
 *  level "Camera" menu, Camera Control Buttons, and Camera Selections.
 *
 *****************************************************************************/

void DCVCameraHandler::AddCameraMenu(QMenuBar* pMenuBar)
   {
   QMenu* pCameraMenu = pMenuBar->addMenu(tr("&Camera"));
   AddCameraButtons(pCameraMenu);
   pCameraMenu->addSeparator();
   AddCameraMenuMirrorItems(pCameraMenu);
   pCameraMenu->addSeparator();
   AddCamerasMenuItems(pCameraMenu);

   return;

   } // end of method DCVCameraHandler::AddCameraMenu

/*****************************************************************************
 *
 ***  DCVCameraHandler::AddCamerasMenuItems
 *
 *  Add all available camera devices to the Camera menu.  Select the first one
 *  as the default.
 *
 *****************************************************************************/

void DCVCameraHandler::AddCamerasMenuItems(QMenu* pMenu)
   {
   QList<QCameraInfo> Cameras = QCameraInfo::availableCameras();

   QActionGroup* pVideoDevicesGroup = new QActionGroup(this);
   pVideoDevicesGroup->setExclusive(true);

   bool bFirst = true;
   int nCamera = 0;
   foreach (const QCameraInfo& Camera, Cameras)
      {
      QString strDescription = Camera.description();
      QAction* pVideoDeviceAction =
            new QAction(strDescription, pVideoDevicesGroup);
      pVideoDeviceAction->setCheckable(true);
      pVideoDeviceAction->setData(QVariant(nCamera));
      if (bFirst)
         {
         // Grab the first available camera device as default
         pVideoDeviceAction->setChecked(true);
         bFirst = false;
         } // end if

      pMenu->addAction(pVideoDeviceAction);

      nCamera++;
      } // end foreach

   connect(pVideoDevicesGroup, SIGNAL(triggered(QAction*)),
         SLOT(UpdateCameraDevice(QAction*)));

   SetCamera(0);

   return;

   } // end of method DCVCameraHandler::SetupCamerasMenuItems

/*****************************************************************************
 *
 ***  DCVCameraHandler::AddCameraButtons
 *
 *  Add the Camera Control Buttons to the specified menu
 *
 *****************************************************************************/

void DCVCameraHandler::AddCameraButtons(QMenu* pMenu)
   {
   if (m_pActionStart == nullptr)
      {
      CreateStartAction();
      } // end if

   m_pActionStart->setEnabled(false);
   pMenu->addAction(m_pActionStart);

   if (m_pActionStop == nullptr)
      {
      CreateStopAction();
      } // end if

   m_pActionStop->setEnabled(false);
   pMenu->addAction(m_pActionStop);

   return;

   } // end of method DCVCameraHandler::AddCameraButtons

/*****************************************************************************
 *
 ***  DCVCameraHandler::AddCameraMenuMirrorItems
 *
 *  Add the Camera Control MenuMirrorItems to the specified menu
 *
 *****************************************************************************/

void DCVCameraHandler::AddCameraMenuMirrorItems(QMenu* pMenu)
   {
   if (m_pActionMirrorHorizontal == nullptr)
      {
      CreateMirrorHorizontalAction();
      } // end if

   m_pActionMirrorHorizontal->setCheckable(true);
   m_pActionMirrorHorizontal->setChecked(m_bMirrorHorizontal);
   pMenu->addAction(m_pActionMirrorHorizontal);

   if (m_pActionMirrorVertical == nullptr)
      {
      CreateMirrorVerticalAction();
      } // end if

   m_pActionMirrorVertical->setCheckable(true);
   m_pActionMirrorVertical->setChecked(m_bMirrorVertical);
   pMenu->addAction(m_pActionMirrorVertical);

   return;

   } // end of method DCVCameraHandler::AddCameraMenuMirrorItems

/*****************************************************************************
 *
 ***  DCVCameraHandler::AddCameraButtons
 *
 *  Add the Camera Control Buttons to the specified toolbar
 *
 *****************************************************************************/

void DCVCameraHandler::AddCameraButtons(QToolBar* pToolBar)
   {
   if (m_pActionStart == nullptr)
      {
      CreateStartAction();
      } // end if

   m_pActionStart->setEnabled(false);
   pToolBar->addAction(m_pActionStart);

   if (m_pActionStop == nullptr)
      {
      CreateStopAction();
      } // end if

   m_pActionStop->setEnabled(false);
   pToolBar->addAction(m_pActionStop);

   return;

   } // end of method DCVCameraHandler::AddCameraButtons

/*****************************************************************************
 *
 ***  DCVCameraHandler::SetCamera
 *
 *****************************************************************************/

void DCVCameraHandler::SetCamera(int nCamera)
   {
   StopCamera();

   m_ImageCapture.release();
   m_ImageCapture.open(nCamera);

   if (m_ImageCapture.isOpened())
      {
      SetResolution(m_CameraResolution);

      StartCamera();
      } // end if
   else
      {
      QMessageBox::warning(m_pParentWidget, tr("Camera Handler"), tr("Failed to open camera"));
      } // end else

   return;

   } // end of method DCVCameraHandler::SetCamera

/******************************************************************************
*
***  DCVCameraHandler::SetResolution
*
******************************************************************************/

bool DCVCameraHandler::SetResolution(int nWidth, int nHeight)
   {
   QSize Resolution(nWidth, nHeight);

   return (SetResolution(Resolution));

   } // end of method DCVCameraHandler::SetResolution

/******************************************************************************
*
***  DCVCameraHandler::SetResolution
*
* Set the Camera Resolution.  Do a check to make sure it happened.  Save the
* Actual Resolution and return a success indication.
*
******************************************************************************/

bool DCVCameraHandler::SetResolution(QSize Resolution)
   {
   m_ImageCapture.set(CV_CAP_PROP_FRAME_WIDTH, Resolution.width());
   m_ImageCapture.set(CV_CAP_PROP_FRAME_HEIGHT, Resolution.height());

   QSize Actual = GetActualResolution();

   m_CameraResolution = Actual;

   bool bRet = (Actual == Resolution);
   if (!bRet)
      {
      QString strMsg(tr("Failed to Set Resolution"));
      QMessageBox::warning(m_pParentWidget, tr("DCVCameraHandler"), strMsg);
      } // end if

   return (bRet);

   } // end of method DCVCameraHandler::SetResolution

/******************************************************************************
*
***  DCVCameraHandler::GetActualResolution()
*
* Query the underlying VideoCapture for the actual resolution since what was
* attempted to be set might not have been supported.
*
******************************************************************************/

QSize DCVCameraHandler::GetActualResolution()
   {
   int nWidth = static_cast<int>(m_ImageCapture.get(CV_CAP_PROP_FRAME_WIDTH));
   int nHeight = static_cast<int>(m_ImageCapture.get(CV_CAP_PROP_FRAME_HEIGHT));

   return (QSize(nWidth, nHeight));

   } // end of method DCVCameraHandler::GetActualResolution

/*****************************************************************************
 *
 ***  DCVCameraHandler::UpdateCameraDevice
 *
 * Handle a new camera being selected from the menu.
 *
 *****************************************************************************/

void DCVCameraHandler::UpdateCameraDevice(QAction* pAction)
   {
   SetCamera(pAction->data().toInt());

   return;

   } // end of method DCVCameraHandler::UpdateCameraDevice

/*****************************************************************************
 *
 ***  DCVCameraHandler::StartCamera
 *
 *****************************************************************************/

void DCVCameraHandler::StartCamera()
   {
   m_pActionStart->setDisabled(true);
   m_CaptureTimer.start();
   m_pActionStop->setEnabled(true);
   m_bRunning = true;

   emit CameraStarted();

   return;

   } // end of method DCVCameraHandler::StartCamera

/*****************************************************************************
 *
 ***  DCVCameraHandler::StopCamera
 *
 *****************************************************************************/

void DCVCameraHandler::StopCamera()
   {
   m_pActionStop->setDisabled(true);
   m_CaptureTimer.stop();
   m_pActionStart->setEnabled(true);
   m_bRunning = false;

   emit CameraStopped();

   return;

   } // end of method DCVCameraHandler::StopCamera

/******************************************************************************
*
***  DCVCameraHandler::MirrorHorizontal
*
******************************************************************************/

void DCVCameraHandler::MirrorHorizontal()
   {
   m_bMirrorHorizontal = !m_bMirrorHorizontal;

   return;

   } // end of method DCVCameraHandler::MirrorHorizontal


/******************************************************************************
*
***  DCVCameraHandler::MirrorVertical
*
******************************************************************************/

void DCVCameraHandler::MirrorVertical()
   {
   m_bMirrorVertical = !m_bMirrorVertical;

   return;

   } // end of method DCVCameraHandler::MirrorVertical

/*****************************************************************************
 *
 ***  DCVCameraHandler::CaptureImage
 *
 *****************************************************************************/

void DCVCameraHandler::CaptureImage()
   {
   if (m_ImageCapture.read(m_CapturedImage))
      {
      if (m_bMirrorHorizontal || m_bMirrorVertical)
         {
         DCVImage::EFlipMode eMode = DCVImage::eFlipX;
         if (m_bMirrorHorizontal && m_bMirrorVertical)
            {
            eMode = DCVImage::eFlipXY;
            } // end if
         else if (m_bMirrorHorizontal)
            {
            eMode = DCVImage::eFlipY;
            }  // end else if

         DCVImage Mirror;
         m_CapturedImage.Flip(Mirror, eMode);
         emit imageCaptured(Mirror);
         } // end if
      else
         {
         emit imageCaptured(m_CapturedImage);
         } // end else
      } // end if

   return;

   } // end of method DCVCameraHandler::CaptureImage
