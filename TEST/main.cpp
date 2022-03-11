#include "t0.h"
#include <iostream>
#include "sun.h"
#include "base.h" 
#include "vectorTest.h"
#include <vector>
#include <queue>
#include <array> 
#include <3DCamera.hpp> 
#include "CalibrationEyeToHand.h" 
#include "Singleton.h"
#include "GaussNewTon.h"
#include "LevenbergMarquardt.h"
#include "MySIFT.h"
#include "CirclePointDector.h"

using namespace std;

static double Func(Mat input, Mat params)
{
	//模型函数
	double t = input.at<double>(0, 0);
	double A = params.at<double>(0, 0);
	double B = params.at<double>(1, 0);
	return A * exp(B*t);
}
 

//-----------1

//void cb0() {
//	std::cout << "cb0\n";
//}
//
//void cb1(int i) {
//	std::cout << "cb1  " << i << "\n";
//}
//
//int cb2(int arg0, int arg1,int arg2) {
//	int sum = arg0 + arg1 + arg2;
//	std::cout << "cb2  " << sum << "\n";
//	return sum;
//}
//
//int cb3() {
//	std::cout << "cb3" << "\n";
//	return 234;
//}

int main() { 
	 
//----------11

	//Mat image = imread("tt.png", 1);
	//imshow("0", image);
	//CirclePointDector::dector(image); 

//----------10 
	Mat image = imread("C:/Users/dell/Desktop/icon.png", 1);
	//Mat image = imread("C:/Users/dell/Desktop/timg.jpg", 1);
	imshow("1", image);
	Mat grayImage, gaussImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY); 
	vector<vector<Mat>> octaves;
	MySIFT::GaussOctaves(grayImage, octaves, 3, 1.6, 3);
	vector<vector<Mat>> dogs;
	MySIFT::GaussDogs(octaves, dogs);
	vector<vector<Vec3f>> limitPoints;
	MySIFT::LimitPoints(dogs, limitPoints); 
	vector<vector<Vec3f>> exactPoints; 
	MySIFT::ExactPoints(dogs, limitPoints, exactPoints);
	vector<vector<Vec3f>> awayExactPoints;
	MySIFT::EdgeInfluence(dogs, exactPoints, awayExactPoints);

	int num = 0;
	for (int i = 0; i < awayExactPoints.size(); i++)
	{
		Mat temp = image.clone();
		for (int j = 0; j < awayExactPoints[i].size(); j++)
		{
			//cout << i << " " << j << "  " << exactPoints[i][j][0] << " " << exactPoints[i][j][1] << "\n";
			circle(image, Point(awayExactPoints[i][j][0]*image.cols, awayExactPoints[i][j][1] * image.rows), 2, Scalar(0, 0, 255));
			num++;
		}
		//imshow(to_string(i+2), temp);
	}
	cout <<"point NUM: " << num << "\n";
	imshow("11", image);
//-----------9

	//VideoCapture cap;
	//cap.open(0); //打开摄像头 
	//
	//if (!cap.isOpened())
	//	return 0;
	//cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH, 1920);
	//cap.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT,1080);
	//Size size(800, 600);
	//Mat frame;// (800, 600, CV_64FC3);
	//while (1)
	//{
	//	cap >> frame;//等价于cap.read(frame);
	//	resize(frame, frame, size);
	//	if (frame.empty())
	//		break;
	//	imshow("video", frame);
	//	if (waitKey(20) > 0)
	//		break;
	//} 
	//cap.release(); 

//-----------8 

	//Mat input(8, 1, CV_64F);
	//Mat output(8, 1, CV_64F);
	//Mat params(2, 1, CV_64F);
	//for (int i = 0; i < 8; i++)
	//{
	//	input.at<double>(i, 0) = i + 1;
	//}
	//output.at<double>(0, 0) = 8.3;
	//output.at<double>(1, 0) = 11.0;
	//output.at<double>(2, 0) = 14.7;
	//output.at<double>(3, 0) = 19.7;
	//output.at<double>(4, 0) = 26.7;
	//output.at<double>(5, 0) = 35.2;
	//output.at<double>(6, 0) = 44.4;
	//output.at<double>(7, 0) = 55.9; 
	//params.at<double>(0, 0) = 6;
	//params.at<double>(1, 0) = 6;
	// 
	////GaussNewTon::GaussNewton(input, output, params, 100, 0.00001, 0.0000001);
	//aa::LevenbergMarquardt::Lm(Func, input, output, params, 100, 0.00001, 0.0000001); 
	//std::cout <<"res: "<< params.at<double>(0, 0) << "  "<<params.at<double>(1, 0) << "\n";

