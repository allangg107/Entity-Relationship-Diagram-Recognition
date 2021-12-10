// RecognizeERDiagram.cpp
// Purpose: recognize and label different entity types in a given hand drawn ER diagram
// Functionality: given a hand drawn ER diagram image, produces an image that boxes
//	and labels each entity type in the diagram appropriately
// Assumptions: 
//	Image used is a valid image ressembling an ER diagram
//	None of the objects drawn are touching the border of the image
//	Objects must be closed (i.e. a square cannot have any gaps between its sides)
// Authors: Allan Genari Gaarden, Tommy Ni, Joshua Medvinsky

#include "RecognizeERDiagram.h"

// ------------------------------------ recognizeDiagram --------------------------------------

// purpose: identify each object in the image
// preconditions: the image has already been defined
// postconditions: every object's contour in the image is stored in the appropriate type vector

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::recognizeDiagram()
{
	int minThreshold = 150;
	int maxThreshold = 255;
	// prepares image to find all contours
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	Mat thresh;
	threshold(grayImage, thresh, minThreshold, maxThreshold, THRESH_BINARY);

	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	// populates type vectors (except weak types)
	detectShapes();

	// gets rid of the unecessary outer contour
	eraseParentContour();

	// distinguishes weak types 
	determineWeakType(entities, weakEntities);
	determineWeakType(relationships, weakRelationships);
	determineWeakType(attributes, weakAttributes);
}

// ------------------------------------ detectShapes --------------------------------------

// purpose: populate vector types (except weak types)
// preconditions: contours vector has been populated 
// postconditions: populates all vector types, except weak types
// This method structure was inspired by http://www.calumk.com/old_posts_archive/0008//detecting-simple-shapes-in-an-image/
// We made edits to the code to not check for angles of the shapes
// as we only deal with rectangles/squares.

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::detectShapes() 
{
	vector<Point> approx;
	int thresholdAreaForRect = 500;
	int thresholdAreaForCircle = 500;
	double thresholdRatioForSqar = 0.2;
	// goes through every contour
	for (size_t i = 0; i < contours.size(); i++) 
	{
		// checks if contour is touching border
		if (contourTouchesBorder(contours[i], image.size()) == false) 
		{
			approxPolyDP(Mat(contours[i]), approx,
				arcLength(Mat(contours[i]), true) * 0.02, true);

			// distinguishes between square and rectangle
			if (approx.size() == 4 &&
				fabs(contourArea(Mat(approx))) > thresholdAreaForRect &&
				isContourConvex(Mat(approx)))
			{

				Rect r = boundingRect(contours[i]);
				double ratio = abs(1 - (double)r.width / r.height);
				if (ratio <= thresholdRatioForSqar) // if sides are mostly similar in length, it is a square
				{
					relationships.push_back(approx);
				}
				else // otherwise it is a rectangle
				{
					entities.push_back(approx);
				}
			}
			else if (approx.size() > 6) // if greater than 6 vertices, it is a circle
			{
				if(fabs(contourArea(Mat(approx))) > thresholdAreaForCircle) attributes.push_back(approx);
			}
		}
		else 
		{
			//delete contour from contours vector if  current contour touches border
			contours.erase(contours.begin() + i);
		}
	}
}

// ------------------------------------ contourTouchesBorder --------------------------------------

// purpose: helper method checks if contour touches the border
// preconditions: using intended contour, and image size is the image's size
// postconditions: returns true or false based on if the contour is touching the border

// --------------------------------------------------------------------------------------
bool RecognizeERDiagram::contourTouchesBorder(const vector<Point>& contour, const Size& imageSize)
{
	Rect shape = boundingRect(contour);
	// created ints to represent the mins and maxes for x and y respectively
	int xMin, xMax, yMin, yMax;
	//set x and y min to 0
	xMin = 0;
	yMin = 0;
	//set x to the images width-1, and y to image height-1
	xMax = imageSize.width - 1;
	yMax = imageSize.height - 1;

	int shapeEndX = shape.x + shape.width - 1;
	int shapeEndY = shape.y + shape.height - 1;
	// if the min or max coordinates of the contour are outside the range of the image, returns true
	if (shape.x <= xMin || shape.y <= yMin || shapeEndX >= xMax || shapeEndY >= yMax)
	{
		return true;
	}
	//returns false if coordinates of the contour are in range
	return false;
}

