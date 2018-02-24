#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define CAMERA0 0


Mat frame, frameHSV, frameBIN;
int filter[6] = {255, 0, 255, 0, 255, 225};

int thresh = 15;

int main()
{
	
	//Mat frame, frameHSV, frameBIN;

	char key;
	//int filter[6] = {255, 0, 255, 0, 255, 225};
	unsigned int i,j = 0;
	unsigned int t = 0;

	unsigned int white_x = 0;  //sumation of all x-coordinated of white pixels
	unsigned int white_y = 0;  //sumation of all y-coordinated of white pixels

	unsigned int white_count = 0; ////Count of all white pixels
	
	vector <unsigned int> centroid_x;
	vector <unsigned int> centroid_y;


	VideoCapture Camera;
	Camera.open(CAMERA0);
	if(!Camera.isOpened())  // check if we succeeded
    	{
        	cout<<" Camera 0 is not Opened..."<<endl;
            		return -1;
    	}
	Camera >>frame;
	Mat result(frame.rows,frame.cols, CV_8UC3, Scalar(0,0,0));
	

	for(;;)
	{	
		white_x = 0; 
		white_y = 0;
		white_count = 0; 
		Camera >> frame;
		cvtColor(frame, frameHSV, CV_BGR2HSV); // Convert colour to HSV
		inRange(frameHSV, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), frameBIN); //Filter HSV image to bet Buinary image
		

		
		for(unsigned int i=0; i<frameBIN.rows; i++)
		{
  			for(unsigned int j=0; j<frameBIN.cols; j++)
          		{	
				if(frameBIN.data[i*frameBIN.cols + j] > 200) // If colour is white
				{
					white_count++; // Number of white pixels
					white_x += j; // 
					white_y += i; //
				}
			}
		}
		
		if(white_count > thresh)
		{
			white_x /= white_count;
			white_y /= white_count;
			centroid_x.push_back(white_x);
			centroid_y.push_back(white_y);			
		}
		else
		{	
			white_x = 0;
			white_y = 0;
			centroid_x.clear();
			centroid_y.clear();
			result = Scalar(0,0,0);
		}
		

		if(centroid_x.size() > 1)
		{

		line(result, Point(centroid_x[centroid_x.size()-1],centroid_y[centroid_y.size()-1]), Point(centroid_x[centroid_x.size()-1],centroid_y[centroid_y.size()-1]), Scalar(255,255,255), 5,8); 


		}

		circle(frame, Point(white_x, white_y), 5, Scalar(0,255,2), 2, 8, 0);
		line(frame, Point(320,0), Point(320,480), Scalar(255,255,255), 1, 8, 0);
		cout<<"("<<white_x<<","<<white_y<<")"<<endl;
		key = waitKey(5);
		if (key == 'q')
			break;
		imshow("Camera", frame);
		imshow("Camera HSV", frameHSV);
		imshow("Camera BIN", frameBIN);
		imshow("Centroid Trajectory",result);
	}

	frame.release();
	frameHSV.release();
	frameBIN.release();
	Camera.release();
	return 0;
}

