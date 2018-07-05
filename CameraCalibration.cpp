/*****************************************************************************
 ******************************  I N C L U D E  ******************************
 ****************************************************************************/

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

#include "CameraCalibration.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

/*****************************************************************************
 ***  class CameraCalibrationBoard
 ****************************************************************************/

// Element serialization names
const std::string CameraCalibrationBoard::m_strID("CameraCalibrationBoard");
const std::string CameraCalibrationBoard::m_strBoardSize("BoardSize");
const std::string CameraCalibrationBoard::m_strCalibrationPattern("CalibratePattern");
const std::string CameraCalibrationBoard::m_strSquareSize("SquareSize");

/******************************************************************************
*
***  CameraCalibrationBoard::CameraCalibrationBoard
*
******************************************************************************/

CameraCalibrationBoard::CameraCalibrationBoard() :
      m_ePattern(EPattern::eChessBoard),
      m_BoardSize(10, 7),
      m_fSquareSize(1.0f)
   {

   return;

   } // end of method CameraCalibrationBoard::CameraCalibrationBoard

/******************************************************************************
*
***  CameraCalibrationBoard::CameraCalibrationBoard
*
******************************************************************************/

CameraCalibrationBoard::CameraCalibrationBoard(
      const CameraCalibrationBoard& src)
   {
   m_ePattern = src.m_ePattern;
   m_BoardSize = src.m_BoardSize;
   m_fSquareSize = src.m_fSquareSize;

   return;

   } // end of method CameraCalibrationBoard::CameraCalibrationBoard

/******************************************************************************
*
***  CameraCalibrationBoard::operator=
*
******************************************************************************/

CameraCalibrationBoard& CameraCalibrationBoard::operator=(
      const CameraCalibrationBoard& rhs)
   {
   if (this != &rhs)
      {
      m_ePattern = rhs.m_ePattern;
      m_BoardSize = rhs.m_BoardSize;
      m_fSquareSize = rhs.m_fSquareSize;
      } // end if

   return (*this);

   } // end of method CameraCalibrationBoard::operator=

/******************************************************************************
*
***  CameraCalibrationBoard::Write
*
* Write the calibration board data to the supplied OpenCV FileStorage.
*
******************************************************************************/

bool CameraCalibrationBoard::Write(cv::FileStorage& FS) const
   {
   bool bRet = true;

   FS << "{"
      << m_strCalibrationPattern << static_cast<int>(m_ePattern)
      << m_strBoardSize << m_BoardSize
      << m_strSquareSize << m_fSquareSize
      << "}";

   return (bRet);

   } // end of method CameraCalibrationBoard::Write

/******************************************************************************
*
***  CameraCalibrationBoard::Read
*
* Read the calibration board data from the OpenCV FileNode.
*
******************************************************************************/

bool CameraCalibrationBoard::Read(const cv::FileNode& Node)
   {
   bool bRet = true;

   Node[m_strBoardSize] >> m_BoardSize;
   int nPattern;
   Node[m_strCalibrationPattern] >> nPattern;
   m_ePattern = static_cast<EPattern>(nPattern);
   Node[m_strSquareSize]  >> m_fSquareSize;

   return (bRet);

   } // end of method CameraCalibrationBoard::Read

/******************************************************************************
*
***  write
*
* Global function necessary for using FileStream's streaming operator to write.
*
******************************************************************************/

void write(cv::FileStorage& FS, cv::String&,
      const CameraCalibrationBoard& X)
   {
   X.Write(FS);

   return;

   } // end of function write

/******************************************************************************
*
***  read
*
* Global function necessary for using FileStream's streaming operator to read.
*
******************************************************************************/

void read(const cv::FileNode& Node, CameraCalibrationBoard& X,
      CameraCalibrationBoard Default /* = CameraCalibrationBoard() */)
   {
   if (Node.empty())
      {
      X = Default;
      } // end if
   else
      {
      X.Read(Node);
      } // end else

   return;

   } // end of function read

/*****************************************************************************
 ***  class CameraCalibration
 ****************************************************************************/