// ------------------------------------ eraseParentContour --------------------------------------

// purpose: to get rid of the outer contour, if there is one
// preconditions: attributes vector has been populated
// postconditions: removes the outer contour from the vector

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::eraseParentContour()
{
	int thresholdForOutsideContour = 20000;
	for (size_t i = 0; i < attributes.size(); i++)
	{
		// given an ER diagram, the outer contour, if it exists, is almost guaranteed to be recognized 
		//	as an attribute. this outer contour is removed based on a reasonable size requirement
		if (contourArea(attributes[i]) > thresholdForOutsideContour)
		{
			attributes.erase(attributes.begin() + i);
		}
	}
}

// ------------------------------------ determineWeakType --------------------------------------

// purpose: seperates the weak from the strong of a given type
// preconditions: pointer to the vector containing the type to seperate, and the pointer to the vector
// //	the weak type will be stored in
// postconditions: all strong types will be left in type and all the weak types found in type will be 
// removed and stored in the weakType vector

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::determineWeakType(vector<vector<Point>>& type, vector<vector<Point>>& weakType)
{
	// compares each contour against all other contours to see if it is nested
	for (int i = 0; i < type.size(); i++)
	{
		for (int j = 0; j < type.size(); j++)
		{
			if(isNested(type[i], type[j])) // if [i] is nested inside [j]
			{
				// consider [j] a weak entity if it is not already
				if (indexOfShape(type[j], weakType) == -1) weakType.push_back(type[j]);
				// remove [i] from type, since it is part of a weak entity
				type.erase(type.begin() + i); 
				// if [i] is removed, need to reset for check on next element
				i--;
				break;
			}
		}
	}

	// removes any weak entities from entities
	int index;
	for (int i = 0; i < weakType.size(); i++)
	{
		index = indexOfShape(weakType[i], type);
		if (index != -1) type.erase(type.begin() + index);
	}
}

// ------------------------------------ isNested --------------------------------------

// purpose: determines if contour1 is inside contour2
// preconditions: contour1 and contour 2 are valid contours
// postconditions: returns true if contour1 is inside contour2, false if not

// --------------------------------------------------------------------------------------
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
	}
	for(int i = 0; i < contour2.size(); i++)
	{
		if (contour2[i].x < upperLeft2.x) upperLeft2.x = contour2[i].x;
		if (contour2[i].y < upperLeft2.y) upperLeft2.y = contour2[i].y;
		if (contour2[i].x > lowerRight2.x) lowerRight2.x = contour2[i].x;
		if (contour2[i].y > lowerRight2.y) lowerRight2.y = contour2[i].y;
	}

	// check if nested
	if (upperLeft2.x < upperLeft1.x && upperLeft2.y < upperLeft1.y && lowerRight2.x > lowerRight1.x && 
		lowerRight2.y > lowerRight1.y)
	{
 		return true;
	}

	return false;
}

// ------------------------------------ drawOriginalImage --------------------------------------

// purpose: display the original, unmodified input image
// preconditions: valid input image has been given
// postconditions: displays the original image

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::drawOriginalImage()
{
	namedWindow("Original Image", WINDOW_NORMAL);
	resizeWindow("Original Image", image.cols, image.rows);
	imshow("Original Image", image);
}

// ------------------------------------ drawAllContours --------------------------------------

// purpose: display the image showing all the contours
// preconditions: valid input image has been given and contours has been populated
// postconditions: dispays the image showing all the contours with one color

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::drawAllContours()
{
	Mat imageCopy = image.clone();
	drawContours(imageCopy, contours, -1, contourColor, 2);
	imshow("All Contours", imageCopy);
	resizeWindow("All Contours", imageCopy.cols, imageCopy.rows);
}

// ------------------------------------ drawColorCodedContours --------------------------------------

