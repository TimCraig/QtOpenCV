/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DCVCameraMainWindow.h"
#include "ui_DCVCameraMainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>

#include "DQCVImageUtils.h"

/*****************************************************************************
 ***  class DCVCameraMainWindow
 *****************************************************************************/

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::DCVCameraMainWindow
 *
 *****************************************************************************/

DCVCameraMainWindow::DCVCameraMainWindow(QWidget *pParent) :
      Base(pParent),
            m_eOrientation(Qt::Horizontal),
            m_ImageSize(320, 240),
            m_pInputImageWidget(nullptr),
            m_pOutputImageWidget(nullptr),
            m_bOutputImageWidget(true),
            m_pMainGrid(nullptr),
            m_pActionOpenImage(nullptr),
            m_pActionSaveInputImage(nullptr),
            m_pActionSaveInputImageAs(nullptr),
            m_pActionExit(nullptr),
            m_pActionClearROIs(nullptr),
            m_pActionProcessImage(nullptr),
            m_pFileMenu(nullptr),
            m_pToolMenu(nullptr),
            m_pViewMenu(nullptr),
            m_pUI(new Ui::DCVCameraMainWindow)
   {
   m_pUI->setupUi(this);

   m_strAppName = "DruaiCameraApp";

   return;

   } // end of DCVCameraMainWindow::DCVCameraMainWindow

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::~DCVCameraMainWindow
 *
 *****************************************************************************/

DCVCameraMainWindow::~DCVCameraMainWindow()
   {
   delete m_pUI;

   m_pUI = nullptr;

   return;

   } // end of DCVCameraMainWindow::~DCVCameraMainWindow

/*****************************************************************************
*
***  DCVCameraMainWindow::GetMenuBar
*
****************************************************************************/

QMenuBar* DCVCameraMainWindow::GetMenuBar()
   {

   return (m_pUI->menuBar);

   } // end of method DCVCameraMainWindow::GetMenuBar

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::Initialize
 *
 *****************************************************************************/

void DCVCameraMainWindow::Initialize(
      Qt::Orientation eOrientation /* = Qt::Horizontal */)
   {
   Base::Initialize();

   CreateFileActions();
   m_pFileMenu = AddFileMenu();
   AddFileToolBar();

   m_eOrientation = eOrientation;
   m_pCameraHandler = new DCVCameraHandler(this, m_ImageSize);
   m_pCameraHandler->AddCameraMenu(m_pUI->menuBar);
   m_pUI->mainToolBar->addSeparator();
   m_pCameraHandler->AddCameraButtons(m_pUI->mainToolBar);

   m_pToolMenu = AddToolsMenu();
   AddToolsToolBar();

   SetupCentralWidget();

   ConnectToolActions();
   ConnectImageWidgets();

   connect(m_pCameraHandler, SIGNAL(imageCaptured(DCVImage&)),
         this, SLOT(ImageCaptured(DCVImage&)));

   connect(m_pCameraHandler, SIGNAL(CameraStarted()),
         this, SLOT(CameraStarted()));
   connect(m_pCameraHandler, SIGNAL(CameraStopped()),
         this, SLOT(CameraStopped()));

   // Don't start camera here, descendant classes my not have finished initializing.
//   m_pCameraHandler->StartCamera();

   return;

   } // end of method DCVCameraMainWindow::Initialize

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::SetupCentralWidget
 *
 * The central widget lays out its children in a grid pattern.  Most of the
 * time it will contain either one or two DImageWidgets.  In the two case, one
 * will display the input image (camera or loaded) and the other will display
 * the processed image.
 *
 *****************************************************************************/

void DCVCameraMainWindow::SetupCentralWidget()
   {
   Base::SetupCentralWidget();

   m_pInputImageWidget = new DImageWidget();
   m_pInputImageWidget->setFixedSize(m_ImageSize);

   m_pMainGrid = new QGridLayout();
   m_pMainGrid->addWidget(m_pInputImageWidget, 0, 0, Qt::AlignHCenter);

   if (m_bOutputImageWidget)
      {
      m_pOutputImageWidget = new DImageWidget();
      m_pOutputImageWidget->setFixedSize(m_ImageSize);

      m_ImageWidgets.push_back(m_pInputImageWidget);
      m_ImageWidgets.push_back(m_pOutputImageWidget);


      if (m_eOrientation == Qt::Horizontal)
         {
         m_pMainGrid->addWidget(m_pOutputImageWidget, 0, 1, Qt::AlignHCenter);
         } // end if
      else if (m_eOrientation == Qt::Vertical)
         {
         m_pMainGrid->addWidget(m_pOutputImageWidget, 1, 0, Qt::AlignHCenter);
         } // end else if
      } // end if

   m_pUI->centralWidget->setLayout(m_pMainGrid);

   return;

   } // end of method DCVCameraMainWindow::SetupCentralWidget

/*****************************************************************************
*
***  DCVCameraMainWindow::CameraStarted
*
****************************************************************************/