// Element serialization names
const std::string CameraCalibration::m_strID("Calibration_Settings");
const std::string CameraCalibration::m_strBoard("Chessboard");
const std::string CameraCalibration::m_strFixAspectRatio("FixAspectRatio");
const std::string CameraCalibration::m_strFlag("ControlFlags");
const std::string CameraCalibration::m_strCameraMatrix("CameraMatrix");
const std::string CameraCalibration::m_strDistortionCoeffs("DistortionCoefficents");
const std::string CameraCalibration::m_strRVecs("RVecs");
const std::string CameraCalibration::m_strTVecs("TVecs");
const std::string CameraCalibration::m_strRMS("RMSError");
const std::string CameraCalibration::m_strReprojErrors("ReprojectionErrors");
const std::string CameraCalibration::m_strTotalAvgError("TotalAverageError");
const std::string CameraCalibration::m_strImagePoints("ImagePoints");
const std::string CameraCalibration::m_strGoodImages("NumberImagesUsed");
const std::string CameraCalibration::m_strImageSize("ImageSize");
const std::string CameraCalibration::m_strSaveImages("SaveImages");

/******************************************************************************
*
***  CameraCalibration::CameraCalibration
*
******************************************************************************/

CameraCalibration::CameraCalibration() :
      m_ImageSize(640, 480),
      m_bFixAspectRatio(false),
      m_nFlag(0),
      m_dRMS(0.0),
      m_dTotalAvgError(0.0),
      m_nGoodImages(0),
      m_bSaveImages(true)
   {
   m_CameraMatrix = cv::Mat::eye(3, 3, CV_64F);
   m_DistortionCoeffs = cv::Mat::zeros(8, 1, CV_64F);

   return;

   } // end of method CameraCalibration::CameraCalibration

/******************************************************************************
*
***  CameraCalibration::CameraCalibration
*
******************************************************************************/

CameraCalibration::CameraCalibration(const CameraCalibration& src)
   {
   m_Board = src.m_Board;
   m_bFixAspectRatio = src.m_bFixAspectRatio;
   m_nFlag = src.m_nFlag;
   m_ImageSize = src. m_ImageSize;
   m_CameraMatrix = src.m_CameraMatrix;
   m_DistortionCoeffs = src.m_DistortionCoeffs;
   m_TVecs = src.m_TVecs;
   m_RVecs = src.m_RVecs;
   m_dRMS = src.m_dRMS;
   m_dTotalAvgError = src. m_dTotalAvgError;
   m_ReprojErrors = src.m_ReprojErrors;
   m_ImagePoints = src.m_ImagePoints;
   m_Images = src.m_Images;
   m_nGoodImages = src.m_nGoodImages;
   m_bSaveImages = src.m_bSaveImages;

   return;

   } // end of method CameraCalibration::CameraCalibration

/******************************************************************************
*
***  CameraCalibration::operator=
*
******************************************************************************/

CameraCalibration& CameraCalibration::operator=(const CameraCalibration& rhs)
   {
   if (this != &rhs)
      {
      m_Board = rhs.m_Board;
      m_bFixAspectRatio = rhs.m_bFixAspectRatio;
      m_nFlag = rhs.m_nFlag;
      m_ImageSize = rhs. m_ImageSize;
      m_CameraMatrix = rhs.m_CameraMatrix;
      m_DistortionCoeffs = rhs.m_DistortionCoeffs;
      m_TVecs = rhs.m_TVecs;
      m_RVecs = rhs.m_RVecs;
      m_dRMS = rhs.m_dRMS;
      m_dTotalAvgError = rhs. m_dTotalAvgError;
      m_ReprojErrors = rhs.m_ReprojErrors;
      m_ImagePoints = rhs.m_ImagePoints;
      m_Images = rhs.m_Images;
      m_nGoodImages = rhs.m_nGoodImages;
      m_bSaveImages = rhs.m_bSaveImages;
      } // end if

   return (*this);

   } // end of method CameraCalibration::operator=

/******************************************************************************
*
***  CameraCalibration::Initialize
*
* Initialize the control elements and clear the calculated members as a prep
* for running a new calibration.
*
******************************************************************************/

