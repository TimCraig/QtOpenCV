/*
 * DQCameraHandler.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Tim
 */

/******************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DQCameraHandler.h"
#include <QCameraInfo>

/*****************************************************************************
***  class DQCameraHandler
*****************************************************************************/

Q_DECLARE_METATYPE(QCameraInfo)

/*****************************************************************************
 *
 ***  DQCameraHandler::DQCameraHandler
 *
 *****************************************************************************/

DQCameraHandler::DQCameraHandler(QObject* pParent) :
      QObject(pParent),
      m_pCamera(nullptr),
      m_CameraResolution(320, 240),
      m_pViewFinder(nullptr),
      m_pImageCapture(nullptr),
      m_bRunning(false),
      m_pActionStart(nullptr),
      m_pActionStop(nullptr),
      m_bMirrorHorizontal(false),
      m_bMirrorVertical(false),
      m_pActionMirrorHorizontal(nullptr),
      m_pActionMirrorVertical(nullptr)
   {
//   m_pViewFinder = new QCameraViewfinder();

   m_CaptureTimer.setInterval(100);

   return;

   } // end of DQCameraHandler::DQCameraHandler

/*****************************************************************************
 *
 ***  DQCameraHandler::DQCameraHandler
 *
 *****************************************************************************/

DQCameraHandler::DQCameraHandler(QObject* pParent, QSize CameraResolution) :
      QObject(pParent),
      m_pCamera(new QCamera(QCameraInfo::defaultCamera())),
      m_CameraResolution(CameraResolution),
      m_pViewFinder(nullptr),
      m_pImageCapture(nullptr),
      m_bRunning(false),
      m_pActionStart(nullptr),
      m_pActionStop(nullptr),
      m_bMirrorHorizontal(false),
      m_bMirrorVertical(false),
      m_pActionMirrorHorizontal(nullptr),
      m_pActionMirrorVertical(nullptr)
   {
//   m_pViewFinder = new QCameraViewfinder();

   m_CaptureTimer.setInterval(100);

   return;

   } // end of DQCameraHandler::DQCameraHandler

/*****************************************************************************
 *
 ***  DQCameraHandler::CreateStartAction
 *
 *****************************************************************************/

void DQCameraHandler::CreateStartAction()
   {
   m_pActionStart = new QAction(QIcon(":/images/camera_start.png"),
         tr("Start Camera"), this);
   m_pActionStart->setStatusTip(tr("Start the Camera"));

   connect(m_pActionStart, SIGNAL(triggered()), this,
         SLOT(StartCamera()));

   return;

   } // end of method DQCameraHandler::CreateStartAction

/*****************************************************************************
 *
 ***  DQCameraHandler::CreateStopAction
 *
 *****************************************************************************/

void DQCameraHandler::CreateStopAction()
   {
   m_pActionStop = new QAction(QIcon(":/images/camera_stop.png"),
         tr("Stop Camera"), this);
   m_pActionStop->setStatusTip(tr("Stop the Camera"));

   connect(m_pActionStop, SIGNAL(triggered()), this, SLOT(StopCamera()));

   return;

   } // end of method DQCameraHandler::CreateStopAction

/*****************************************************************************
 *
 ***  DQCameraHandler::CreateMirrorHorizontalAction
 *
 *****************************************************************************/

void DQCameraHandler::CreateMirrorHorizontalAction()
   {
   m_pActionMirrorHorizontal = new QAction(tr("Mirror Horizontal"), this);
   m_pActionMirrorHorizontal->setStatusTip(tr("Mirror the Captured Image Horizontally"));

   connect(m_pActionMirrorHorizontal, SIGNAL(triggered()), this, SLOT(MirrorHorizontal()));

   return;

   } // end of method DQCameraHandler::CreateMirrorHorizontalAction

/*****************************************************************************
 *
 ***  DQCameraHandler::CreateMirrorVerticalAction
 *
 *****************************************************************************/

void DQCameraHandler::CreateMirrorVerticalAction()
   {
   m_pActionMirrorVertical = new QAction(tr("Mirror Vertical"), this);
   m_pActionMirrorVertical->setStatusTip(tr("Mirror the Captured Image Vertically"));

   connect(m_pActionMirrorVertical, SIGNAL(triggered()), this, SLOT(MirrorVertical()));

   return;

   } // end of method DQCameraHandler::CreateMirrorVerticalAction

/*****************************************************************************
 *
 ***  DQCameraHandler::AddCameraMenu
 *
 *  Add a full "standard" camera menu to the passed QMenuBar.  Adds the top
 *  level "Camera" menu, Camera Control Buttons, and Camera Selections.
 *
 *****************************************************************************/

