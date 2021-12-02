#include "RecognizeERDiagram.h"

struct Test
{
	string imageName;
	int expectedCircles;
	int expectedRectangles;
	int expectedSquares;
};

void testCase(Test test, bool draw)
{
	RecognizeERDiagram* rec = new RecognizeERDiagram(test.imageName);
	rec->recognizeDiagram();

	if (draw)
	{
		rec->drawOriginalImage();
		//rec->drawAllContours();
		//rec->drawColorCodedContours();
		rec->drawRectForShapes();
	}
	
	cout << "\nActual vs Expected" << endl;
	cout << "Circles: " << rec->getNumCircles() << " : " << test.expectedCircles << endl;
	cout << "Rects  : " << rec->getNumRectangles() << " : " << test.expectedRectangles << endl;
	cout << "Squares: " << rec->getNumSquares() << " : " << test.expectedSquares << endl;

	waitKey(0);
	destroyAllWindows();
}

int main()
{
	bool drawTests = true;
	vector<Test> testCases;

	testCases.push_back(Test{ "paintTest4.png", 2, 2, 1 });

	/*
	add more test cases here
	*/

	for (int i = 0; i < testCases.size(); i++)
	{
		testCase(testCases[i], drawTests);
	}
}