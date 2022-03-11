#pragma once
#include <vector>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

class CalibrationEyeToHand
{
public:

	static void EyeToHand(vector<double*> hand, vector<double*> eye, double out[16]);

private:

	static vector<double> rodrigues(Mat m);
	static vector<double> amendRodrigues(vector<double> v);
};

