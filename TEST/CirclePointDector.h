#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

class CirclePointDector
{
public: 

	static void dector(Mat image)
	{
		SimpleBlobDetector::Params params;
		params.minThreshold = 0;
		params.maxThreshold = 255;
		params.thresholdStep = 5;

		params.minRepeatability = 2;

		params.minDistBetweenBlobs = 10;

		params.filterByColor = true;
		params.blobColor = 0;

		params.filterByArea = true;
		params.minArea = 50; 
		params.maxArea = 5000;

		params.filterByCircularity = true;
		params.minCircularity = 0.9;
		params.maxCircularity = std::numeric_limits<float>::max();

		params.filterByInertia = true;
		params.minInertiaRatio = 0.5;
		params.maxInertiaRatio = std::numeric_limits<float>::max();

		params.filterByConvexity = true;
		params.minConvexity = 0.5;
		params.maxConvexity = std::numeric_limits<float>::max();

		Ptr<SimpleBlobDetector> dect = SimpleBlobDetector::create(params);

		std::vector<KeyPoint> key_points;

		dect->detect(image, key_points);

		Mat outputimage;
		drawKeypoints(image, key_points, outputimage, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		imshow("out", outputimage);
		waitKey(0);
	}

private:

}; 