bool CameraCalibration::Initialize(const CameraCalibrationBoard& Board,
      const cv::Size& ImageSize, bool bFixAspectRatio, int nFlag,
      bool bSaveImages /* = false */)
   {
   bool bRet = true;

   m_nGoodImages = 0;
   m_RVecs.clear();
   m_TVecs.clear();
   m_ReprojErrors.clear();
   m_ImagePoints.clear();
   m_Images.clear();
   m_dRMS = 0.0;
   m_dTotalAvgError = 0.0;

   m_CameraMatrix = cv::Mat::eye(3, 3, CV_64F);
   m_DistortionCoeffs = cv::Mat::zeros(8, 1, CV_64F);

   m_Board = Board;
   m_bFixAspectRatio = bFixAspectRatio;
   m_nFlag = nFlag;
   m_ImageSize = ImageSize;
   m_bSaveImages = bSaveImages;

   return (bRet);

   } // end of method CameraCalibration::Initialize

/******************************************************************************
*
***  CameraCalibration::ProcessImage
*
* Takes an "original" image and the gray conversion of that image.  Allows the
* calling program to do any preprocessing desired on both images.
*
* bUseImage indicates whether to use the image in the calibration.  Some
* programs may want to preview live images to the user with annotation.
*
******************************************************************************/

