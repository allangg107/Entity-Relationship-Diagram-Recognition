#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
using namespace std;
using namespace cv;

static double angle(const Point pt1, const Point pt2, const Point pt0)
{
	double dx1 = (double)pt1.x - pt0.x;
	double dy1 = (double)pt1.y - pt0.y;
	double dx2 = (double)pt2.x - pt0.x;
	double dy2 = (double)pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

//helper method checks if shape is on the border
bool contourTouchesBorder(const vector<Point>& contour, const Size& imageSize)
{
	cv::Rect shape = cv::boundingRect(contour);

	bool touchesBorder = false;

	int xMin, xMax, yMin, yMax;

	xMin = 0;
	yMin = 0;
	xMax = imageSize.width - 1;
	yMax = imageSize.height - 1;

	//checks if x or y is at the border
	int shapeEndX = shape.x + shape.width - 1;
	int shapeEndY = shape.y + shape.height - 1;
	if (shape.x <= xMin || shape.y <= yMin ||	shapeEndX >= xMax ||shapeEndY >= yMax)
	{
		touchesBorder = true;
	}

	return touchesBorder;
}

void detectShapes(vector<vector<Point>>& contours, const Mat& image) {
	vector<vector<Point>> squares;
	vector<vector<Point>> rectangles;
	vector<vector<Point>> circles;
	vector<Point> approx;

	for (size_t i = 0; i < contours.size(); i++) {
		// checks if contour is touching border
		if (contourTouchesBorder(contours[i], image.size()) == false) {
			approxPolyDP(Mat(contours[i]), approx,
				arcLength(Mat(contours[i]), true) * 0.02, true);

			//Detect difference between square and rectangle
			if (approx.size() == 4 &&
				fabs(contourArea(Mat(approx))) > 1000 &&
				isContourConvex(Mat(approx)))
			{
				double maxCosine = 0;

				//for (int j = 2; j < 5; j++)
				//{
				//	double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				//	maxCosine = MAX(maxCosine, cosine);
				//}

				Rect r = boundingRect(contours[i]);
				double ratio = abs(1 - (double)r.width / r.height);
				if (ratio <= 0.2) {
					//Detects its a square
					squares.push_back(approx);
				}
				else {
					//Detects its a rectangle
					rectangles.push_back(approx);
				}
			}
			//Detects that it is a circle/oval
			else if(approx.size() > 6) {
				circles.push_back(approx);
			}
		}
		else {
			//delete contour if contour touches border
			contours.erase(contours.begin() + i);
		}
	}
	cout << "\nNumber of squares: " << squares.size() << endl;
	cout << "\nNumber of rectangles: " << rectangles.size() << endl;
	cout << "\nNumber of circles: " << circles.size() << endl;

	Mat testImageCopy(image.size(), image.type());
	drawContours(testImageCopy, contours, -1, Scalar(120, 0, 120), 2);
	drawContours(testImageCopy, rectangles, -1, Scalar(255, 0, 0), 2);
	drawContours(testImageCopy, squares, -1, Scalar(0, 255, 0), 2);
	drawContours(testImageCopy, circles, -1, Scalar(0, 0, 255), 2);
	imshow("Test", testImageCopy);
}

int main()
{
	//Mat testImage = imread("rectangle&triangle&circle.png");
	
	//paint test
	Mat testImage = imread("paintTest3.png");

	Mat testImageGray;
	cvtColor(testImage, testImageGray, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(testImageGray, thresh, 150, 255, THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	//Detects shapes
	detectShapes(contours, testImage);

	Mat testImageCopy = testImage.clone();
	drawContours(testImageCopy, contours, -1, Scalar(120, 0, 120), 2);

	imshow("Original", testImage);
	imshow("Contours", testImageCopy);

	//cout << "\nNumber of rectangles: " << squares.size() << endl;

	waitKey(0);
	destroyAllWindows();
}