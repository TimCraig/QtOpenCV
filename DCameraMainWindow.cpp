/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include "DCameraMainWindow.h"
#include "ui_DCameraMainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>

/*****************************************************************************
 ***  class DCameraMainWindow
 *****************************************************************************/

/*****************************************************************************
 *
 ***  DCameraMainWindow::DCameraMainWindow
 *
 *****************************************************************************/

DCameraMainWindow::DCameraMainWindow(QWidget *pParent) :
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
            m_pUI(new Ui::DCameraMainWindow)
   {
   m_pUI->setupUi(this);

   m_strAppName = "DruaiCameraApp";

   return;

   } // end of DCameraMainWindow::DCameraMainWindow

/*****************************************************************************
 *
 ***  DCameraMainWindow::~DCameraMainWindow
 *
 *****************************************************************************/

DCameraMainWindow::~DCameraMainWindow()
   {
   delete m_pUI;

   m_pUI = nullptr;

   return;

   } // end of DCameraMainWindow::~DCameraMainWindow

/*****************************************************************************
*
***  DCameraMainWindow::GetMenuBar
*
****************************************************************************/

QMenuBar* DCameraMainWindow::GetMenuBar()
   {

   return (m_pUI->menuBar);

   } // end of method DCameraMainWindow::GetMenuBar

/*****************************************************************************
 *
 ***  DCameraMainWindow::Initialize
 *
 *****************************************************************************/

void DCameraMainWindow::Initialize(
      Qt::Orientation eOrientation /* = Qt::Horizontal */)
   {
   Base::Initialize();

   CreateFileActions();
   m_pFileMenu = AddFileMenu();
   AddFileToolBar();

   m_eOrientation = eOrientation;
   m_pCameraHandler = new DQCameraHandler(this, m_ImageSize);
   m_pCameraHandler->AddCameraMenu(m_pUI->menuBar);
   m_pUI->mainToolBar->addSeparator();
   m_pCameraHandler->AddCameraButtons(m_pUI->mainToolBar);

   m_pToolMenu = AddToolsMenu();
   AddToolsToolBar();

   SetupCentralWidget();

   ConnectToolActions();
   ConnectImageWidgets();

   connect(m_pCameraHandler, SIGNAL(imageCaptured(int, const QImage&)),
         this, SLOT(ImageCaptured(int, const QImage&)));

   connect(m_pCameraHandler, SIGNAL(CameraStarted()),
         this, SLOT(CameraStarted()));
   connect(m_pCameraHandler, SIGNAL(CameraStopped()),
         this, SLOT(CameraStopped()));

   m_pCameraHandler->StartCamera();

   return;

   } // end of method DCameraMainWindow::Initialize

/*****************************************************************************
 *
 ***  DCameraMainWindow::SetupCentralWidget
 *
 * The central widget lays out its children in a grid pattern.  Most of the
 * time it will contain either one or two DImageWidgets.  In the two case, one
 * will display the input image (camera or loaded) and the other will display
 * the processed image.
 *
 *****************************************************************************/

void DCameraMainWindow::SetupCentralWidget()
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

   } // end of method DCameraMainWindow::SetupCentralWidget

/*****************************************************************************
*
***  DCameraMainWindow::CameraStarted
*
****************************************************************************/

void DCameraMainWindow::CameraStarted()
   {
   UpdateFileActions();
   m_pInputImageWidget->DisableRubberBand();

   return;

   } // end of method DCameraMainWindow::CameraStarted

/*****************************************************************************
*
***  DCameraMainWindow::CameraStopped
*
****************************************************************************/

void DCameraMainWindow::CameraStopped()
   {
   UpdateFileActions();
   m_pInputImageWidget->EnableRubberBand();

   return;

   } // end of method DCameraMainWindow::CameraStopped

/*****************************************************************************
*
***  DCameraMainWindow::CreateFileActions
*
****************************************************************************/

void DCameraMainWindow::CreateFileActions()
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

   } // end of method DCameraMainWindow::CreateFileActions

/*****************************************************************************
*
***  DCameraMainWindow::AddFileMenu
*
****************************************************************************/

QMenu* DCameraMainWindow::AddFileMenu()
   {
   QMenu* pFileMenu = m_pUI->menuBar->addMenu(tr("&File"));

   pFileMenu->addAction(m_pActionOpenImage);
   pFileMenu->addAction(m_pActionSaveInputImage);
   pFileMenu->addAction(m_pActionSaveInputImageAs);

   pFileMenu->addSeparator();

   pFileMenu->addAction(m_pActionExit);

   return (pFileMenu);

   } // end of method DCameraMainWindow::AddFileMenu

/*****************************************************************************
 *
 ***  DCameraMainWindow::AddToolsMenu
 *
 *****************************************************************************/

QMenu* DCameraMainWindow::AddToolsMenu()
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

   } // end of method DCameraMainWindow::AddToolsMenu

/*****************************************************************************
 *
 ***  DCameraMainWindow::AddViewMenu
 *
 *****************************************************************************/