// purpose: displays all the contours color coded based on the type
// preconditions: valid input image has been given and all contour types have been populated
// postconditions: displays all the contours color coded based on the type

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::drawColorCodedContours()
{
	int thickness = 2;
	int contourid = -1;
	Mat imageCopy(image.size(), image.type());
	drawContours(imageCopy, contours, contourid, contourColor, thickness);
	drawContours(imageCopy, entities, contourid, entityColor, thickness);
	drawContours(imageCopy, relationships, contourid, relationshipColor, thickness);
	drawContours(imageCopy, attributes, contourid, attributeColor, thickness);
	drawContours(imageCopy, weakEntities, contourid, weakEntityColor, thickness);
	drawContours(imageCopy, weakRelationships, contourid, weakRelationshipColor, thickness);
	drawContours(imageCopy, weakAttributes, contourid, weakAttributeColor, thickness);
	namedWindow("Color Coded Contours", WINDOW_NORMAL);
	imshow("Color Coded Contours", imageCopy);
	resizeWindow("Color Coded Contours", imageCopy.cols, imageCopy.rows);
}

// ------------------------------------ drawRectForShapes --------------------------------------

// purpose: box and label all the shapes and display the resulting image
// preconditions: image has been defined and all type vectors have been populated as intended
// postconditions: displays an image with all the objects boxed and labled appropriately

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::drawRectForShapes()
{
	Mat imageCopy = image.clone();
	if (!entities.empty()) drawRectsForSpecificShape(entities, imageCopy, entityColor);
	if (!relationships.empty()) drawRectsForSpecificShape(relationships, imageCopy, relationshipColor);
	if (!attributes.empty()) drawRectsForSpecificShape(attributes, imageCopy, attributeColor);
	if (!weakEntities.empty()) drawRectsForSpecificShape(weakEntities, imageCopy, weakEntityColor);
	if (!weakRelationships.empty()) drawRectsForSpecificShape(weakRelationships, imageCopy, 
		weakRelationshipColor);
	if (!weakAttributes.empty()) drawRectsForSpecificShape(weakAttributes, imageCopy, weakAttributeColor);
	namedWindow("Color Coded Shapes", WINDOW_NORMAL);
	resizeWindow("Color Coded Shapes", imageCopy.cols, imageCopy.rows);
	imshow("Color Coded Shapes", imageCopy);
}

// ------------------------------------ drawRectsForSpecificShape --------------------------------------

// purpose: boxes and labels all the shapes of a specific type
// preconditions: using a valid type, a valid image, and intended color
// postconditions: image is modified to box and label all the shapes of a specific type with color 
//	equal to color passed in

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::drawRectsForSpecificShape(vector<vector<Point>> currentShape, Mat& 
	imageCopy, const Scalar color)
{
	Point upperLeft = currentShape[0][0];
	Point lowerRight = currentShape[0][0];
	int boundingBoxOffByPixel = 10;
	// goes through every shape
	for (int i = 0; i < currentShape.size(); i++) 
	{
		upperLeft = currentShape[i][0];
		lowerRight = currentShape[i][0];
		// determine coordinates to draw box
		for (int j = 0; j < currentShape[i].size(); j++) 
		{
			if (currentShape[i][j].x < upperLeft.x) upperLeft.x = currentShape[i][j].x;
			if (currentShape[i][j].y < upperLeft.y) upperLeft.y = currentShape[i][j].y;
			if (currentShape[i][j].x > lowerRight.x) lowerRight.x = currentShape[i][j].x;
			if (currentShape[i][j].y > lowerRight.y) lowerRight.y = currentShape[i][j].y;
		}
		
		// extend box to envelop the shape
		upperLeft.x -= boundingBoxOffByPixel;
		upperLeft.y -= boundingBoxOffByPixel;
		lowerRight.x += boundingBoxOffByPixel;
		lowerRight.y += boundingBoxOffByPixel;
		rectangle(imageCopy, upperLeft, lowerRight, color, 2);
		labelShape(imageCopy, color, upperLeft);
	}
}

// ------------------------------------ labelShape --------------------------------------

// purpose: to label a shape on the given image
// preconditions: valid image using a predefined type color and a point within the image
// postconditions: labels the shape in the image based on the color passed in at the point given

