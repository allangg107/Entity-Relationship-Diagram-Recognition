#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;



int main()
{
	//Tommy first implementation
	Mat testImage = imread("paintTest2.png");
	
	//Converts to gray scale image
	Mat imageInGray;
	cvtColor(testImage, imageInGray, COLOR_BGR2GRAY);

	//Make a threshold
	Mat threshImage;
	threshold(imageInGray, threshImage, 200, 255, THRESH_BINARY);
	imshow("Binary image", threshImage);
	waitKey(0);
	destroyAllWindows();

	//Finding countours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(threshImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat image_copy = testImage.clone();
		drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
		imshow("None approximation", image_copy);
		waitKey(0);
		destroyAllWindows();


	/*
	//Allan's inital code to test things
	Mat testImage = imread("rectangle&triangle&circle.png");

	Mat testImageGray;
	cvtColor(testImage, testImageGray, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(testImageGray, thresh, 150, 255, THRESH_BINARY);
	imshow("Binary mage", thresh);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	
	Mat testImageCopy = testImage.clone();
	findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	drawContours(testImageCopy, contours, -1, Scalar(120, 0, 120), 2);
	imshow("None approximation", testImageCopy);

	waitKey(0);
	destroyAllWindows();
	*/
}