void DQCameraHandler::AddCameraMenu(QMenuBar* pMenuBar)
   {
   QMenu* pCameraMenu = pMenuBar->addMenu(tr("&Camera"));
   AddCameraButtons(pCameraMenu);
   pCameraMenu->addSeparator();
   AddCameraMenuMirrorItems(pCameraMenu);
   pCameraMenu->addSeparator();
   AddCamerasMenuItems(pCameraMenu);

   return;

   } // end of method DQCameraHandler::AddCameraMenu

/*****************************************************************************
 *
 ***  DQCameraHandler::AddCamerasMenuItems
 *
 *  Add all available camera devices to the Camera menu.  Select the first one
 *  as the default.
 *
 *****************************************************************************/

void DQCameraHandler::AddCamerasMenuItems(QMenu* pMenu)
   {
   //Cameras:
   QList<QCameraInfo> Cameras = QCameraInfo::availableCameras();

   QActionGroup* pVideoDevicesGroup = new QActionGroup(this);
   pVideoDevicesGroup->setExclusive(true);
   for (const QCameraInfo& CameraInfo : Cameras)
      {
      QAction* pVideoDeviceAction =
            new QAction(CameraInfo.description(), pVideoDevicesGroup);
      pVideoDeviceAction->setCheckable(true);
      pVideoDeviceAction->setData(QVariant::fromValue(CameraInfo));

      if (CameraInfo == QCameraInfo::defaultCamera())
         {
         // Grab the first available camera device as default
         pVideoDeviceAction->setChecked(true);
         } // end if

      pMenu->addAction(pVideoDeviceAction);
      } // end foreach

   connect(pVideoDevicesGroup, &QActionGroup::triggered, this,
         &DQCameraHandler::UpdateCameraDevice);

   SetCamera(QCameraInfo::defaultCamera());

   return;

   } // end of method DQCameraHandler::SetupCamerasMenuItems

/*****************************************************************************
 *
 ***  DQCameraHandler::AddCameraButtons
 *
 *  Add the Camera Control Buttons to the specified menu
 *
 *****************************************************************************/

void DQCameraHandler::AddCameraButtons(QMenu* pMenu)
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

   } // end of method DQCameraHandler::AddCameraButtons

/*****************************************************************************
 *
 ***  DQCameraHandler::AddCameraMenuMirrorItems
 *
 *  Add the Camera Control MenuMirrorItems to the specified menu
 *
 *****************************************************************************/

void DQCameraHandler::AddCameraMenuMirrorItems(QMenu* pMenu)
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

   } // end of method DQCameraHandler::AddCameraMenuMirrorItems

/*****************************************************************************
 *
 ***  DQCameraHandler::AddCameraButtons
 *
 *  Add the Camera Control Buttons to the specified toolbar
 *
 *****************************************************************************/

void DQCameraHandler::AddCameraButtons(QToolBar* pToolBar)
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

   } // end of method DQCameraHandler::AddCameraButtons

#if 0

/*****************************************************************************
 *
 ***  DQCameraHandler::SetCamera
 *
 *****************************************************************************/

void DQCameraHandler::SetCamera(const QCameraInfo& CameraInfo)
   {
   m_CaptureTimer.stop();
   delete m_pImageCapture;
   delete m_pCamera;

   m_pCamera = new QCamera(CameraInfo);
   m_pImageCapture = new QCameraImageCapture(m_pCamera);
   m_pImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

//   m_pCamera->setViewfinder(m_pViewFinder);
   m_pCamera->setCaptureMode(QCamera::CaptureStillImage);

   // When the timer times out, capture an image
   SetResolution(m_CameraResolution);
   connect(&m_CaptureTimer, SIGNAL(timeout()), this, SLOT(CaptureImage()));

   // Connection to handle the captured image when it arrives
   connect(m_pImageCapture, SIGNAL(imageCaptured(int, const QImage&)), this,
         SLOT(ImageCaptured(int, const QImage&)));

   return;

   } // end of method DQCameraHandler::SetCamera

#endif

/*****************************************************************************
 *
 ***  DQCameraHandler::SetCamera
 *
 *****************************************************************************/

void DQCameraHandler::SetCamera(const QCameraInfo& CameraInfo)
   {
   m_CaptureTimer.stop();
//   delete m_pImageCapture;
   delete m_pCamera;

   m_pCamera = new QCamera(CameraInfo);

   QVideoWidget* pVideoWidget = new QVideoWidget();
   pVideoWidget->setFixedSize(m_CameraResolution);
   m_pCamera->setViewfinder(pVideoWidget);
   m_pCamera->setCaptureMode(QCamera::CaptureStillImage);

   // When the timer times out, capture an image
   connect(&m_CaptureTimer, SIGNAL(timeout()), this, SLOT(CaptureImage()));

   // Connection to handle the captured image when it arrives
   connect(m_pImageCapture, SIGNAL(imageCaptured(int, const QImage&)), this,
         SLOT(ImageCaptured(int, const QImage&)));

   return;

   } // end of method DQCameraHandler::SetCamera

