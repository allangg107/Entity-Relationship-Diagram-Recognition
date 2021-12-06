// RecognizeERDiagram.h
// Purpose: recognize and label different entity types in a given hand drawn ER diagram
// Functionality: given a hand drawn ER diagram image, produces an image that boxes
//	and labels each entity type in the diagram appropriately
// Assumptions: 
//	Image used is a valid image ressembling an ER diagram
//	None of the objects drawn are touching the border of the image
//	Objects must be closed (i.e. a square cannot have any gaps between its sides)
// Authors: Allan Genari Gaarden, Tommy Ni, Joshua Medvinsky

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
	// default constructor not allowed
	RecognizeERDiagram() = delete;
	// ------------------------------------ parameter constructor --------------------------------------

// purpose: the only way to create an instance of the class
// preconditions: fileName is a valid image in the directory
// postconditions: image is read in and all object contours are stored in the appropriate type vector

// --------------------------------------------------------------------------------------
	RecognizeERDiagram(string fileName);
	// ------------------------------------ drawOriginalImage --------------------------------------

// purpose: display the original, unmodified input image
// preconditions: valid input image has been given
// postconditions: displays the original image

// --------------------------------------------------------------------------------------
	void drawOriginalImage();
	// ------------------------------------ drawAllContours --------------------------------------

// purpose: display the image showing all the contours
// preconditions: valid input image has been given and contours has been populated
// postconditions: dispays the image showing all the contours with one color

// --------------------------------------------------------------------------------------
	void drawAllContours();
	// ------------------------------------ drawColorCodedContours --------------------------------------

// purpose: displays all the contours color coded based on the type
// preconditions: valid input image has been given and all contour types have been populated
// postconditions: displays all the contours color coded based on the type

// --------------------------------------------------------------------------------------
	void drawColorCodedContours();
	// ------------------------------------ drawRectForShapes --------------------------------------

// purpose: box and label all the shapes and display the resulting image
// preconditions: image has been defined and all type vectors have been populated as intended
// postconditions: displays an image with all the objects boxed and labled appropriately

// --------------------------------------------------------------------------------------
	void drawRectForShapes();
	// ------------------------------------ drawRectsForSpecificShape --------------------------------------

// purpose: boxes and labels all the shapes of a specific type
// preconditions: using a valid type, a valid image, and intended color
// postconditions: image is modified to box and label all the shapes of a specific type with color 
//	equal to color passed in

// --------------------------------------------------------------------------------------
	void drawRectsForSpecificShape(vector<vector<Point>> currentShape, Mat& imageCopy, const Scalar color);
	// ------------------------------------ labelShape --------------------------------------

// purpose: to label a shape on the given image
// preconditions: valid image using a predefined type color and a point within the image
// postconditions: labels the shape in the image based on the color passed in at the point given

// --------------------------------------------------------------------------------------
	void labelShape(Mat& imageCopy, const Scalar color, Point upperLeft);
	// ------------------------------------ indexOfShape --------------------------------------

// purpose: to return the index of a target shape inside a type vector to search
// preconditions: target shape and search shape are defined
// postconditions: returns index of target in search vector, otherwise returns -1

// --------------------------------------------------------------------------------------
	int indexOfShape(const vector<Point>& targetShape, const vector<vector<Point>>& searchShape);
	// ------------------------------------ getNumAttributes --------------------------------------

// purpose: get the number of attributes detected from the image
// preconditions: none
// postconditions: returns the number of attributes stored in the attributes vector

// --------------------------------------------------------------------------------------
	int getNumAttributes();
	// ------------------------------------ getNumEntities --------------------------------------

// purpose: get the number of entities detected from the image
// preconditions: none
// postconditions: returns the number of entities stored in the entities vector

// --------------------------------------------------------------------------------------
	int getNumEntities();
	// ------------------------------------ getNumRelationships --------------------------------------