QMenu* DCameraMainWindow::AddViewMenu()
   {
   QMenu* pViewMenu = m_pUI->menuBar->addMenu(tr("&View"));

   return (pViewMenu);

   } // end of method DCameraMainWindow::AddViewMenu

/*****************************************************************************
*
***  DCameraMainWindow::AddFileToolBar
*
****************************************************************************/

QToolBar* DCameraMainWindow::AddFileToolBar()
   {
   QToolBar* pFileToolBar = m_pUI->mainToolBar;
   pFileToolBar->addAction(m_pActionOpenImage);
   pFileToolBar->addAction(m_pActionSaveInputImage);
   pFileToolBar->addAction(m_pActionSaveInputImageAs);

   pFileToolBar->addSeparator();

   pFileToolBar->addAction(m_pActionExit);

   return (pFileToolBar);

   } // end of method DCameraMainWindow::AddFileToolBar

/******************************************************************************
*
***  DCameraMainWindow::AddToolsToolBar
*
******************************************************************************/

QToolBar* DCameraMainWindow::AddToolsToolBar()
   {
   QToolBar* pToolsToolBar = m_pUI->mainToolBar;

   pToolsToolBar->addSeparator();

   pToolsToolBar->addAction(m_pActionProcessImage);

   return (pToolsToolBar);

   } // end of method DCameraMainWindow::AddToolsToolBar

/*****************************************************************************
*
***  DCameraMainWindow::UpdateFileActions
*
****************************************************************************/

void DCameraMainWindow::UpdateFileActions()
   {
   bool bEnable = !m_pCameraHandler->IsRunning();
   m_pActionOpenImage->setEnabled(bEnable);
   m_pActionSaveInputImage->setEnabled(bEnable);
   m_pActionSaveInputImageAs->setEnabled(bEnable);
   m_pActionProcessImage->setEnabled(bEnable);

   return;

   } // end of method DCameraMainWindow::UpdateFileActions

/*****************************************************************************
 *
 ***  DCameraMainWindow::ConnectToolActions
 *
 * Connect the Tool Menu Actions to their functions.
 *
 *****************************************************************************/

void DCameraMainWindow::ConnectToolActions()
   {
   // Connect the Clear ROI menu action to each of the image widgets
   foreach(DImageWidget * pIW, m_ImageWidgets)
      {
      connect(m_pActionClearROIs, SIGNAL(triggered()), pIW, SLOT(ClearROI()));
      } // end foreach

   connect(m_pActionProcessImage, SIGNAL(triggered()), SLOT(ReprocessImage()));

   return;

   } // end of method DCameraMainWindow::ConnectToolActions

/*****************************************************************************
 *
 ***  DCameraMainWindow::ConnectImageWidgets
 *
 * Connect the image widgets so when the ROI of one changes, they all change
 * their ROI to match.
 *
 *****************************************************************************/

void DCameraMainWindow::ConnectImageWidgets()
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

} // end of method DCameraMainWindow::ConnectImageWidgets

/*****************************************************************************
 *
 ***  DCameraMainWindow::ImageCaptured
 *
 *****************************************************************************/

void DCameraMainWindow::ImageCaptured(int /* nID */, const QImage &Image)
   {
   m_CapturedImage = Image.scaled(m_ImageSize);
   ProcessImage(m_CapturedImage);

   return;

   } // end of method DCameraMainWindow::ImageCaptured

/******************************************************************************
*
***  DCameraMainWindow::ReprocessImage
*
******************************************************************************/

void DCameraMainWindow::ReprocessImage()
   {
   ProcessImage(m_CapturedImage);

   return;

   } // end of method DCameraMainWindow::ReprocessImage

/*****************************************************************************
 *
 ***  DCameraMainWindow::ProcessImage
 *
 *****************************************************************************/

void DCameraMainWindow::ProcessImage(const DQImage& Image)
   {
   m_pInputImageWidget->SetImage(Image);
   m_pOutputImageWidget->SetImage(Image);

   return;

   } // end of method DCameraMainWindow::ProcessImage

/*****************************************************************************
*
***  DCameraMainWindow::OpenImage
*
****************************************************************************/

void DCameraMainWindow::OpenImage()
   {
   QString strFileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::currentPath(),
         tr("Image files (*.jpg *.png *.bmp);;All files (*.*)"));

   if (!strFileName.isEmpty())
      {
      QImage Image;
      if (Image.load(strFileName))
         {
         m_CapturedImage = Image;
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

   } // end of method DCameraMainWindow::OpenImage

/*****************************************************************************
*
***  DCameraMainWindow::SaveInputImage
*
****************************************************************************/

void DCameraMainWindow::SaveInputImage()
   {
   if (!m_strInputImage.isEmpty())
      {
      m_CapturedImage.save(m_strInputImage, nullptr, 100);
      } // end if
   else
      {
      SaveInputImageAs();
      } // end else

   return;

   } // end of method DCameraMainWindow::SaveInputImage

/*****************************************************************************
*
***  DCameraMainWindow::SaveInputImageAs
*
****************************************************************************/

void DCameraMainWindow::SaveInputImageAs()
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

   } // end of method DCameraMainWindow::SaveInputImageAs