bool CameraCalibration::ProcessImage(cv::Mat& Image, cv::Mat& GrayImage,
      bool bAnnotateImage, bool bUseImage)
   {
   std::vector<cv::Point2f> Points;

   // Find feature points on the input format
   bool bFound = false;
   switch (m_Board.m_ePattern)
      {
      case CameraCalibrationBoard::EPattern::eChessBoard:
         bFound = cv::findChessboardCorners(Image, m_Board.m_BoardSize,
               Points, CV_CALIB_CB_ADAPTIVE_THRESH |
               CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
         break;

      case CameraCalibrationBoard::EPattern::eCirclesGrid:
         bFound = cv::findCirclesGrid(Image, m_Board.m_BoardSize, Points);
         break;

      case CameraCalibrationBoard::EPattern::eAsymmetricCirclesGrid:
         bFound = cv::findCirclesGrid(Image, m_Board.m_BoardSize,
               Points, cv::CALIB_CB_ASYMMETRIC_GRID);
         break;

      default:
         break;
      } // end switch

   if (bFound)
      {
      // improve the bFound corners' coordinate accuracy for chessboard
      if (m_Board.m_ePattern == CameraCalibrationBoard::EPattern::eChessBoard)
         {
         cv::cornerSubPix(GrayImage, Points, cv::Size(11,11),
               cv::Size(-1,-1),
               cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
         } // end if

      if (bUseImage)
         {
         // Include this image in the calibration calculation
         m_ImagePoints.push_back(Points);
         m_nGoodImages++;

         // Save the images for future reprocessing before being annotated
         if (m_bSaveImages)
            {
            m_Images.push_back(Image.clone());
            } // end if
         } // end if

      if (bAnnotateImage)
         {
         // Draw the corners.
         cv::drawChessboardCorners(Image, m_Board.m_BoardSize,
               cv::Mat(Points), bFound);
         } // end if
      } // end if

   return (bFound);

   } // end of method CameraCalibration::ProcessImage

/******************************************************************************
*
***  CameraCalibration::CalcBoardCornerPositions
*
* Produce a vector of 3D point coordinates for the board corners based on the
* pattern and specified size.
*
******************************************************************************/

void CameraCalibration::CalcBoardCornerPositions(
      std::vector<cv::Point3f>& Corners)
   {
   Corners.clear();

   switch (m_Board.m_ePattern)
      {
      case CameraCalibrationBoard::EPattern::eChessBoard:
      case CameraCalibrationBoard::EPattern::eCirclesGrid:
         for (int i = 0 ; i < m_Board.m_BoardSize.height ; i++)
            {
            for (int j = 0 ; j < m_Board.m_BoardSize.width ; j++)
               {
               Corners.push_back(cv::Point3f((j * m_Board.m_fSquareSize), (i * m_Board.m_fSquareSize), 0.0f));
               } // end for
            } // end for

         break;

      case CameraCalibrationBoard::EPattern::eAsymmetricCirclesGrid:
         for (int i = 0 ; i < m_Board.m_BoardSize.height ; i++)
            {
            for (int j = 0 ; j < m_Board.m_BoardSize.width ; j++)
               {
               Corners.push_back(cv::Point3f((((2 * j) + (i % 2)) * m_Board.m_fSquareSize),
                     (i * m_Board.m_fSquareSize), 0.0f));
               } // end for
            } // end for

         break;

      default:
         break;
      }

   return;

   } // end of method CameraCalibration::CalcBoardCornerPositions

/******************************************************************************
*
***  CameraCalibration::RunCalibration
*
* Perform the actual calibration calculations.
*
******************************************************************************/

bool CameraCalibration::RunCalibration()
   {
   m_CameraMatrix = cv::Mat::eye(3, 3, CV_64F);
   if (m_bFixAspectRatio)
      {
      m_CameraMatrix.at<double>(0,0) = 1.0;
      } // end if

   m_DistortionCoeffs = cv::Mat::zeros(8, 1, CV_64F);

   std::vector<std::vector<cv::Point3f>> ObjectPoints(1);
   CalcBoardCornerPositions(ObjectPoints[0]);

   ObjectPoints.resize(m_ImagePoints.size(), ObjectPoints[0]);

   //Find intrinsic and extrinsic camera parameters
   m_dRMS = cv::calibrateCamera(ObjectPoints, m_ImagePoints, m_ImageSize,
         m_CameraMatrix, m_DistortionCoeffs,
         m_RVecs, m_TVecs, m_nFlag);

//   cout << "Re-projection error reported by calibrateCamera: "<< rms << endl;

   bool bGood = checkRange(m_CameraMatrix) && checkRange(m_DistortionCoeffs);

   m_dTotalAvgError = ComputeReprojectionErrors(ObjectPoints);

   return (bGood);

   } // end of method CameraCalibration::RunCalibration

/******************************************************************************
*
***  CameraCalibration::ComputeReprojectionErrors
*
******************************************************************************/

double CameraCalibration::ComputeReprojectionErrors(
      const std::vector<std::vector<cv::Point3f> >& ObjectPoints)
   {
   std::vector<cv::Point2f> ImagePoints2;
   int nTotalPoints = 0;
   double dTotalErr = 0.0;
   m_ReprojErrors.resize(ObjectPoints.size());

   for (size_t i = 0 ; i < ObjectPoints.size() ; i++)
      {
      cv::projectPoints(cv::Mat(ObjectPoints[i]), m_RVecs[i], m_TVecs[i],
            m_CameraMatrix, m_DistortionCoeffs, ImagePoints2);
      double dErr = cv::norm(cv::Mat(m_ImagePoints[i]), cv::Mat(ImagePoints2), CV_L2);

      int n = static_cast<int>(ObjectPoints[i].size());
      double dErr2 = dErr * dErr;
      m_ReprojErrors[i] = std::sqrt(dErr2 / n);
      dTotalErr += dErr2;
      nTotalPoints += n;
      } // end for

   return (std::sqrt(dTotalErr / nTotalPoints));

   } // end of method CameraCalibration::ComputeReprojectionErrors

/******************************************************************************
*
***  CameraCalibration::Write
*
* Write the calibration to file using OpenCV's FileStorage system.  Produces
* either XML or YAML formats based on the supplied name extension (.xml, .yml).
*
******************************************************************************/

bool CameraCalibration::Write(const std::string& strFileName) const
   {
   cv::FileStorage FS(strFileName, cv::FileStorage::WRITE);
   bool bRet = FS.isOpened();
   if (bRet)
      {
      bRet = Write(FS);
      } // end if

   if (m_bSaveImages)
      {
      // Strip off the extension
      boost::filesystem::path Path(strFileName);
      boost::filesystem::path Dir(Path.parent_path());
      boost::filesystem::path File = Path.stem();
      Dir /= File;

      WriteImages(Dir.string());
      } // end if

   return (bRet);

   } // end of method CameraCalibration::Write

/******************************************************************************
*
***  CameraCalibration::Write
*
* Actual OpenCV FileStage write file persistence.
*
******************************************************************************/

bool CameraCalibration::Write(cv::FileStorage& FS) const
   {
   bool bRet = true;

   FS << m_strID << "{"
      << m_strBoard << m_Board
      << m_strFixAspectRatio << m_bFixAspectRatio
      << m_strFlag << m_nFlag
      << m_strImageSize << m_ImageSize
      << m_strCameraMatrix << m_CameraMatrix
      << m_strDistortionCoeffs << m_DistortionCoeffs
      << m_strRVecs << m_RVecs
      << m_strTVecs << m_TVecs
      << m_strRMS << m_dRMS
      << m_strReprojErrors << m_ReprojErrors
      << m_strTotalAvgError << m_dTotalAvgError
      << m_strImagePoints << m_ImagePoints
      << m_strGoodImages << m_nGoodImages
      << "}";

   return (bRet);

   } // end of method CameraCalibration::Write

/******************************************************************************
*
***  CameraCalibration::Read
*
* Read calibration data from file into memory using OpenCV's FileStorage
* mechanism.
*
******************************************************************************/

bool CameraCalibration::Read(const cv::FileNode& Node)
   {
   bool bRet = true;

   Node[m_strBoard] >> m_Board;
   Node[m_strFixAspectRatio] >> m_bFixAspectRatio;
   Node[m_strFlag] >> m_nFlag;
   Node[m_strImageSize] >> m_ImageSize;
   Node[m_strCameraMatrix] >> m_CameraMatrix;
   Node[m_strDistortionCoeffs] >> m_DistortionCoeffs;
   Node[m_strRVecs] >> m_RVecs;
   Node[m_strTVecs] >> m_TVecs;
   Node[m_strRMS] >> m_dRMS;
   Node[m_strReprojErrors] >> m_ReprojErrors;
   Node[m_strTotalAvgError] >> m_dTotalAvgError;
   Node[m_strImagePoints] >> m_ImagePoints;
   Node[m_strGoodImages] >> m_nGoodImages;

   return (bRet);

   } // end of method CameraCalibration::Read

/******************************************************************************
*
***  CameraCalibration::WriteImages
*
* Write the images used to calculate the calibration to individual sequentially
* numbered files.  The extension determines the file type and the format
* parameters are the OpenCV defaults for that format.
*
******************************************************************************/

bool CameraCalibration::WriteImages(const std::string& strBaseFileName,
      const std::string& strExtension /* = std::string("png" */) const
   {
   bool bRet = true;

   int i = 0;
   for (const auto& Image : m_Images)
      {
      std::string strCounter(boost::lexical_cast<std::string>(i));
      std::string strImageFile = strBaseFileName + "-" + strCounter + "."
            + strExtension;

      cv::imwrite(strImageFile, Image);

      i++;
      } // end for

   return (bRet);

   } // end of method CameraCalibration::WriteImages

/******************************************************************************
*
***  CameraCalibration::ReadCameraMatrix
*
* When the user is only interested in the Camera Matrix from the calibration.
*
******************************************************************************/

bool CameraCalibration::ReadCameraMatrix(const cv::FileStorage& FS,
      cv::Mat& CameraMatrix)
   {
   bool bRet = FS.isOpened();
   if (bRet)
      {
      cv::FileNode Node = FS[m_strID];
      Node[m_strCameraMatrix] >> CameraMatrix;
      } // end if

   return (bRet);

   } // end of method CameraCalibration::ReadCameraMatrix

/******************************************************************************
*
***  CameraCalibration::ReadDistortionCoeffs
*
******************************************************************************/

bool CameraCalibration::ReadDistortionCoeffs(const cv::FileNode& Node,
      cv::Mat& DistortionCoeffs)
   {
   bool bRet = true;

   Node[m_strDistortionCoeffs] >> DistortionCoeffs;

   return (bRet);

   } // end of method CameraCalibration::ReadDistortionCoeffs