/******************************************************************************
*
***  DQCameraHandler::SetResolution
*
******************************************************************************/

void DQCameraHandler::SetResolution(int nWidth, int nHeight)
   {
   m_CameraResolution.setWidth(nWidth);
   m_CameraResolution.setHeight(nHeight);

   if (m_pImageCapture != nullptr)
      {
      QImageEncoderSettings Settings = m_pImageCapture->encodingSettings();

      Settings.setResolution(nWidth, nHeight);

      m_pImageCapture->setEncodingSettings(Settings);
      } // end if

   return;

   } // end of method DQCameraHandler::SetResolution

/******************************************************************************
*
***  DQCameraHandler::SetResolution
*
******************************************************************************/

void DQCameraHandler::SetResolution(QSize Resolution)
   {
   m_CameraResolution = Resolution;

   if (m_pImageCapture != nullptr)
      {
      QImageEncoderSettings Settings = m_pImageCapture->encodingSettings();

      Settings.setResolution(Resolution);

      m_pImageCapture->setEncodingSettings(Settings);
      } // end if

   return;

   } // end of method DQCameraHandler::SetResolution

/*****************************************************************************
 *
 ***  DQCameraHandler::UpdateCameraDevice
 *
 * Handle a new camera being selected from the menu.
 *
 *****************************************************************************/

void DQCameraHandler::UpdateCameraDevice(QAction* pAction)
   {
   SetCamera(qvariant_cast<QCameraInfo>(pAction->data()));

   return;

   } // end of method DQCameraHandler::UpdateCameraDevice

/*****************************************************************************
 *
 ***  DQCameraHandler::StartCamera
 *
 *****************************************************************************/

void DQCameraHandler::StartCamera()
   {
   m_pActionStart->setDisabled(true);
   m_pCamera->start();
   m_CaptureTimer.start();
   m_pActionStop->setEnabled(true);
   m_bRunning = true;

   emit CameraStarted();

   return;

   } // end of method DQCameraHandler::StartCamera

/*****************************************************************************
 *
 ***  DQCameraHandler::StopCamera
 *
 *****************************************************************************/

void DQCameraHandler::StopCamera()
   {
   m_pActionStop->setDisabled(true);
   m_CaptureTimer.stop();
   m_pCamera->stop();
   m_pActionStart->setEnabled(true);
   m_bRunning = false;

   emit CameraStopped();

   return;

   } // end of method DQCameraHandler::StopCamera

/******************************************************************************
*
***  DQCameraHandler::MirrorHorizontal
*
******************************************************************************/

void DQCameraHandler::MirrorHorizontal()
   {
   m_bMirrorHorizontal = !m_bMirrorHorizontal;

   return;

   } // end of method DQCameraHandler::MirrorHorizontal


/******************************************************************************
*
***  DQCameraHandler::MirrorVertical
*
******************************************************************************/

void DQCameraHandler::MirrorVertical()
   {
   m_bMirrorVertical = !m_bMirrorVertical;

   return;

   } // end of method DQCameraHandler::MirrorVertical

/*****************************************************************************
 *
 ***  DQCameraHandler::CaptureImage
 *
 *****************************************************************************/

void DQCameraHandler::CaptureImage()
   {
   // Giving a nonexistent directory seems to keep the images from automatically saving
   m_pImageCapture->capture("D:\\Foo\\Capture.jpg");

   return;

   } // end of method DQCameraHandler::CaptureImage

/*****************************************************************************
 *
 ***  DQCameraHandler::ImageCaptured
 *
 * Handle the captured image if it hasn't been passed directly to our parent.
 * This function is virtual so it's easily overridden polymorphically for self
 * contained processing or preprocessing before passing on.  This default stub
 * just passes reemits the imageCaptured signal.
 *
 *****************************************************************************/

void DQCameraHandler::ImageCaptured(int nID, const QImage& Image)
   {
   if (m_bMirrorHorizontal || m_bMirrorVertical)
      {
      QImage Mirror = Image.mirrored(m_bMirrorHorizontal, m_bMirrorVertical);
      emit imageCaptured(nID, static_cast<const QImage&>(Mirror));
      } // end if
   else
      {
      emit imageCaptured(nID, Image);
      } // end else

   return;

   } // end of method DQCameraHandler::ImageCaptured

