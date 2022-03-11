#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

class GaussNewTon
{
public:


	static double Func(Mat input, Mat params) 
	{
		//模型函数
		double t = input.at<double>(0, 0);
		double A = params.at<double>(0, 0);
		double B = params.at<double>(1, 0);
		return A * exp(B*t);
	}

	static double Deriv(Mat input, Mat params, int n,double step)
	{
		//偏导
		Mat params1 = params.clone();
		Mat params2 = params.clone();
		params1.at<double>(n, 0) += step;
		params2.at<double>(n, 0) -= step;
		double p1 = Func(input, params1);
		double p2 = Func(input, params2);
		return (p1 - p2) / (2 * step);
	}

	static void GaussNewton(Mat input, Mat output, Mat &params,int maxIter,double step,double convergent)
	{
		int m = input.rows;
		int n = input.cols;
		int num_params = params.rows;

		Mat r(m, 1, CV_64F);//残差
		Mat Jf(m, num_params, CV_64F);//雅可比
		Mat inp(1, n, CV_64F);//单个输入数据

		double last_mse = 0;

		for (int i = 0; i < maxIter; i++)
		{
			double mse = 0;
			for (int x = 0; x < m; x++)
			{
				for (int y = 0; y < n; y++)
				{
					inp.at<double>(0, y) = input.at<double>(x, y);
				}

				r.at<double>(x, 0) = output.at<double>(x, 0) -Func(inp, params);

				mse += r.at<double>(x, 0)*r.at<double>(x, 0);

				for (int k = 0; k < num_params; k++)
				{
					Jf.at<double>(x, k) = Deriv(inp, params, k, step);
				}
			}

			mse /= m;

			if (fabs(last_mse - mse) < convergent)
			{
				break;
			}

			Mat delta = (Jf.t()*Jf).inv()*Jf.t()*r;
			params += delta;

			std::cout <<i<< " MSE: " << mse << "\n";

			last_mse = mse;
		}
		 
	}


private:

};
 
