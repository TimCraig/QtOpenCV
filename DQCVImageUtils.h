/*
 * DQCVImageUtils.h
 *
 *  Created on: May 10, 2014
 *      Author: Tim Craig
 *
 *      This file contains functions for convering between QImage and OpenCV
 *      cv::Mat.  This allows the program to handle the HMI parts in Qt and
 *      OpenCV for the nitty gritty image processing tasks at which it
 *      excels.
 *
 *      Currently, only the most basic image types are handled but it is
 *      easy to add others as necessary.
 */

#ifndef DQCVIMAGEUTILS_H_
#define DQCVIMAGEUTILS_H_

/*****************************************************************************
 ******************************  I N C L U D E  *******************************
 *****************************************************************************/

#include <QImage>
#include <opencv2/core.hpp>
#include "DQImage.h"

QImage cvMatToQImage(const cv::Mat& MatIn, bool bCloneData = true);
cv::Mat QImageToCvMat(const QImage& ImageIn, bool bCloneImageData = true);
cv::Mat QImageToCvMat(const QImage& ImageIn, int nCVType);

#endif /* DQCVIMAGEUTILS_H_ */
