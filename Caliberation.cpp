#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
// Use sstream to create image names including integer
#include <sstream>
#include <ctime>

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using GenApi objects
using namespace GenApi;

// Namespace for using opencv objects.
using namespace cv;

// Namespace for using cout.
using namespace std;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 2;


//#define CAMERA0 0
//#define CAMERA1 1

float data1[2][2];
float data2[2][2];
unsigned int Rclick, Lclick;

//void HSVTuning( int i)
//{
//	cout<<"HSV Tunning of Camera "<<i<<endl;
//    Mat openCvImage, openCvImageHSV, openCvImageBIN;
//	char key;
//	int filter[6];
//    camera.StartGrabbing(GrabStrategy_LatestImageOnly);



//    // This smart pointer will receive the grab result data.
//    CGrabResultPtr ptrGrabResult;
//    CPylonImage pylonImage;

//	namedWindow("Parameters"); // Windoe to display Parameter trackbar
//	createTrackbar( "Hmax", "Parameters", &filter[0], 255, NULL);
//	createTrackbar( "Hmin", "Parameters", &filter[1], 255, NULL);
//	createTrackbar( "Smax", "Parameters", &filter[2], 255, NULL);
//	createTrackbar( "Smin", "Parameters", &filter[3], 255, NULL);
//	createTrackbar( "Vmax", "Parameters", &filter[4], 255, NULL);
//	createTrackbar( "Vmin", "Parameters", &filter[5], 255, NULL);
	
//	for(;;)
//    {
//        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

//        cvtColor(openCvImage, openCvImageHSV, CV_BGR2HSV); // Convert colour to HSV

//        inRange(openCvImageHSV, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), openCvImageBIN); //Filter HSV image to bet Buinary image

//        key = waitKey(5);
//        if (key == 'q')
//            break;
//      imshow("Camera", openCvImage);
//        imshow("Camera HSV", openCvImageHSV);
//        imshow("Camera BIN", openCvImageBIN);
//	}
//	destroyAllWindows();
//}

//void OrientationOfCamera(unsigned int i, CInstantCamera camera)
//{
//    //cout<<"Check Orientation of Camera "<<i<<endl;
//    Mat openCvImage;
//	char key;
//	namedWindow("Camera 0"); // Window to display input RGB image
	
//	for(;;)
//	{
//        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());
//		key = waitKey(5);
//		line(frame, Point(320, 0), Point(320, 480), Scalar(0,0,255), 1, 8);
//		if (key == 'q')
//			break;
//        imshow("Camera 0", openCvImage);
//	}
//	destroyAllWindows();
//}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     	if  ( event == EVENT_RBUTTONDOWN )
     	{
		cout << "mouse is R clicked - position (" << x << ", " << y << ")" << endl;
		data1[Rclick][0] = x;
		data1[Rclick][1] = y;
		Rclick++;
     	}
 
    	if  ( event == EVENT_LBUTTONDOWN )
     	{
          	cout << "mouse is L clicked - position (" << x << ", " << y << ")" << endl;
		data2[Lclick][0] = x;
		data2[Lclick][1] = y;
		Lclick++;
     	}
}

bool Intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
{
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < 1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    return true;
}



