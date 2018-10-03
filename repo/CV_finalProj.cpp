
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
	int red, green, blue;
	CascadeClassifier face_cascade;
	face_cascade.load("c://Users/mksph/Desktop/opencv_root/external libraries/opencv/etc/haarcascades/haarcascade_frontalface_alt.xml");
	if (!face_cascade.load("c://Users/mksph/Desktop/opencv_root/external libraries/opencv/etc/haarcascades/haarcascade_frontalface_alt.xml"))
	{
		cerr << "Error Loading XML file" << endl;
		return 0;
	}

	VideoCapture capture(0);
	if (!capture.isOpened())
		throw "Error when reading file";
	namedWindow("window", 1);
	for (;;)
	{
		int blur_degree = 15;
		char c = waitKey(1);
		if (c == 27)//escape key!
			break;
		if (c == 'i' || c == 'I')
			blur_degree += 5;
		if (c == 'd' || c == 'D')
			blur_degree -= 5;

		Mat image;
		capture >> image;
		if (image.empty())
			break;

		// Detect faces
		std::vector<Rect> faces;
		face_cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		// Draw circles on the detected faces
		for (int i = 0; i < faces.size(); i++)
		{
			//rectangle(image, cvPoint(faces[i].x, faces[i].y), cvPoint(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(255, 255, 255), 1, 8, 0);

			//// blurring the image./////////////////////////////////////////////////////////////////
			vector<Rect> mCells;

			for (int y = faces[i].y; y < (faces[i].y + faces[i].height) - blur_degree; y += blur_degree) {
				for (int x = faces[i].x; x < (faces[i].x + faces[i].width - blur_degree); x += blur_degree) {
					//int k = (x * y) + x;
					Rect grid_rect(x, y, blur_degree, blur_degree);
					cout << grid_rect << endl;
					mCells.push_back(grid_rect);
					//rectangle(image, grid_rect, Scalar(0, 255, 0), 1);
					for (int y_new = y; y_new < y + (blur_degree); y_new++)
					{
						for (int x_new = x; x_new < x + (blur_degree); x_new++)
						{
							red += (image.ptr<uchar>(y_new)[3 * x_new + 0]);
							green += (image.ptr<uchar>(y_new)[3 * x_new + 1]);
							blue += (image.ptr<uchar>(y_new)[3 * x_new + 2]);
						}
					}
					red = red / (blur_degree * blur_degree);
					green = green / (blur_degree * blur_degree);
					blue = blue / (blur_degree * blur_degree);
					for (int y_new = y; y_new < y + (blur_degree); y_new++)
					{
						for (int x_new = x; x_new < x + (blur_degree); x_new++)
						{
							(image.ptr<uchar>(y_new)[3 * x_new + 0]) = red;
							(image.ptr<uchar>(y_new)[3 * x_new + 1]) = green;
							(image.ptr<uchar>(y_new)[3 * x_new + 2]) = blue;
						}
					}
				}

				imshow("Detected Face", image);
			}
		}
	}

	return 0;
}

