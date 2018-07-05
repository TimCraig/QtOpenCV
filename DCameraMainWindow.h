#ifndef DCAMERAMAINWINDOW_H
#define DCAMERAMAINWINDOW_H

/*****************************************************************************
******************************  I N C L U D E  *******************************
*****************************************************************************/

#include "DQCameraHandler.h"
#include "DQImage.h"
#include "ImageWidget.h"
#include "DPersistentMainWindow.h"

#include <QGridLayout>
#include <QString>

/*****************************************************************************
*
***  class DCameraMainWindow
*
*  Class to provide a base class for image capture and processing programs.
*  Immediate support for camera capture with input image and processed image
*  display.  Hooks are provided in the initialization chain for relatively
*  easy customization.
*
*  Two part construction with an Initialize function is used since derived
*  virtual functions are not called via base class constuctor.
*
*****************************************************************************/

namespace Ui
   {
   class DCameraMainWindow;
   }

class DCameraMainWindow : public DPersistentMainWindow
   {
   Q_OBJECT

   using Base = DPersistentMainWindow;

   public:
      explicit DCameraMainWindow(QWidget* pParent = nullptr);
      ~DCameraMainWindow();

      virtual void Initialize(Qt::Orientation eOrientation = Qt::Horizontal);

   protected:
      Qt::Orientation m_eOrientation;
      QSize m_ImageSize;
      DQCameraHandler* m_pCameraHandler;
      DQImage m_CapturedImage;
      DImageWidget* m_pInputImageWidget;
      DImageWidget* m_pOutputImageWidget;
      bool m_bOutputImageWidget;
      QGridLayout* m_pMainGrid;
      QVector<DImageWidget*> m_ImageWidgets;

      QString m_strInputImage;
      QString m_strOutputImage;
      QString m_strBothImages;

      // File actions
      QAction* m_pActionOpenImage;
      QAction* m_pActionSaveInputImage;
      QAction* m_pActionSaveInputImageAs;
      QAction* m_pActionExit;

      // Tool actions
      QAction* m_pActionClearROIs;
      QAction* m_pActionProcessImage;

      QMenu* m_pFileMenu;
      QMenu* m_pToolMenu;
      QMenu* m_pViewMenu;

      Ui::DCameraMainWindow* m_pUI;

      QMenuBar* GetMenuBar();

      virtual void SetupCentralWidget();
      virtual void CreateFileActions();
      virtual QMenu* AddFileMenu();
      virtual QMenu* AddToolsMenu();
      virtual QMenu* AddViewMenu();
      virtual QToolBar* AddFileToolBar();
      virtual QToolBar* AddToolsToolBar();
      virtual void ConnectToolActions();
      virtual void ConnectImageWidgets();

   protected slots:
      void ImageCaptured(int nID, const QImage& Image);
      virtual void CameraStarted();
      virtual void CameraStopped();
      virtual void ProcessImage(const DQImage& Image);
      virtual void ReprocessImage();
      virtual void OpenImage();
      virtual void SaveInputImage();
      virtual void SaveInputImageAs();
      virtual void UpdateFileActions();

   private:

   }; // end of class DCameraMainWindow

#endif // DCAMERAMAINWINDOW_H

