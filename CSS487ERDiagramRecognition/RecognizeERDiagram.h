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
	void drawOriginalImage();
	void drawAllContours();
	void drawColorCodedContours();
	void labelShape(Mat& imageCopy, Scalar color, Point upperLeft);
	void drawRectForShapes();
	void drawRectsForSpecificShape(vector<vector<Point>> currentShape, Mat& imageCopy, Scalar color);
	int indexOfShape(const vector<Point>& targetShape, const vector<vector<Point>>& searchShape);
	int getNumAttributes();
	int getNumEntities();
	int getNumRelationships();
	int getNumWeakEntities();
	int getNumWeakRelationships();

private:
	Mat image;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> relationships;
	vector<vector<Point>> entities;
	vector<vector<Point>> attributes;
	vector<vector<Point>> weakRelationships;
	vector<vector<Point>> weakEntities;
	vector<vector<Point>> weakAttributes;

	Scalar contourColor = (120, 0, 120);
	Scalar entityColor = (255, 0, 0);
	Scalar relationshipColor = (0, 255, 0);
	Scalar attributeColor = (0, 0, 255);
	Scalar weakEntityColor = (200, 150, 150);
	Scalar weakRelationshipColor = (150, 200, 150);
	Scalar weakAttributeColor = (150, 150, 200);

	void recognizeDiagram();
	void detectShapes();
	// bool checkIfWeak(int contourIndex);
	bool contourTouchesBorder(const vector<Point>& contour, const Size& imageSize);
	void eraseParentContour();
	void determineWeakType(vector<vector<Point>>& type, vector<vector<Point>>& weakType);
	bool isNested(const vector<Point>& contour1, const vector<Point>& contour2);
	// double angle(const Point pt1, const Point pt2, const Point pt0);
};

#endif