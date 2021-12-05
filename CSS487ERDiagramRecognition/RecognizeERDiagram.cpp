#include "RecognizeERDiagram.h"

void RecognizeERDiagram::recognizeDiagram()
{
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(grayImage, thresh, 150, 255, THRESH_BINARY);

	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	detectShapes();

	eraseParentContour();

	determineWeakType(entities, weakEntities);
	determineWeakType(relationships, weakRelationships);
}

void RecognizeERDiagram::detectShapes() 
{
	vector<Point> approx;

	for (size_t i = 0; i < contours.size(); i++) 
	{
		// checks if contour is touching border
		if (contourTouchesBorder(contours[i], image.size()) == false) 
		{
			approxPolyDP(Mat(contours[i]), approx,
				arcLength(Mat(contours[i]), true) * 0.02, true);

			//Detect difference between square and rectangle
			if (approx.size() == 4 &&
				fabs(contourArea(Mat(approx))) > 1000 &&
				isContourConvex(Mat(approx)))
			{

				Rect r = boundingRect(contours[i]);
				double ratio = abs(1 - (double)r.width / r.height);
				if (ratio <= 0.2) // if square
				{
					/*bool weak = checkIfWeak(i);
					if (weak) weakRelationships.push_back(approx);*/
					relationships.push_back(approx);
				}
				else // otherwise rectangle
				{
					/*bool weak = checkIfWeak(i);
					if (weak) weakEntities.push_back(approx);*/
					entities.push_back(approx);
				}
			}
			else if (approx.size() > 6) // if greater than 6 vertices, it is a circle
			{
				attributes.push_back(approx);
			}
		}
		else 
		{
			//delete contour if contour touches border
			contours.erase(contours.begin() + i);
		}
	}
}

/*
bool RecognizeERDiagram::checkIfWeak(int contourIndex)
{
	if (hierarchy[contourIndex][2] != -1) return true;
	
	return false;
}
*/

//helper method checks if shape is on the border
bool RecognizeERDiagram::contourTouchesBorder(const vector<Point>& contour, const Size& imageSize)
{
	Rect shape = boundingRect(contour);

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
		return true;
	}

	return false;
}

void RecognizeERDiagram::eraseParentContour()
{
	for (size_t i = 0; i < attributes.size(); i++)
	{
		if (contourArea(attributes[i]) > 20000)
		{
			attributes.erase(attributes.begin() + i);
		}
	}
}

void RecognizeERDiagram::determineWeakType(vector<vector<Point>>& type, vector<vector<Point>>& weakType)
{
	vector<vector<Point>> currentWeakType;

	for (int i = 0; i < type.size(); i++)
	{
		for (int j = 0; j < type.size(); j++)
		{
			if(isNested(type[i], type[j])) // if [i] is nested inside [j]
			{
				currentWeakType.push_back(type[j]); // consider [j] a weak entity
				for (int k = 0; k < currentWeakType.size(); k++)
				{
					// if [i] was previously determined to be a weak entity, remove it
					if (currentWeakType[k] == type[i])
					{
						currentWeakType.erase(currentWeakType.begin() + k);
					}
				}
				// remove [i] from type, since it is part of a weak entity
				type.erase(type.begin() + i); 
				// if [i] is removed, need to reset for check on next element
				i--;
				break;
			}
		}
	}

	for (int i = 0; i < currentWeakType.size(); i++)
	{
		for (int j = 0; j < type.size(); j++)
		{
			if (type[j] == currentWeakType[i])
			{
				type.erase(type.begin() + j);
			}
			if (!containsShape(currentWeakType[i], weakType)) weakType.push_back(currentWeakType[i]);
		}
	}
}

bool RecognizeERDiagram::isNested(const vector<Point>& contour1, const vector<Point>& contour2)
{
	// determine upperLeft and lowerRight points for both contours
	Point upperLeft1 = contour1[0];
	Point lowerRight1 = contour1[0];
	Point upperLeft2 = contour2[0];
	Point lowerRight2 = contour2[0];

	for (int i = 0; i < contour1.size(); i++) 
	{
		if (contour1[i].x < upperLeft1.x) upperLeft1.x = contour1[i].x;
		if (contour1[i].y < upperLeft1.y) upperLeft1.y = contour1[i].y;
		if (contour1[i].x > lowerRight1.x) lowerRight1.x = contour1[i].x;
		if (contour1[i].y > lowerRight1.y) lowerRight1.y = contour1[i].y;

		if (contour2[i].x < upperLeft2.x) upperLeft2.x = contour2[i].x;
		if (contour2[i].y < upperLeft2.y) upperLeft2.y = contour2[i].y;
		if (contour2[i].x > lowerRight2.x) lowerRight2.x = contour2[i].x;
		if (contour2[i].y > lowerRight2.y) lowerRight2.y = contour2[i].y;
	}
	/*
	Point upperLeft1, lowerLeft1, upperRight1, lowerRight1 = contour1[0];
	Point upperLeft2, lowerLeft2, upperRight2, lowerRight2 = contour2[0];

	for (int i = 0; i < contour1.size(); i++)
	{
		if (contour1[i].x <= upperLeft1.x)
		{
			if (contour1[i].y <= upperLeft1.y) upperLeft1 = contour1[i];
			else lowerLeft1 = contour1[i];
		}
		else
		{
			if (contour1[i].y >= lowerRight1.y) lowerRight1 = contour1[i];
			else upperRight1 = contour1[i];
		}

		if (contour2[i].x <= upperLeft2.x)
		{
			if (contour2[i].y <= upperLeft2.y) upperLeft2 = contour2[i];
			else lowerLeft2 = contour2[i];
		}
		else
		{
			if (contour2[i].y >= lowerRight2.y) lowerRight2 = contour2[i];
			else upperRight2 = contour2[i];
		}
	}*/

	// check if nested
	if (upperLeft2.x < upperLeft1.x && upperLeft2.y < upperLeft1.y && lowerRight2.x > lowerRight1.x && lowerRight2.y > lowerRight1.y)
	{
 		return true;
	}

	return false;
}

