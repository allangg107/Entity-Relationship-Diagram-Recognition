#ifndef RECOGNIZE_ER_DIAGRAM_H
#define RECOGNIZE_ER_DIAGRAM_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>
using namespace std;
using namespace cv;

class RecognizeERDiagram
{
public:
	RecognizeERDiagram() = delete;
	RecognizeERDiagram(string fileName);
	void recognizeDiagram();
	void drawOriginalImage();
	void drawAllContours();
	void drawColorCodedContours();
	void labelShape(Mat& imageCopy, Scalar color, Point upperLeft);
	void drawRectForShapes();
	void drawRectForSpecificShape(vector<vector<Point>> currentShape, Mat& imageCopy, Scalar color);
	int getNumCircles();
	int getNumRectangles();
	int getNumSquares();

private:
	Mat image;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> squares;
	vector<vector<Point>> rectangles;
	vector<vector<Point>> circles;

	void detectShapes();
	bool contourTouchesBorder(const vector<Point>& contour, const Size& imageSize);
	void eraseParentContour();
	double angle(const Point pt1, const Point pt2, const Point pt0);
};

#endif