#pragma region Test
	 
//-----------7

	//std::cout << Singleton::instance()->aa << "\n";

//-----------6
	
	//double hand_0[16] = { -0.526493,-0.839712,-0.132996,-103.625,-0.552518,0.456842,-0.697151,-617.876,0.646164,-0.293562,-0.70448,358.947,0,0,0,1 };
	//double hand_1[16] = { 0.0571415,0.0282277,-0.997967,-161.265,-0.326202,0.945266,0.00805938,-521.358,0.943571,0.325078,0.0632218,451.887,0,0,0,1 };
	//double hand_2[16] = { -0.556786,-0.177671,0.811433,-30.6774,-0.804539,0.358378,-0.473585,-589.702,-0.206657,-0.916514,-0.342483,419.595,0,0,0,1 };
	//double hand_3[16] = { -0.338188, -0.790614, -0.51045, 243.249, 0.306364, 0.42037, -0.854067, -456.846, 0.889815, -0.445218, 0.100051, 436.491,0,0,0,1};
	//double hand_4[16] = { 0.448312, -0.806878, 0.384661, 187.83, -0.51981, 0.114755, 0.846539, -384.533, -0.727196, -0.579465, -0.367977, 612.997,0,0,0,1 };
	//
	//double eye_0[] = { 0.060747,-0.923803,-0.378019,-161.9,0.9603,-0.0492166,0.274594,-322.8,-0.272276,-0.379692,0.884138,-1496.36,0,0,0,1 };
	//double eye_1[] = { 0.9469,-0.285689,-0.147522,-312.68,0.304938,0.943417,0.130296,-450.39,0.10195,-0.168362,0.980439,-1530.09,0,0,0,1 };
	//double eye_2[] = { -0.817418,-0.575813,-0.0163465,-212.99,0.57498,-0.813853,-0.0839086,-242.54,0.0350121,-0.0779873,0.996339,-1533.13,0,0,0,1 };
	//double eye_3[] = { 0.457025, -0.083748, -0.885502, -193.61, 0.302377, 0.950892, 0.0661303, -61.39, 0.836478, -0.297978, 0.459905, -1536.66 ,0,0,0,1};
	//double eye_4[] = { -0.742842, -0.162787, 0.649374, -271.12, 0.511379, -0.763988, 0.393465, 19.28, 0.432063, 0.624358, 0.650768, -1564.22 ,0,0,0,1};
	//
	//vector<double*> hand;
	//vector<double*> eye;
	//
	//hand.push_back(hand_0);
	//hand.push_back(hand_1);
	//hand.push_back(hand_2);
	////hand.push_back(hand_3);
	////hand.push_back(hand_4);
	//
	//eye.push_back(eye_0);
	//eye.push_back(eye_1);
	//eye.push_back(eye_2);
	////eye.push_back(eye_3);
	////eye.push_back(eye_4);
	//
	//double out[16];
	//CalibrationEyeToHand::EyeToHand(hand, eye, out);
	//for (size_t i = 0; i < 16; i++)
	//{
	//	cout << out[i] << "  ";
	//}

