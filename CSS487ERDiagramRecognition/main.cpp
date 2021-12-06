// main.cpp
// Purpose: driver to test RecognizeERDiagram class
// Functionality: tests the RecognizeERDiagram class with various inputs and gives the corresponding outputs
// Assumptions: 
//	Each test image being used is in the directory
// Authors: Allan Genari Gaarden, Tommy Ni, Joshua Medvinsky

#include "RecognizeERDiagram.h"

// test structure for ease of adding tests
struct Test
{
	string imageName;
	int expectedAttributes;
	int expectedEntities;
	int expectedRelationships;
	int expectedWeakEntities;
	int expectedWeakRelationships;
	int expectedMultivaluedAttributes;
};

// ------------------------------------ testCase --------------------------------------

// purpose: test a given case
// preconditions: test is using valid image and draw is true or false depending on if user wants drawing
// postconditions: outputs the actual vs expected numbers of each type recognized in the image
//	and outputs resulting images if applicable

// --------------------------------------------------------------------------------------
void testCase(Test test, bool draw)
{
	RecognizeERDiagram* rec = new RecognizeERDiagram(test.imageName);

	cout << "\nActual vs Expected" << endl;
	cout << "Attributes        : " << rec->getNumAttributes() << " : " << test.expectedAttributes << endl;
	cout << "Entities          : " << rec->getNumEntities() << " : " << test.expectedEntities << endl;
	cout << "Relationships     : " << rec->getNumRelationships() << " : " << test.expectedRelationships << endl;
	cout << "Weak Entities     : " << rec->getNumWeakEntities() << " : " << test.expectedWeakEntities << endl;
	cout << "Weak Relationships: " << rec->getNumWeakRelationships() << " : " << test.expectedWeakRelationships << endl;
	cout << "Multivalued Attributes: " << rec->getNumMultivaluedAttributes() << " : " << test.expectedMultivaluedAttributes << endl;

	if (draw)
	{
		rec->drawOriginalImage();
		rec->drawRectForShapes();
		rec->drawColorCodedContours();

		waitKey(0);
		destroyAllWindows();
	}
}

// ------------------------------------ main --------------------------------------

// purpose: to run all tests
// preconditions: the tests that are added in main must be in the directory
// postconditions: gives the corresponding outputs for each test

// --------------------------------------------------------------------------------------
int main()
{
	bool drawTests = true;
	vector<Test> testCases;

	testCases.push_back(Test{ "paintTest8.png", -1, -1, -1, -1, -1, -1 });


	/*
	add more test cases here
	*/

	for (int i = 0; i < testCases.size(); i++)
	{
		testCase(testCases[i], drawTests);
	}
}