void DCVCameraMainWindow::CameraStarted()
   {
   UpdateFileActions();
   m_pInputImageWidget->DisableRubberBand();

   return;

   } // end of method DCVCameraMainWindow::CameraStarted

/*****************************************************************************
*
***  DCVCameraMainWindow::CameraStopped
*
****************************************************************************/

void DCVCameraMainWindow::CameraStopped()
   {
   UpdateFileActions();
   m_pInputImageWidget->EnableRubberBand();

   return;

   } // end of method DCVCameraMainWindow::CameraStopped

/*****************************************************************************
*
***  DCVCameraMainWindow::CreateFileActions
*
****************************************************************************/

void DCVCameraMainWindow::CreateFileActions()
   {
   m_pActionOpenImage = new QAction(QIcon(":/images/fileopen.png"),
         tr("Open Image"), this);
   m_pActionOpenImage->setShortcut(tr("Ctrl+O"));
   m_pActionOpenImage->setStatusTip(tr("Open an Image File"));

   connect(m_pActionOpenImage, SIGNAL(triggered()), this, SLOT(OpenImage()));

   m_pActionSaveInputImage = new QAction(QIcon(":/images/Save2.png"),
         tr("Save Input Image"), this);
   m_pActionSaveInputImage->setShortcut(tr("Ctrl+S"));
   m_pActionSaveInputImage->setStatusTip(tr("Save the Input Image"));

   connect(m_pActionSaveInputImage, SIGNAL(triggered()), this, SLOT(SaveInputImage()));

   m_pActionSaveInputImageAs = new QAction(QIcon(":/images/Save_As.png"),
         tr("Save Input Image As..."), this);
   m_pActionSaveInputImageAs->setStatusTip(
         tr("Save the Input Image with a different filename"));

   connect(m_pActionSaveInputImageAs, SIGNAL(triggered()), this, SLOT(SaveInputImageAs()));

   m_pActionExit = new QAction(QIcon(":/images/EXIT2.png"),
         tr("Exit"), this);
   m_pActionExit->setShortcut(tr("Ctrl+Q"));
   m_pActionExit->setStatusTip(tr("Exit the Application"));

   connect(m_pActionExit, SIGNAL(triggered()), this, SLOT(close()));

   return;

   } // end of method DCVCameraMainWindow::CreateFileActions

/*****************************************************************************
*
***  DCVCameraMainWindow::AddFileMenu
*
****************************************************************************/

QMenu* DCVCameraMainWindow::AddFileMenu()
   {
   QMenu* pFileMenu = m_pUI->menuBar->addMenu(tr("&File"));

   pFileMenu->addAction(m_pActionOpenImage);
   pFileMenu->addAction(m_pActionSaveInputImage);
   pFileMenu->addAction(m_pActionSaveInputImageAs);

   pFileMenu->addSeparator();

   pFileMenu->addAction(m_pActionExit);

   return (pFileMenu);

   } // end of method DCVCameraMainWindow::AddFileMenu

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::AddToolsMenu
 *
 *****************************************************************************/

QMenu* DCVCameraMainWindow::AddToolsMenu()
   {
   QMenu* pToolsMenu = m_pUI->menuBar->addMenu(tr("&Tools"));

   m_pActionClearROIs = new QAction(tr("Clear ROIs"), this);
   m_pActionClearROIs->setStatusTip(tr("Clear Image Regions of Interest"));
   pToolsMenu->addAction(m_pActionClearROIs);

   m_pActionProcessImage = new QAction(QIcon(":/images/Redo.png"), tr("Process Imgage"), this);
   m_pActionProcessImage->setStatusTip(
         tr("Manually Trigger Processing the Current Image"));
   pToolsMenu->addAction(m_pActionProcessImage);

   return (pToolsMenu);

   } // end of method DCVCameraMainWindow::AddToolsMenu

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::AddViewMenu
 *
 *****************************************************************************/

QMenu* DCVCameraMainWindow::AddViewMenu()
   {
   QMenu* pViewMenu = m_pUI->menuBar->addMenu(tr("&View"));

   return (pViewMenu);

   } // end of method DCVCameraMainWindow::AddViewMenu

/*****************************************************************************
*
***  DCVCameraMainWindow::AddFileToolBar
*
****************************************************************************/

QToolBar* DCVCameraMainWindow::AddFileToolBar()
   {
   QToolBar* pFileToolBar = m_pUI->mainToolBar;
   pFileToolBar->addAction(m_pActionOpenImage);
   pFileToolBar->addAction(m_pActionSaveInputImage);
   pFileToolBar->addAction(m_pActionSaveInputImageAs);

   pFileToolBar->addSeparator();

   pFileToolBar->addAction(m_pActionExit);

   return (pFileToolBar);

   } // end of method DCVCameraMainWindow::AddFileToolBar

/******************************************************************************
*
***  DCVCameraMainWindow::AddToolsToolBar
*
******************************************************************************/

QToolBar* DCVCameraMainWindow::AddToolsToolBar()
   {
   QToolBar* pToolsToolBar = m_pUI->mainToolBar;

   pToolsToolBar->addSeparator();

   pToolsToolBar->addAction(m_pActionProcessImage);

   return (pToolsToolBar);

   } // end of method DCVCameraMainWindow::AddToolsToolBar

