/*
 * DQCameraHandler.h
 *
 *  Created on: Apr 10, 2014
 *      Author: Tim
 */

#ifndef DQCAMERAHANDLER_H_
#define DQCAMERAHANDLER_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QTimer>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

/*****************************************************************************
 *
 ***  class DQCameraHandler
 *
 * Class to provide a standard interface to handing a QCamera.  A user
 * interface is provide with menu and toolbar items.
 *
 *****************************************************************************/

class DQCameraHandler : public QObject
   {
   Q_OBJECT

   public:
      DQCameraHandler(QObject* pParent);
      DQCameraHandler(QObject* pParent, QSize CameraResolution);     
      DQCameraHandler(const DQCameraHandler& src) = delete;

      ~DQCameraHandler() = default;

      DQCameraHandler& operator=(const DQCameraHandler& rhs) = delete;

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

   signals:
      void imageCaptured(int nID, const QImage& Image);
      void CameraStarted();
      void CameraStopped();

   public slots:
      void StartCamera();
      void StopCamera();
      void MirrorHorizontal();
      void MirrorVertical();
      void SetResolution(int nWidth, int nHeight);
      void SetResolution(QSize Resolution);

      void SetCaptureInterval(int nInterval)
         {
         m_CaptureTimer.setInterval(nInterval);
         return;
         }

   protected:
      QCamera* m_pCamera;
      QSize m_CameraResolution;
      QCameraViewfinder* m_pViewFinder;
      QCameraImageCapture* m_pImageCapture;
      bool m_bRunning;
      QImage m_CapturedImage;
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
      void SetCamera(const QCameraInfo& CameraInfo);
      void UpdateCameraDevice(QAction *pAction);

      void CaptureImage();
      virtual void ImageCaptured(int nID, const QImage &Image);

   private:

   };
// end of class DQCameraHandler

#endif /* DQCAMERAHANDLER_H_ */