//-----------5 
 
	//printf("This is SamplePointReconstruction sample!\n\n");
	//int ret;
	//
	//// get camera pointer and connect a valid camera
	//cs::ICameraPtr camera = cs::getCameraPtr();
	//ret = camera->connect();
	//if (ret != SUCCESS)
	//{
	//	printf("camera connect failed(%d)!\n", ret);
	//	return -1;
	//}
	////start depth stream
	//ret = camera->startStream(STREAM_TYPE_DEPTH, { STREAM_FORMAT_Z16, 640, 400, 2 });
	//if (ret != SUCCESS)
	//{
	//	printf("camera start depth stream failed(%d)!\n", ret);
	//	return -1;
	//}
	////start rgb stream
	//std::vector<StreamInfo> streamInfos;
	//camera->getStreamInfos(STREAM_TYPE_RGB, streamInfos);
	//for (auto info : streamInfos)
	//{
	//	if (info.format == STREAM_FORMAT_RGB8)
	//	{
	//		ret = camera->startStream(STREAM_TYPE_RGB, info);
	//		if (ret != SUCCESS)
	//		{
	//			printf("camera start rgb stream failed(%d)!\n", ret);
	//			return -1;
	//		}
	//	}
	//}
	//
	//int count = 1;
	//while (count--)
	//{
	//	cs::IFramePtr frameDepth, frameRgb;
	//	if (SUCCESS != camera->getFrame(STREAM_TYPE_DEPTH, frameDepth))
	//		continue;
	//	printf("Get a new Frame!\n");
	//	cs::Pointcloud pc;
	//	Intrinsics intr;
	//	camera->getIntrinsics(STREAM_TYPE_DEPTH, intr);
	//	camera->setProperty(STREAM_TYPE_DEPTH, PROPERTY_EXPOSURE, 4000);
	//	float scale = 0.1f;
	//	PropertyExtension value;
	//	if (SUCCESS == camera->getPropertyExtension(PROPERTY_EXT_DEPTH_SCALE, value))
	//	{
	//		scale = 0.02;//value.depthScale;
	//	}
	//	if (SUCCESS == camera->getFrame(STREAM_TYPE_RGB, frameRgb))
	//	{
	//		Intrinsics intrColor;
	//		camera->getIntrinsics(STREAM_TYPE_RGB, intrColor);
	//		Extrinsics extrColor;
	//		camera->getExtrinsics(extrColor); 
	//
	//		Distort pro;
	//		camera->getDistort(STREAM_TYPE_DEPTH, pro); 
	//
	//		pc.generatePoints((unsigned short *)frameDepth->getData(), frameDepth->getWidth(), frameDepth->getHeight(), scale, &intr, &intrColor, &extrColor);
	//		pc.exportToFile("./pc.ply", (unsigned char *)frameRgb->getData(), frameRgb->getWidth(), frameRgb->getHeight());
	//	}
	//	else
	//	{
	//		pc.generatePoints((unsigned short *)frameDepth->getData(), frameDepth->getWidth(), frameDepth->getHeight(), scale, &intr, nullptr, nullptr);
	//		pc.exportToFile("./pc.ply", nullptr, 0, 0);
	//	}
	//}
	//
	//// disconnect camera
	//ret = camera->disconnect();
	//if (ret != SUCCESS)
	//{
	//	printf("camera disconnect failed(%d)!\n", ret);
	//	return -1;
	//}
	//

//-----------4

	//vectorTest* vt = new vectorTest();
	//std::vector<double> p = { 3,4,5 };
	//vt->test(p);
	//vt->print();


//-----------3

	//int A[5] = { 1,2,3,5,6 };
	//std::cout << A[3] << "--" << *(A+3) << "\n";

//-----------2

	//base* b = new base();
	//sun* sun1 = new sun();
	//sun1->ttest();

//-----------1

	//t0* t1 = new t0();
	//
	//decltype(t1) t = new t0();
	//
	//t->t0_F0(t->test);
	//t->t0_F1();
	//
	//t->cb0 = std::bind(cb0);
	//t->cb1 = std::bind(cb1, std::placeholders::_1);
	//t->cb2 = std::bind(cb2, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);//std::placeholders::_1 指向第一个形参
	//t->cb3 = std::bind(cb3);
	//t->cb4 = std::bind(&t0::t_cb4, t);
	//t->cb5 = std::bind(cb2, std::placeholders::_1, 2, 3);
	//
	//t->t0_F2();
	//t->t0_F3(t->cb1);
	//t->t0_F4(t->cb2);
	//t->t0_F5();
	//t->t0_F6();
	//t->t0_F7();

#pragma endregion

	waitKey(0);
	return 0;
}
