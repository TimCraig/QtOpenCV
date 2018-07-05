/*
 * DCVCameraHandler.h
 *
 *  Created on: Oct 12, 2015
 *      Author: Tim
 */

#ifndef DCVCAMERAHANDLER_H_
#define DCVCAMERAHANDLER_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

#include <opencv2/videoio.hpp>
#include "CVImage.h"

/*****************************************************************************
 *
 ***  class DCVCameraHandler
 *
 * Class to provide a stanard interface to an OpenCV camera.  A user interace
 * with menu and toolbar items is provided.
 *
 *****************************************************************************/

class DCVCameraHandler : public QObject
   {
   Q_OBJECT

   public:
      DCVCameraHandler(QWidget* pParent);
      DCVCameraHandler(QWidget* pParent, QSize CameraResolution);
      DCVCameraHandler(const DCVCameraHandler& src) = delete;

      ~DCVCameraHandler()
         {
         return;
         }

      DCVCameraHandler& operator=(const DCVCameraHandler& rhs) = delete;

      void AddCameraMenu(QMenuBar* pMenuBar);
      void AddCamerasMenuItems(QMenu* pMenu);
      void AddCameraButtons(QMenu* pMenu);
      void AddCameraMenuMirrorItems(QMenu* pMenu);
      void AddCameraButtons(QToolBar* pToolBar);

      int GetCaptureInterval() const
         {
         return (m_CaptureTimer.interval());
         }

      bool IsRunning() const
         {
         return (m_bRunning);
         }

      // Query the underlying device for actual resolution
      // TTC Should be const but cv::VideoCapture is not const correct
      QSize GetActualResolution();

   signals:
      void imageCaptured(DCVImage& Image);
      void CameraStarted();
      void CameraStopped();

   public slots:
      void StartCamera();
      void StopCamera();
      void MirrorHorizontal();
      void MirrorVertical();
      bool SetResolution(int nWidth, int nHeight);
      bool SetResolution(QSize Resolution);

      void SetCaptureInterval(int nInterval)
         {
         m_CaptureTimer.setInterval(nInterval);
         return;
         }

   protected:
      QWidget* m_pParentWidget;
      QSize m_CameraResolution;
      cv::VideoCapture m_ImageCapture;
      bool m_bRunning;
      DCVImage m_CapturedImage;
      QTimer m_CaptureTimer;
      QAction* m_pActionStart;
      QAction* m_pActionStop;
      bool m_bMirrorHorizontal;
      bool m_bMirrorVertical;
      QAction* m_pActionMirrorHorizontal;
      QAction* m_pActionMirrorVertical;

      void CreateStartAction();
      void CreateStopAction();
      void CreateMirrorHorizontalAction();
      void CreateMirrorVerticalAction();

   protected slots:
      void SetCamera(int nCamera);
      void UpdateCameraDevice(QAction *pAction);

      void CaptureImage();

   private:

   };
// end of class DCVCameraHandler

#endif /* DCVCAMERAHANDLER_H_ */