/*****************************************************************************
*
***  DCVCameraMainWindow::UpdateFileActions
*
****************************************************************************/

void DCVCameraMainWindow::UpdateFileActions()
   {
   bool bEnable = !m_pCameraHandler->IsRunning();
   m_pActionOpenImage->setEnabled(bEnable);
   m_pActionSaveInputImage->setEnabled(bEnable);
   m_pActionSaveInputImageAs->setEnabled(bEnable);
   m_pActionProcessImage->setEnabled(bEnable);

   return;

   } // end of method DCVCameraMainWindow::UpdateFileActions

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::ConnectToolActions
 *
 * Connect the Tool Menu Actions to their functions.
 *
 *****************************************************************************/

void DCVCameraMainWindow::ConnectToolActions()
   {
   // Connect the Clear ROI menu action to each of the image widgets
   foreach(DImageWidget * pIW, m_ImageWidgets)
      {
      connect(m_pActionClearROIs, SIGNAL(triggered()), pIW, SLOT(ClearROI()));
      } // end foreach

   connect(m_pActionProcessImage, SIGNAL(triggered()), SLOT(ReprocessImage()));

   return;

   } // end of method DCVCameraMainWindow::ConnectToolActions

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::ConnectImageWidgets
 *
 * Connect the image widgets so when the ROI of one changes, they all change
 * their ROI to match.
 *
 *****************************************************************************/

void DCVCameraMainWindow::ConnectImageWidgets()
   {
   foreach (DImageWidget* pSignaler, m_ImageWidgets)
      {
      foreach(DImageWidget* pSlot, m_ImageWidgets)
         {
         if (pSignaler != pSlot)
            {
            connect(pSignaler, SIGNAL(ROIChanged(const QRect&)), pSlot, SLOT(SetROI(const QRect&)));
            } // end if
      } // end foreach
   } // end foreach

return;

} // end of method DCVCameraMainWindow::ConnectImageWidgets

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::ImageCaptured
 *
 *****************************************************************************/

void DCVCameraMainWindow::ImageCaptured(DCVImage &Image)
   {
   m_CapturedImage = Image;
   ProcessImage(m_CapturedImage);

   return;

   } // end of method DCVCameraMainWindow::ImageCaptured

/******************************************************************************
*
***  DCVCameraMainWindow::ReprocessImage
*
******************************************************************************/

void DCVCameraMainWindow::ReprocessImage()
   {
   ProcessImage(m_CapturedImage);

   return;

   } // end of method DCVCameraMainWindow::ReprocessImage

/*****************************************************************************
 *
 ***  DCVCameraMainWindow::ProcessImage
 *
 *****************************************************************************/

void DCVCameraMainWindow::ProcessImage(DCVImage& Image)
   {
   QImage Display = cvMatToQImage(Image);
   m_pInputImageWidget->SetImage(Display);
   m_pOutputImageWidget->SetImage(Display);

   return;

   } // end of method DCVCameraMainWindow::ProcessImage

/*****************************************************************************
*
***  DCVCameraMainWindow::OpenImage
*
****************************************************************************/

void DCVCameraMainWindow::OpenImage(
      int nFlags /* = DCVImage::eReadUnchanged */)
   {
   QString strFileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::currentPath(),
         tr("Image files (*.jpg *.png *.bmp);;All files (*.*)"));

   if (!strFileName.isEmpty())
      {
      if (m_CapturedImage.ReadImage(strFileName.toStdString(), nFlags))
         {
         m_strInputImage = strFileName;

         ProcessImage(m_CapturedImage);
         } // end if
      else
         {
         QString strMsg(tr("Failed to load image "));
         strMsg += strFileName;
         QMessageBox::warning(this, tr("Load Image"), strMsg);
         } // end else
      } // end if

   return;

   } // end of method DCVCameraMainWindow::OpenImage

/*****************************************************************************
*
***  DCVCameraMainWindow::SaveInputImage
*
****************************************************************************/

void DCVCameraMainWindow::SaveInputImage()
   {
   if (!m_strInputImage.isEmpty())
      {
      m_CapturedImage.WriteImage(m_strInputImage.toStdString());
      } // end if
   else
      {
      SaveInputImageAs();
      } // end else

   return;

   } // end of method DCVCameraMainWindow::SaveInputImage

/*****************************************************************************
*
***  DCVCameraMainWindow::SaveInputImageAs
*
****************************************************************************/

void DCVCameraMainWindow::SaveInputImageAs()
   {
   QString strFileName = QFileDialog::getSaveFileName(this,
         tr("Save Input Image As"), QDir::currentPath(),
         tr("Image files (*.jpg *.png *.bmp)"));
   if (!strFileName.isEmpty())
      {
      m_strInputImage = strFileName;
      SaveInputImage();
      } // end if

   return;

   } // end of method DCVCameraMainWindow::SaveInputImageAs




