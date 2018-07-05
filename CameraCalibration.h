#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

/* This file contains classes to simplify and integrate the OpenCV funtions for doing camera calibration.
 * These provide a standard basis for applications performing a calibration along with methods for perisistent
 * storage in standard file formats and methods for their serialization.
 *
 */

/*****************************************************************************
******************************  I N C L U D E  ******************************
****************************************************************************/

#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
//#include <opencv2/core/operations.hpp>

/*****************************************************************************
 *
 ***  class CameraCalibrationBoard
 *
 * Characterize the board used for the camera calibration.
 *
 *****************************************************************************/

class CameraCalibrationBoard
   {
   public:
      // OpenCV provides for detecting these kinds of calibration patterns.
      enum class EPattern { eChessBoard, eCirclesGrid, eAsymmetricCirclesGrid };

      // Type of calbration pattern
      EPattern m_ePattern;
      // Interior width and hight of board in squares
      cv::Size m_BoardSize;
      // Board square size in user units
      float m_fSquareSize;

      CameraCalibrationBoard();
      CameraCalibrationBoard(const CameraCalibrationBoard& src);

      ~CameraCalibrationBoard() = default;

      CameraCalibrationBoard& operator=(const CameraCalibrationBoard& rhs);

      bool Write(cv::FileStorage& FS) const;
      bool Read(const cv::FileNode& Node);
      bool Read(const cv::FileStorage& FS)
         {
         cv::FileNode Node = FS[m_strID];
         return (Read(Node));
         }

   protected:
      // Storage Names for serialization
      static const std::string m_strID;
      static const std::string m_strBoardSize;
      static const std::string m_strCalibrationPattern;
      static const std::string m_strSquareSize;

   private:

   }; // end of class CameraCalibrationBoard

// Global functions for OpenCV FileStorage streaming
void write(cv::FileStorage& FS, cv::String&, const CameraCalibrationBoard& X);
void read(const cv::FileNode& Node, CameraCalibrationBoard& X,
          CameraCalibrationBoard Default = CameraCalibrationBoard());

/*****************************************************************************
 *
 ***  class CameraCalibration
 *
 * This class provides methods for analyzing camera images to extract
 * calibration data, process the data to perform the calibration, and provide
 * serialization methods for storing the data in files.  The current file
 * formats supported are XML and YML via OpenCV's cv::FileStorage class.
 *
 *****************************************************************************/

class CameraCalibration
   {
   public:
      CameraCalibration();
      CameraCalibration(const CameraCalibration& src);

      ~CameraCalibration() = default;

      CameraCalibration& operator=(const CameraCalibration& rhs);

      int GetNumGoodImages() const
         {
         return (m_nGoodImages);
         }

      bool Initialize(const CameraCalibrationBoard& Board,
            const cv::Size& ImageSize, bool bFixAspectRatio,
            int nFlag,bool bSaveImages = false);

      bool ProcessImage(cv::Mat &Image, cv::Mat& GrayImage,
            bool bAnnotateImage, bool bUseImage);
      bool RunCalibration();

      // Serialization functions
      bool Write(cv::FileStorage& FS) const;
      bool Write(const std::string& strFileName) const;
      bool Read(const cv::FileNode& Node);
      bool Read(const std::string& strFileName)
         {
         cv::FileStorage FS(strFileName, cv::FileStorage::READ);
         bool bRet = FS.isOpened();
         if (bRet)
            {
            bRet = Read(FS);
            } // end if
         return (bRet);
         }

      bool Read(const cv::FileStorage& FS)
         {
         cv::FileNode Node = FS[m_strID];
         return (Read(Node));
         }

      static bool ReadCameraMatrix(const cv::FileStorage& FS,
            cv::Mat& CameraMatrix);
      static bool ReadCameraMatrix(const std::string strFileName,
            cv::Mat& CameraMatrix)
         {
         return (ReadCameraMatrix(
               cv::FileStorage(strFileName, cv::FileStorage::READ),
               CameraMatrix));
         }

      static bool ReadDistortionCoeffs(const cv::FileNode& Node,
            cv::Mat& DistortionCoeffs);

      bool WriteImages(const std::string& strFileName,
            const std::string& strExtension = std::string("png")) const;

      // Global functions for OpenCV FileStorage streaming
      void write(cv::FileStorage& FS, const std::string&,
            const CameraCalibration& X);
      void read(const cv::FileNode& Node, CameraCalibration& X,
            const CameraCalibration& Default = CameraCalibration());

   protected:
      // Board used for calibration
      CameraCalibrationBoard m_Board;

      cv::Size m_ImageSize;
      bool m_bFixAspectRatio;
      int m_nFlag;
      cv::Mat m_CameraMatrix;
      cv::Mat m_DistortionCoeffs;
      std::vector<cv::Mat> m_RVecs;
      std::vector<cv::Mat> m_TVecs;
      double m_dRMS;
      std::vector<double> m_ReprojErrors;
      double m_dTotalAvgError;
      std::vector<std::vector<cv::Point2f>> m_ImagePoints;
      std::vector<cv::Mat> m_Images;
      int m_nGoodImages;
      bool m_bSaveImages;

      // Storage names
      static const std::string m_strID;
      static const std::string m_strBoard;
      static const std::string m_strFixAspectRatio;
      static const std::string m_strFlag;
      static const std::string m_strCameraMatrix;
      static const std::string m_strDistortionCoeffs;
      static const std::string m_strRVecs;
      static const std::string m_strTVecs;
      static const std::string m_strRMS;
      static const std::string m_strReprojErrors;
      static const std::string m_strTotalAvgError;
      static const std::string m_strImagePoints;
      static const std::string m_strGoodImages;
      static const std::string m_strImageSize;
      static const std::string m_strSaveImages;

      void CalcBoardCornerPositions(std::vector<cv::Point3f>& Corners);
      double ComputeReprojectionErrors(
            const std::vector<std::vector<cv::Point3f>>& ObjectPoints);

   private:

   }; // end of class CameraCalibration

#endif // CAMERACALIBRATION_H

