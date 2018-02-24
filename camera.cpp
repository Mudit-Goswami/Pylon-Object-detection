// C++ libraries
#include <iostream>
#include <string>
#include <ctime>
#include<fstream>
// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

#define FPS 200

using namespace cv;
using namespace std;


int filter[6] = {255,0,255,0,255,225}; // Arrey for HSV filter, [Hmax, Hmin, Smax, Smin, Vmax, Vmin]
int thresh = 13; // Minimum pixels needed to be considered as Ball : To avoid noise

Mat img; // Input RGB image : Global cause of trackbar
Mat imghsv; // Input HSV image : Global cause of trackbar
Mat imgbin; // Input Binary image : Filtered object is ball : Global cause of trackbar

// Trackbar function : Routine called when trackbar is modified
void on_trackbar( int, void* )
{
	inRange(imghsv, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), imgbin); // Filtering functioni in Opencv
	//imshow("bin",imgbin);
}

// Main Routine
int main( int argc, char** argv )
{

   timespec tS,tE;
    tS.tv_sec = 0;
    tS.tv_nsec = 0;
    tE.tv_sec = 0;
    tE.tv_nsec = 0;
	unsigned int i,j = 0; // Looping variables	
	unsigned int time[209];
	unsigned short int pos[209][2];
	unsigned int t = 0;

	ofstream out("Time.csv");
	vector <unsigned int> cx; // Vector containing x-coordinates detected centroids in one trajectory 
	vector <unsigned int> cy; // Vector containing y-coordinates detected centroids in one trajectory
	unsigned int x = 0; // Contain sumation of all x-coordinated of white pixels in filtered image
	unsigned int y = 0; // Contain sumation of all y-coordinated of white pixels in filtered image
	unsigned int whitecount = 0; //Count of all white pixels

	VideoCapture Camera(0); // Object of camera 
    if(!Camera.isOpened())  // check if we succeeded
    {
        cout<<"Camera is not Opened..."<<endl;
            return -1;
    }
	Camera.set(CV_CAP_PROP_FPS, FPS); // Setting FPS = 200 : FPS is a macro

Camera.set(CV_CAP_PROP_FRAME_WIDTH, 160);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 120);


	
	namedWindow("Live Feed"); // Window to display input RGB image
	namedWindow("Parameters"); // Windoe to display Parameter trackbar
	namedWindow("Image HSV"); // Windoe to display Parameter trackbar
	namedWindow("Filtered Image"); // Window to display filtered Image
	namedWindow("Centroid Trajectory"); // Window to display centroids found

	char key = 'r'; // Key press to stop execution

	createTrackbar( "Hmax", "Parameters", &filter[0], 255,on_trackbar);
	createTrackbar( "Hmin", "Parameters", &filter[1], 255,on_trackbar);
	createTrackbar( "Smax", "Parameters", &filter[2], 255,on_trackbar);
	createTrackbar( "Smin", "Parameters", &filter[3], 255,on_trackbar);
	createTrackbar( "Vmax", "Parameters", &filter[4], 255,on_trackbar);
	createTrackbar( "Vmin", "Parameters", &filter[5], 255,on_trackbar);
	createTrackbar( "Thresh", "Parameters", &thresh, 1000,on_trackbar);

	// Waste 1000 frames : Let camera settle and adjust to brightness
	for(;;)
	{	

		clock_gettime(CLOCK_REALTIME, &tS);
		Camera >> img;	
		key = waitKey(1);
		imshow("Live Feed",img);
		if (key == 'q')
			break;
		clock_gettime(CLOCK_REALTIME, &tE);
    		cout << ": " <<tE.tv_nsec -  tS.tv_nsec<<endl;
		if(t<1000)
			time[t] = tE.tv_nsec -  tS.tv_nsec;
		else
			break;
		t++;

	}

for(i=0;i<1000;i++)
		out<<time[i]<<endl;
	out.close();
return 0;
	
	Mat imageresult(img.rows,img.cols, CV_8UC3, Scalar(0,0,0)); // Image containing all centroids of one trajectory

	while(true)
	{

    		
		imageresult = Scalar(0);
		x = 0; // Initialize centroid parameters to zero
		y = 0; // Initialize centroid parameters to zero
		whitecount = 0; // Initialize centroid parameters to zero

		Camera >> img; // Get a new frame
cout<<"hi"<<endl;
		cvtColor(img,imghsv,CV_BGR2HSV); // Convert colour to HSV
		cout<<"sedf";
		inRange(imghsv, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), imgbin); //Filter HSV image to bet Buinary image
		
		// Centroid calculation ------------------------------------------------
		for(unsigned int i=0; i<imgbin.rows; i++)
		{
  			for(unsigned int j=0; j<imgbin.cols; j++)
          		{	
				if(imgbin.data[i*imgbin.cols + j] > 200) // If colour is white
				{
					whitecount++; // Number of white pixels
					x += j; // 
					y += i; //
				}
			}
		}
		
		if(whitecount > thresh)
		{
			x /= whitecount;
			y /= whitecount;
			cx.push_back(x);
			cy.push_back(y);			
		}
		else
		{	
			x = 0;
			y = 0;
			cx.clear();
			cy.clear();
			imageresult = Scalar(0,0,0);
		}
		// ------------------------------------------------
		
		if(cx.size() > 1)

pos[t][0] = x;
pos[t][1] = y;
		{
			//line(imageresult, Point(cx[cx.size()-1],cy[cy.size()-1]), Point(cx[cx.size()-1],cy[cy.size()-1]), Scalar(255,255,255), 5,8);  
		}

		circle(img, Point(x, y), 1, Scalar(255,255,255), 2, 8, 0);
		cout<<"("<<x<<","<<y<<")"<<endl;
		imshow("Live Feed",img); // Show input RGB image
		imshow("Image HSV",imghsv); // Show input HSV image
		imshow("Filtered Image",imgbin); // Show filtered image
		                                                                                              
		imshow("Centroid Trajectory",imageresult); // Show centroid image

		key = waitKey(10); // Wait for 1ms for intrupt from user to exit
		if(key == 'q') // If user presses 'q'
			break; // Exit while loop

		clock_gettime(CLOCK_REALTIME, &tE);
    		cout << ": " <<tE.tv_nsec -  tS.tv_nsec<<endl;

		
		

	}

	cout<<"doine "<<t<<endl;
	for(i=0;i<209;i++)
		out<<time[i]<<endl;
	out.close();

	for(i=0;i<209;i++)
		out<<pos[i][0]<<","<<pos[i][1]<<endl;
	out.close();


    destroyAllWindows(); // Distroid all display windows
    return 0;
}
