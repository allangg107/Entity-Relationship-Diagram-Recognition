#include "RecognizeERDiagram.h"

struct Test
{
	string imageName;
	int expectedAttributes;
	int expectedEntities;
	int expectedRelationships;
	int expectedWeakEntities;
	int expectedWeakRelationships;
};

void testCase(Test test, bool draw)
{
	RecognizeERDiagram* rec = new RecognizeERDiagram(test.imageName);

	if (draw)
	{
		rec->drawOriginalImage();
		rec->drawRectForShapes();
		rec->drawColorCodedContours();
	}

	cout << "\nActual vs Expected" << endl;
	cout << "Attributes        : " << rec->getNumAttributes() << " : " << test.expectedAttributes << endl;
	cout << "Entities          : " << rec->getNumEntities() << " : " << test.expectedEntities << endl;
	cout << "Relationships     : " << rec->getNumRelationships() << " : " << test.expectedRelationships << endl;
	cout << "Weak Entities     : " << rec->getNumWeakEntities() << " : " << test.expectedWeakEntities << endl;
	cout << "Weak Relationships: " << rec->getNumWeakRelationships() << " : " << test.expectedWeakRelationships << endl;

	waitKey(0);
	destroyAllWindows();
}

int main()
{
	bool drawTests = true;
	vector<Test> testCases;

	testCases.push_back(Test{ "paintTest8.png", -1, -1, -1, -1, -1 });


	/*
	add more test cases here
	*/

	for (int i = 0; i < testCases.size(); i++)
	{
		testCase(testCases[i], drawTests);
	}
}