void OriginOfThread(VideoCapture &camera)
{
	unsigned short int i;
    Mat frame;
	Rclick = Lclick = 0;
	char key = 'r';
	namedWindow("Camera"); // Window to display input RGB image
	
	cout<<" Press any key to record video, please swing the ball..."<<endl;
	VideoWriter video;
	video.open("out2.avi", CV_FOURCC('M','J','P','G'), 30, Size(camera.get(CV_CAP_PROP_FRAME_WIDTH), camera.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
	key = waitKey(0);
	for(i=0; i<750; i++)
	{
		camera >> frame;
		video << frame;
		cout<<i<<endl;
		key = waitKey(10);
		if (key == 'q')
			break;
		imshow("Camera", frame);

	}
	cout<<endl;
	
	cout<<" Video captured..."<<endl;
	video.release();

	Point2f r;
	VideoCapture videoip;
	videoip.open("out.avi");

	setMouseCallback("Camera", CallBackFunc, NULL);

	for(;;)
	{	
		videoip >> frame;
		imshow("Camera",frame);
		key = waitKey(0);
		if (key == 'q')
			break;
		else if(key == 'n')
		{
			videoip >> frame;	
		}
		
		else if(key == 'c')
		{
			Intersection(Point2f(data1[0][0],data1[0][1]), Point2f(data1[1][0],data1[1][1]), Point2f(data2[0][0],data2[0][1]), Point2f(data2[1][0],data2[1][1]), r);
			cout<<r<<endl;
			Rclick = 0;
			Lclick = 0;
		} 
	}
}

void LengthOfThread(VideoCapture &camera)
{
	VideoCapture videoip;
	Mat frame;
	videoip.open("out.avi");
	float origin[2];
	cout<<" Enter center ";
	cin>>origin[0]>>origin[1];

	setMouseCallback("Camera", CallBackFunc, NULL);
	char key = 'r';
	for(;;)
	{	
		Rclick = 0;
		videoip >> frame;
		imshow("Camera",frame);
		key = waitKey(0);
		if (key == 'q')
			break;
		else if(key == 'n')
		{
			cout<<sqrt((data1[0][0] - origin[0])*(data1[0][0] - origin[0]) + (data1[0][1] - origin[1])*(data1[0][1] - origin[1]))<<endl;	
		}
	}
}

int main(int argc, char** argv)
{
    std::clock_t start,start1,start2,start3,start4;
    double duration,duration1,duration2,duration3,duration4;
	// OpenCV -------------------------
    //VideoCapture Camera;
    Pylon::PylonAutoInitTerm autoInitTerm;
    PylonInitialize();
  //  Mat openCvImage, openCvImageHSV, openCvImageBIN;
	char key;
    CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
    // The parameter MaxNumBuffer can be used to control the count of buffers
    // allocated for grabbing. The default value of this parameter is 10.
    camera.MaxNumBuffer = 1;
    start = clock();
// create pylon image format converter and pylon image
    CImageFormatConverter formatConverter;
    formatConverter.OutputPixelFormat= PixelType_BGR8packed;
    CPylonImage pylonImage;
    duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"printf: Converter "<< duration <<'\n';

// Create an OpenCV image
    Mat theFrame, openCvImage, openCvImageHSV, openCvImageBIN;

    // Start the grabbing of c_countOfImagesToGrab images.
    // The camera device is parameterized with a default configuration which
    // sets up free-running continuous acquisition.
    //camera.StartGrabbing(GrabStrategy_LatestImageOnly);



    // This smart pointer will receive the grab result data.
    //CGrabResultPtr ptrGrabResult;



    // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
    // when c_countOfImagesToGrab images have been retrieved.
//    while ( camera.IsGrabbing())
//    {
//        // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
      //camera.RetrieveResult(500, ptrGrabResult, TimeoutHandling_ThrowException);

//        // Image grabbed successfully?
//        if (ptrGrabResult->GrabSucceeded())
//        {
//            // Access the image data.
//            cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
//            cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
//            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
//            cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;

//    // Convert the grabbed buffer to pylon imag
//    formatConverter.Convert(pylonImage, ptrGrabResult);
//    // Create an OpenCV image out of pylon image
//    openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

//	Camera[0].open(CAMERA0);
//	if(!Camera[0].isOpened())  // check if we succeeded
//    	{
//        	cout<<" Camera 0 is not Opened..."<<endl;
//            		return -1;
//    	}
	
//    Camera.open(CAMERA1);
//    if(!Camera.isOpened())  // check if we succeeded
//    	{
//        	cout<<" Camera 1 is not Opened..."<<endl;
//            		return -1;
//    	}

	// C++ -------------------------
    int i=1;
	
	
	
//	cout<<" ===Orientation Caliberation==="<<endl;	
//	for(i=0; i<2; i++)
	//{
	//	OrientationOfCamera(i, Camera[i]);
	//	cout<<" Orientation of camera "<<i<<" Done!!!"<<endl;
	//}
	
//	cout<<" ===Length Caliberation"<<endl;
//	cout<<" Which camera to use for origin calculation? ";
//	cin>>i;
//	OriginOfThread(Camera[i]);

//	cout<<" ===Distance Caliberation"<<endl;
//	LengthOfThread(Camera[i]);
	
    cout<<" ===HSV Caliberation==="<<endl;
//    for(i=1; i<1; i++)
//	{
    cout<<"HSV Tunning of Camera "<<i<<endl;
   //Mat openCvImage, openCvImageHSV, openCvImageBIN;
    //char key;
    int filter[6];
    camera.StartGrabbing(GrabStrategy_LatestImageOnly);



    // This smart pointer will receive the grab result data.
    CGrabResultPtr ptrGrabResult;
   // CPylonImage pylonImage;

    namedWindow("Parameters"); // Windoe to display Parameter trackbar
    createTrackbar( "Hmax", "Parameters", &filter[0], 255, NULL);
    createTrackbar( "Hmin", "Parameters", &filter[1], 255, NULL);
    createTrackbar( "Smax", "Parameters", &filter[2], 255, NULL);
    createTrackbar( "Smin", "Parameters", &filter[3], 255, NULL);
    createTrackbar( "Vmax", "Parameters", &filter[4], 255, NULL);
    createTrackbar( "Vmin", "Parameters", &filter[5], 255, NULL);

    for(;;)
    {
        start1 = clock();
        // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
        camera.RetrieveResult(500, ptrGrabResult, TimeoutHandling_ThrowException);
        duration1 = ( clock() - start1 ) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: retrieve "<< duration1 <<'\n';

        // Image grabbed successfully?
        if (ptrGrabResult->GrabSucceeded())
        {
            const uint8_t *pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
            int frameCols = ptrGrabResult->GetWidth();
            int frameRows = ptrGrabResult->GetHeight();
            // Access the image data.
//            cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
//            cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
//            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
//            cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
            start2 = clock();
            formatConverter.Convert(pylonImage, ptrGrabResult);
            theFrame=cv::Mat(cv::Size(frameCols, frameRows), CV_8U, (void*)pImageBuffer);
            duration2 = ( clock() - start2 ) / (double) CLOCKS_PER_SEC;
            std::cout<<"printf: formatter "<< duration2 <<'\n';

            start3 = clock();
            cv::Mat openCvImage;
            theFrame.copyTo(openCvImage); // myFrame life cycle is now under your control
            // Create an OpenCV image out of pylon image
    //        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());
            theFrame.release();

    // Convert the grabbed buffer to pylon imag
//        formatConverter.Convert(pylonImage, ptrGrabResult);
    // Create an OpenCV image out of pylon image
//        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

    // Create a display window
//        namedWindow( "OpenCV Display Window", CV_WINDOW_NORMAL);//AUTOSIZE //FREERATIO
    // Display the current image with opencv
//        imshow( "OpenCV Display Window", openCvImage);
    // Define a timeout for customer's input in ms.
    // '0' means indefinite, i.e. the next image will be displayed after closing the window
    // '1' means live stream
       waitKey(1);
        }
        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

        cvtColor(openCvImage, openCvImageHSV, CV_BGR2HSV); // Convert colour to HSV

        inRange(openCvImageHSV, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), openCvImageBIN); //Filter HSV image to bet Buinary image
        duration3= (clock() - start3) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: hsv "<< duration3 <<'\n';

        start4 = clock();
//        key = waitKey(1);
        duration4= ( clock() - start4) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: key "<< duration4 <<'\n';
//        if (key == 'q')
//            break;
        imshow("Camera", openCvImage);
//        imshow("Camera HSV", openCvImageHSV);
        imshow("Camera BIN", openCvImageBIN);
    }
    destroyAllWindows();
    PylonTerminate();

}
//	}

	

	

//	return 0;

