#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

namespace aa {

	class LevenbergMarquardt
	{
	public:

		//static double Func(Mat input, Mat params)
		//{
		//	//模型函数
		//	double t = input.at<double>(0, 0);
		//	double A = params.at<double>(0, 0);
		//	double B = params.at<double>(1, 0);
		//	return A * exp(B*t);
		//}
		
		static double Deriv(double(*Func)(Mat input,Mat params), Mat input, Mat params, int n, double step)
		{
			Mat param1 = params.clone();
			Mat param2 = params.clone();

			param1.at<double>(n, 0) += step;
			param2.at<double>(n, 0) -= step;

			double p1 = Func(input, param1);
			double p2 = Func(input, param2);

			return (p1 - p2) / (2 * step);
		}

		static void Lm(double(*Func)(Mat input, Mat params), Mat inputs, Mat outputs, Mat& params, int maxIter, double step, double convergent)
		{
			int m = inputs.rows;
			int n = inputs.cols;
			int num_params = params.rows;

			Mat r(m, 1, CV_64F);//残差
			Mat r_temp(m, 1, CV_64F);//残差
			Mat Jf(m, num_params, CV_64F);//雅可比
			Mat inp(1, n, CV_64F);//单个输入数据
			Mat params_temp = params.clone();


			double last_mse = 0;
			double u = 1, v = 2;
			Mat I = Mat::eye(num_params, num_params, CV_64F);

			for (int i = 0; i < maxIter; i++)
			{
				double mse = 0;
				double mse_temp = 0;
				for (int x = 0; x < m; x++)
				{
					for (int y = 0; y < n; y++)
					{
						inp.at<double>(0, y) = inputs.at<double>(x, y);
					}

					r.at<double>(x, 0) = outputs.at<double>(x, 0) - Func(inp, params);

					mse += r.at<double>(x, 0)*r.at<double>(x, 0);

					for (int k = 0; k < num_params; k++)
					{
						Jf.at<double>(x, k) = Deriv(Func,inp, params, k, step);
					}
				}

				mse /= m;
				params_temp = params.clone();

				Mat delta = (Jf.t()*Jf + u * I).inv()*Jf.t()*r;
				params_temp += delta; 

				for (int x = 0; x < m; x++)
				{  
					r_temp.at<double>(x, 0) = outputs.at<double>(x, 0) - Func(inp, params_temp);

					mse_temp += r_temp.at<double>(x, 0)*r_temp.at<double>(x, 0);
				}

				mse_temp /= m;

				Mat q = (mse - mse_temp) / (0.5*delta.t()*(u*delta - Jf.t() * r));

				if (q.at<double>(0, 0) > 0)
				{
					double v1 = 1.0 / 3;
					double v2 = 1 - pow(2 * q.at<double>(0, 0) - 1, 3);
					double vmax = v1 > v2 ? v1 : v2;
					u = u * vmax;
					v = 2;
					params = params_temp;
				}
				else
				{
					u = u * v;
					v = 2 * v;
					params = params_temp;
				}

				if (fabs(last_mse - mse) < convergent)
				{
					break;
				} 

				std::cout << i << " MSE: " << mse << "\n";

				last_mse = mse;
			}
		}

	private:

	};
}
 