/*
double RecognizeERDiagram::angle(const Point pt1, const Point pt2, const Point pt0)
{
	double dx1 = (double)pt1.x - pt0.x;
	double dy1 = (double)pt1.y - pt0.y;
	double dx2 = (double)pt2.x - pt0.x;
	double dy2 = (double)pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}
*/

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
	drawContours(imageCopy, entities, -1, Scalar(255, 0, 0), 2);
	drawContours(imageCopy, relationships, -1, Scalar(0, 255, 0), 2);
	drawContours(imageCopy, attributes, -1, Scalar(0, 0, 255), 2);
	drawContours(imageCopy, weakEntities, -1, Scalar(200, 150, 150), 2);
	drawContours(imageCopy, weakRelationships, -1, Scalar(150, 200, 150), 2);
	imshow("Color Coded Contours", imageCopy);
}

void RecognizeERDiagram::labelShape(Mat& imageCopy, Scalar color, Point upperLeft) 
{
	upperLeft.y -= 3;
	//if red, then entity type
	if (color == Scalar(255, 0, 0)) 
	{
		putText(imageCopy, "Entity", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
	//if green, then relationship type
	if (color == Scalar(0, 255, 0)) 
	{
		putText(imageCopy, "Relationship", upperLeft, FONT_HERSHEY_SIMPLEX, 0.4, color, 1);
	}
	//if blue, then attribute type
	if (color == Scalar(0, 0, 255)) 
	{
		putText(imageCopy, "Attribute", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
	//if weird blue, then weak entity type
	if (color == Scalar(200, 150, 150))
	{
		putText(imageCopy, "Weak Entity", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
	//if green, then weak relationship type
	if (color == Scalar(150, 200, 150))
	{
		putText(imageCopy, "Weak Relationship", upperLeft, FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
	}
}

void RecognizeERDiagram::drawRectForShapes()
{
	Mat imageCopy = image.clone();
	if (!entities.empty()) drawRectsForSpecificShape(entities, imageCopy, Scalar(255, 0, 0));
	if (!relationships.empty()) drawRectsForSpecificShape(relationships, imageCopy, Scalar(0, 255, 0));
	if (!attributes.empty()) drawRectsForSpecificShape(attributes, imageCopy, Scalar(0, 0, 255));
	if (!weakEntities.empty()) drawRectsForSpecificShape(weakEntities, imageCopy, Scalar(200, 150, 150));
	if (!weakRelationships.empty()) drawRectsForSpecificShape(weakRelationships, imageCopy, Scalar(150, 200, 150));
	imshow("Color Coded Shapes", imageCopy);
}

void RecognizeERDiagram::drawRectsForSpecificShape(vector<vector<Point>> currentShape, Mat& imageCopy, Scalar color) {
	Point upperLeft = currentShape[0][0];
	Point lowerRight = currentShape[0][0];
	
	for (int i = 0; i < currentShape.size(); i++) 
	{
		upperLeft = currentShape[i][0];
		lowerRight = currentShape[i][0];
		for (int j = 0; j < currentShape[i].size(); j++) 
		{
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

bool RecognizeERDiagram::containsShape(const vector<Point>& targetShape, const vector<vector<Point>>& searchShape)
{
	for (int i = 0; i < searchShape.size(); i++)
	{
		if (targetShape == searchShape[i]) return true;
	}
	return false;
}

RecognizeERDiagram::RecognizeERDiagram(string fileName)
{
	image = imread(fileName);
	recognizeDiagram();
}

int RecognizeERDiagram::getNumAttributes()
{
	return (int)attributes.size();
}

int RecognizeERDiagram::getNumEntities()
{
	return (int)entities.size();
}

int RecognizeERDiagram::getNumRelationships()
{
	return (int)relationships.size();
}

int RecognizeERDiagram::getNumWeakEntities()
{
	return (int)weakEntities.size();
}

int RecognizeERDiagram::getNumWeakRelationships()
{
	return (int)weakRelationships.size();
}
