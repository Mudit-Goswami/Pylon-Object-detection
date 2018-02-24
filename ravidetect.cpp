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

int main(int argc, char* argv[])
{
    std::clock_t start,start1,start2,start3,start4;
    double duration,duration1,duration2,duration3,duration4;
    Mat openCvImageHSV, openCvImageBIN;

    char key;
    int filter[6] = {255, 0, 255, 0, 255, 225};
    // The exit code of the sample application.
    int exitCode = 0;


    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
    // is initialized during the lifetime of this object.
    Pylon::PylonAutoInitTerm autoInitTerm;
     PylonInitialize();
    try
    {
        // Create an instant camera object with the camera device found first.
        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
        // Print the model name of the camera.
     cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
        // Create an instant camera object with the camera device found first.
       // CDeviceInfo info;
    //info.SetSerialNumber("21998726");
    //cout << "Creating Camera..." << endl;
    //CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
    //cout << "Camera Created." << endl;
        // Print the model name of the camera.
        //cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

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
    Mat theFrame, openCvImage;

        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing(GrabStrategy_LatestImageOnly);



        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;



        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
        while ( camera.IsGrabbing())
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
//                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
//                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
//                const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
//                cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
        start2 = clock();
        // Convert the grabbed buffer to pylon imag
        formatConverter.Convert(pylonImage, ptrGrabResult);
        theFrame=cv::Mat(cv::Size(frameCols, frameRows), CV_8U, (void*)pImageBuffer, cv::Mat::AUTO_STEP);
        duration2 = (clock() - start2 ) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: formatter "<< duration2 <<'\n';


        start3 = clock();
        cv::Mat openCvImage;
        theFrame.copyTo(openCvImage); // myFrame life cycle is now under your control
        // Create an OpenCV image out of pylon image
//        openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());
        theFrame.release();

        cvtColor(openCvImage, openCvImageHSV, CV_BGR2HSV); // Convert colour to HSV

        inRange(openCvImageHSV, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), openCvImageBIN); //Filter HSV image to bet Buinary image
        duration3= ( clock() - start3) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: hsv "<< duration3 <<'\n';

        start4 = clock();
     //   key = waitKey(1);
        duration4= (clock() - start4) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: key "<< duration4 <<'\n';
//        if (key == 'q')
//            break;
        imshow("Camera", openCvImage);
//        imshow("Camera HSV", openCvImageHSV);
        imshow("Camera BIN", openCvImageBIN);


        // Create a display window
//        namedWindow( "OpenCV Display Window", CV_WINDOW_NORMAL);//AUTOSIZE //FREERATIO
        // Display the current image with opencv
//        imshow( "OpenCV Display Window", openCvImage);
        // Define a timeout for customer's input in ms.
        // '0' means indefinite, i.e. the next image will be displayed after closing the window
        // '1' means live stream
        waitKey(1);

            }
            else
            {
                cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
        openCvImage.release();
//        openCvImageHSV.release();
        openCvImageBIN.release();
        //camera.release();
    }
    catch (GenICam::GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press Enter to exit." << endl;
    while( cin.get() != '\n');

    return exitCode;
    PylonTerminate();
}












//#define CAMERA0 0

//int main()
//{
//	VideoCapture Camera;
//	Mat frame, frameHSV, frameBIN;

//	char key;
//	int filter[6] = {255, 0, 255, 0, 255, 225};

//	Camera.open(CAMERA0);
//	if(!Camera.isOpened())  // check if we succeeded
//    	{
//        	cout<<" Camera 0 is not Opened..."<<endl;
//            		return -1;
//    	}

//	for(;;)
//	{
//		Camera >> frame;
//		cvtColor(frame, frameHSV, CV_BGR2HSV); // Convert colour to HSV
//		inRange(frameHSV, Scalar(filter[1], filter[3], filter[5]), Scalar(filter[0], filter[2], filter[4]), frameBIN); //Filter HSV image to bet Buinary image


//		key = waitKey(5);
//		if (key == 'q')
//			break;
//		imshow("Camera", frame);
//		imshow("Camera HSV", frameHSV);
//		imshow("Camera BIN", frameBIN);
//	}

//	frame.release();
//	frameHSV.release();
//	frameBIN.release();
//	Camera.release();
//}