// purpose: get the number of relationships detected from the image
// preconditions: none
// postconditions: returns the number of relationships stored in the relationships vector

// --------------------------------------------------------------------------------------
	int getNumRelationships();
	// ------------------------------------ getNumWeakEntities --------------------------------------

// purpose: get the number of weak entities detected from the image
// preconditions: none
// postconditions: returns the number of weak entities stored in the weak entities vector

// --------------------------------------------------------------------------------------
	int getNumWeakEntities();
	// ------------------------------------ getNumWeakRelationships --------------------------------------

// purpose: get the number of weak relationships detected from the image
// preconditions: none
// postconditions: returns the number of weak relationships stored in the weak relationships vector

// --------------------------------------------------------------------------------------
	int getNumWeakRelationships();
	// ------------------------------------ getNumMultivaluedAttributes --------------------------------------

// purpose: get the number of multivalued attributes detected from the image
// preconditions: none
// postconditions: returns the number of multivalued attributes stored in the weak attributes vector

// --------------------------------------------------------------------------------------
	int getNumMultivaluedAttributes();

private:
	Mat image;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// vectors to store each type
	vector<vector<Point>> relationships;
	vector<vector<Point>> entities;
	vector<vector<Point>> attributes;
	vector<vector<Point>> weakRelationships;
	vector<vector<Point>> weakEntities;
	vector<vector<Point>> weakAttributes;

	// predefined colors for each type
	Scalar contourColor = Scalar(120, 0, 120);
	Scalar entityColor = Scalar(255, 0, 0);
	Scalar relationshipColor = Scalar(0, 255, 0);
	Scalar attributeColor = Scalar(0, 0, 255);
	Scalar weakEntityColor = Scalar(200, 150, 150);
	Scalar weakRelationshipColor = Scalar(150, 200, 150);
	Scalar weakAttributeColor = Scalar(150, 150, 200);

	// ------------------------------------ recognizeDiagram --------------------------------------

// purpose: identify each object in the image
// preconditions: the image has already been defined
// postconditions: every object's contour in the image is stored in the appropriate type vector

// --------------------------------------------------------------------------------------
	void recognizeDiagram();
	// ------------------------------------ detectShapes --------------------------------------

// purpose: populate vector types (except weak types)
// preconditions: contours vector has been populated 
// postconditions: populates all vector types, except weak types

// --------------------------------------------------------------------------------------
	void detectShapes();
	// ------------------------------------ contourTouchesBorder --------------------------------------

// purpose: helper method checks if contour touches the border
// preconditions: using intended contour, and image size is the image's size
// postconditions: returns true or false based on if the contour is touching the border

// --------------------------------------------------------------------------------------
	bool contourTouchesBorder(const vector<Point>& contour, const Size& imageSize);
	// ------------------------------------ eraseParentContour --------------------------------------

// purpose: to get rid of the outer contour, if there is one
// preconditions: attributes vector has been populated
// postconditions: removes the outer contour from the vector

// --------------------------------------------------------------------------------------
	void eraseParentContour();
	// ------------------------------------ determineWeakType --------------------------------------

// purpose: seperates the weak from the strong of a given type
// preconditions: pointer to the vector containing the type to seperate, and the pointer to the vector
// //	the weak type will be stored in
// postconditions: all strong types will be left in type and all the weak types found in type will be 
// removed and stored in the weakType vector

// --------------------------------------------------------------------------------------
	void determineWeakType(vector<vector<Point>>& type, vector<vector<Point>>& weakType);
	// ------------------------------------ isNested --------------------------------------

// purpose: determines if contour1 is inside contour2
// preconditions: contour1 and contour 2 are valid contours
// postconditions: returns true if contour1 is inside contour2, false if not

// --------------------------------------------------------------------------------------
	bool isNested(const vector<Point>& contour1, const vector<Point>& contour2);
	
	
	// bool checkIfWeak(int contourIndex);
	// double angle(const Point pt1, const Point pt2, const Point pt0);
};

#endif