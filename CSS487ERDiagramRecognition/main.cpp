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
	cout << "Weak Relationships: " << rec->getNumWeakRelationships() << " : " << 
		test.expectedWeakRelationships << endl;
	cout << "Multivalued Attributes: " << rec->getNumMultivaluedAttributes() << " : " << 
		test.expectedMultivaluedAttributes << endl;

	if (draw)
	{
		rec->drawOriginalImage();
		rec->drawRectForShapes();
		//rec->drawColorCodedContours();

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

	testCases.push_back(Test{ "paintTestSimple1.png", 0, 2, 1, 0, 0, 0 });
	testCases.push_back(Test{ "paintTestSimple2.png", 4, 2, 1, 0, 0, 1 });
	testCases.push_back(Test{ "paintTestIntermediate1.png", 3, 2, 1, 1, 1, 1 });
	testCases.push_back(Test{ "paintTestIntermediate2.png", 3, 2, 1, 1, 1, 0 });
	testCases.push_back(Test{ "paintTestIntermediate3.png", 5, 2, 1, 0, 0, 0 });
	testCases.push_back(Test{ "paintTestIntermediate4.png", 5, 1, 0, 1, 1, 0 });
	testCases.push_back(Test{ "paintTestAdvance1.png", 13, 3, 3, 1, 1, 0 });

	/*
	add more test cases here
	*/

	// runs each test
	for (int i = 0; i < testCases.size(); i++)
	{
		testCase(testCases[i], drawTests);
	}
}