// --------------------------------------------------------------------------------------
void RecognizeERDiagram::labelShape(Mat& imageCopy, const Scalar color, Point upperLeft)
{
	upperLeft.y -= 3;
	int thickness = 1;
	double fontSize = 0.5;
	//if entity color, then entity type
	if (color == entityColor)
	{
		putText(imageCopy, "Entity", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
	//if relationship color, then relationship type
	if (color == relationshipColor)
	{
		putText(imageCopy, "Relationship", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
	//if attribute color, then attribute type
	if (color == attributeColor)
	{
		putText(imageCopy, "Attribute", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
	//if weak entity color, then weak entity type
	if (color == weakEntityColor)
	{
		putText(imageCopy, "Weak Entity", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
	//if weak relationship color, then weak relationship type
	if (color == weakRelationshipColor)
	{
		putText(imageCopy, "Weak Relationship", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
	//if weak attribute color, then weak attribute type
	if (color == weakAttributeColor)
	{
		putText(imageCopy, "Multivalued Attribute", upperLeft, FONT_HERSHEY_SIMPLEX, fontSize, color, thickness);
	}
}

// ------------------------------------ indexOfShape --------------------------------------

// purpose: to return the index of a target shape inside a type vector to search
// preconditions: target shape and search shape are defined
// postconditions: returns index of target in search vector, otherwise returns -1

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::indexOfShape(const vector<Point>& targetShape, 
	const vector<vector<Point>>& searchShape)
{
	for (int i = 0; i < searchShape.size(); i++)
	{
		if (targetShape == searchShape[i]) return i;
	}
	return -1;
}

// ------------------------------------ parameter constructor --------------------------------------

// purpose: the only way to create an instance of the class
// preconditions: fileName is a valid image in the directory
// postconditions: image is read in and all object contours are stored in the appropriate type vector

// --------------------------------------------------------------------------------------
RecognizeERDiagram::RecognizeERDiagram(string fileName)
{
	image = imread(fileName);
	recognizeDiagram();
}

// ------------------------------------ getNumAttributes --------------------------------------

// purpose: get the number of attributes detected from the image
// preconditions: none
// postconditions: returns the number of attributes stored in the attributes vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumAttributes()
{
	return (int)attributes.size();
}

// ------------------------------------ getNumEntities --------------------------------------

// purpose: get the number of entities detected from the image
// preconditions: none
// postconditions: returns the number of entities stored in the entities vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumEntities()
{
	return (int)entities.size();
}

// ------------------------------------ getNumRelationships --------------------------------------

// purpose: get the number of relationships detected from the image
// preconditions: none
// postconditions: returns the number of relationships stored in the relationships vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumRelationships()
{
	return (int)relationships.size();
}

// ------------------------------------ getNumWeakEntities --------------------------------------

// purpose: get the number of weak entities detected from the image
// preconditions: none
// postconditions: returns the number of weak entities stored in the weak entities vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumWeakEntities()
{
	return (int)weakEntities.size();
}

// ------------------------------------ getNumWeakRelationships --------------------------------------

// purpose: get the number of weak relationships detected from the image
// preconditions: none
// postconditions: returns the number of weak relationships stored in the weak relationships vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumWeakRelationships()
{
	return (int)weakRelationships.size();
}

// ------------------------------------ getNumMultivaluedAttributes --------------------------------------

// purpose: get the number of multivalued attributes detected from the image
// preconditions: none
// postconditions: returns the number of multivalued attributes stored in the weak attributes vector

// --------------------------------------------------------------------------------------
int RecognizeERDiagram::getNumMultivaluedAttributes()
{
	return (int)weakAttributes.size();
}


// ------------------------------------ checkIfWeak --------------------------------------

// purpose: to check if the countour was a weak (has a shape inside of it)
// preconditions: contourIndex isn't garbage data
// postconditions: returns true if the shape either had a parent or a child
//				   false otherwise
// 
// Not currently used in our program as we had a hard time using the hierarchy vector
// that was produced from findCountour(). Sometimes a shape didn't have a child
// but the method would return saying it did.
/*
bool RecognizeERDiagram::checkIfWeak(int contourIndex)
{
	if (hierarchy[contourIndex][2] != -1) return true;

	return false;
}
*/

// ------------------------------------ angle --------------------------------------

// purpose: to check the angle of a contour/shape in the image
// preconditions: all points wasn't garbage data
// postconditions: returns the angle of the shape
// 
// This method was taken from https://docs.opencv.org/4.x/db/d00/samples_2cpp_2squares_8cpp-example.html
// 
// Not currently used in our program as we do not care about the angle of our shapes/contours.
// But this method will be used if our next feature of the program is to determine non ER diagram
// symbols.
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
