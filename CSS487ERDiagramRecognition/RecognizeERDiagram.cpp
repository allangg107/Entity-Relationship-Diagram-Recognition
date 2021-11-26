#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
using namespace std;
using namespace cv;

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

int main()
{
	/*
	//Tommy's code to test things
	Mat testImage = imread("paintTest2.png");
	
	//Converts to gray scale image
	Mat imageInGray;
	cvtColor(testImage, imageInGray, COLOR_BGR2GRAY);

	//Make a threshold
	Mat threshImage;
	threshold(imageInGray, threshImage, 200, 255, THRESH_BINARY);
	imshow("Binary image", threshImage);
	waitKey(0);
	destroyAllWindows();

	//Finding countours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(threshImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat image_copy = testImage.clone();
		drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
		imshow("None approximation", image_copy);
		waitKey(0);
		destroyAllWindows();
	*/

	
	//Allan's code to test things
	Mat testImage = imread("rectangle&triangle&circle.png");

	Mat testImageGray;
	cvtColor(testImage, testImageGray, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(testImageGray, thresh, 150, 255, THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	vector<vector<Point> > squares;	
	vector<Point> approx;

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx,
			arcLength(Mat(contours[i]), true) * 0.02, true);

		if (approx.size() == 4 &&
			fabs(contourArea(Mat(approx))) > 1000 &&
			isContourConvex(Mat(approx)))
		{
			double maxCosine = 0;

			for (int j = 2; j < 5; j++)
			{
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}

			if (maxCosine < 0.3)
				squares.push_back(approx);
		}
	}

	Mat testImageCopy = testImage.clone();
	drawContours(testImageCopy, contours, -1, Scalar(120, 0, 120), 2);

	imshow("Binary Image", thresh);
	imshow("None approximation", testImageCopy);

	cout << "\nNumber of rectangles: " << squares.size() << endl;

	waitKey(0);
	destroyAllWindows();
}