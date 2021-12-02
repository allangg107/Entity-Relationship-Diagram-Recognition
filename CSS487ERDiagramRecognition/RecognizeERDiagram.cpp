#include "RecognizeERDiagram.h"

void RecognizeERDiagram::recognizeDiagram()
{
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(grayImage, thresh, 150, 255, THRESH_BINARY);

	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	detectShapes();
}

void RecognizeERDiagram::detectShapes() {
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
			else if (approx.size() > 6) {
				circles.push_back(approx);
			}
		}
		else {
			//delete contour if contour touches border
			contours.erase(contours.begin() + i);
		}
	}
}

//helper method checks if shape is on the border
bool RecognizeERDiagram::contourTouchesBorder(const vector<Point>& contour, const Size& imageSize)
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
	if (shape.x <= xMin || shape.y <= yMin || shapeEndX >= xMax || shapeEndY >= yMax)
	{
		touchesBorder = true;
	}

	return touchesBorder;
}

double RecognizeERDiagram::angle(const Point pt1, const Point pt2, const Point pt0)
{
	double dx1 = (double)pt1.x - pt0.x;
	double dy1 = (double)pt1.y - pt0.y;
	double dx2 = (double)pt2.x - pt0.x;
	double dy2 = (double)pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

void RecognizeERDiagram::drawOriginalImage()
{
	imshow("Original Image", image);
}

void RecognizeERDiagram::drawAllContours()
{
	Mat imageCopy = image.clone();
	drawContours(imageCopy, contours, -1, Scalar(120, 0, 120), 2);
	imshow("All Contours", imageCopy);
}

void RecognizeERDiagram::drawColorCodedContours()
{
	Mat imageCopy(image.size(), image.type());
	drawContours(imageCopy, contours, -1, Scalar(120, 0, 120), 2);
	drawContours(imageCopy, rectangles, -1, Scalar(255, 0, 0), 2);
	drawContours(imageCopy, squares, -1, Scalar(0, 255, 0), 2);
	drawContours(imageCopy, circles, -1, Scalar(0, 0, 255), 2);
	imshow("Color Coded Contours", imageCopy);
}

void RecognizeERDiagram::labelShape(Mat& imageCopy, Scalar color, Point upperLeft) {
	upperLeft.y -= 3;
	//if red, than entity type
	if (color == Scalar(255, 0, 0)) {
		putText(imageCopy, "Entity", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
	//if green, than relationship type
	if (color == Scalar(0, 255, 0)) {
		putText(imageCopy, "Relationship", upperLeft, FONT_HERSHEY_SIMPLEX, 0.4, color, 1);
	}
	//if blue, than attribute type
	if (color == Scalar(0, 0, 255)) {
		putText(imageCopy, "Attribute", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
}

void RecognizeERDiagram::drawRectForShapes() {

	Mat imageCopy = image.clone();
	if (!rectangles.empty()) drawRectForSpecificShape(rectangles, imageCopy, Scalar(255, 0, 0));
	if (!squares.empty()) drawRectForSpecificShape(squares, imageCopy, Scalar(0, 255, 0));
	if (!circles.empty()) drawRectForSpecificShape(circles, imageCopy, Scalar(0, 0, 255));
	imshow("Color Coded Shapes", imageCopy);
}

void RecognizeERDiagram::drawRectForSpecificShape(vector<vector<Point>> currentShape, Mat& imageCopy, Scalar color) {
	Point upperLeft = currentShape[0][0];
	Point lowerRight = currentShape[0][0];
	
	for (int i = 0; i < currentShape.size(); i++) {
		upperLeft = currentShape[i][0];
		lowerRight = currentShape[i][0];
		for (int j = 0; j < currentShape[i].size(); j++) {
			if (currentShape[i][j].x < upperLeft.x) upperLeft.x = currentShape[i][j].x;
			if (currentShape[i][j].y < upperLeft.y) upperLeft.y = currentShape[i][j].y;
			if (currentShape[i][j].x > lowerRight.x) lowerRight.x = currentShape[i][j].x;
			if (currentShape[i][j].y > lowerRight.y) lowerRight.y = currentShape[i][j].y;
		}
		
		upperLeft.x -= 20;
		upperLeft.y -= 20;
		lowerRight.x += 20;
		lowerRight.y += 20;
		rectangle(imageCopy, upperLeft, lowerRight, color, 1);
		labelShape(imageCopy, color, upperLeft);
	}
}


RecognizeERDiagram::RecognizeERDiagram(string fileName)
{
	image = imread(fileName);
}

int RecognizeERDiagram::getNumCircles()
{
	return circles.size();
}

int RecognizeERDiagram::getNumRectangles()
{
	return rectangles.size();
}

int RecognizeERDiagram::getNumSquares()
{
	return squares.size();
}
