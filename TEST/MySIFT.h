#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class MySIFT {

public:

	//高斯核
	static Mat GaussKernel(int element, double segma)
	{
		Mat m(element, element, CV_64F);
		double A = 1 / (2 * CV_PI*segma*segma);
		int c = element / 2;
		double sum = 0;
		for (int i = 0; i < element; i++)
		{
			for (int j = 0; j < element; j++)
			{
				double t = -((i - c)*(i - c) + (j - c)*(j - c)) / (2 * segma*segma);
				double g = A * exp(t);
				m.at<double>(i, j) = g;
				sum += g;
			}
		}
		m /= sum;
		cout << m << "\n";
		return m;
	}

	//高斯模糊
	static void GaussImage(Mat& src, Mat& dst, Mat kernel)
	{
		Mat res = src.clone();
		for (int x = 0; x < src.cols; x++)
		{
			for (int y = 0; y < src.rows; y++)
			{
				double value[3] = { 0,0,0 };
				for (int i = 0; i < kernel.cols; i++)
				{
					for (int j = 0; j < kernel.rows; j++)
					{
						int a = i - kernel.cols / 2 + x;
						int b = j - kernel.rows / 2 + y;

						if (a < 0 || a >= src.cols || b < 0 || b >= src.rows)
						{
							continue;
						}

						if (src.channels() == 1)
						{
							value[0] += kernel.at<double>(i, j)*src.at<uchar>(a, b);
						}
						else if (src.channels() == 3)
						{
							value[0] += kernel.at<double>(i, j)*src.at<Vec3b>(a, b)[0];
							value[1] += kernel.at<double>(i, j)*src.at<Vec3b>(a, b)[1];
							value[2] += kernel.at<double>(i, j)*src.at<Vec3b>(a, b)[2];
						}

					}
				}
				if (src.channels() == 1)
				{
					res.at<uchar>(x, y) = value[0];
				}
				else if (src.channels() == 3)
				{
					res.at<Vec3b>(x, y)[0] = value[0];
					res.at<Vec3b>(x, y)[1] = value[1];
					res.at<Vec3b>(x, y)[2] = value[2];
				}
			}
		}
		dst = res;
	}

	//高斯模糊分轴
	static void GaussImageSeparation(Mat& src, Mat& dst, int ksize, double segma)
	{
		vector<double> kernel;
		double A = 1 / sqrt(2 * CV_PI*segma*segma);
		int c = ksize / 2;
		double sum = 0;
		for (int i = 0; i < ksize; i++)
		{
			double t = -(i - c)*(i - c) / (2 * segma*segma);
			double value = A * exp(t);
			sum += value;
			kernel.push_back(value);
		}
		for (int i = 0; i < kernel.size(); i++)
		{
			kernel[i] /= sum;
		}

		Mat res = src.clone();
		for (int x = 0; x < src.rows; x++)
		{
			for (int y = 0; y < src.cols; y++)
			{
				double value[3] = { 0,0,0 };
				for (int i = 0; i < ksize; i++)
				{
					int a = i - ksize / 2 + y;
					/*	if (a < 0 || a >= src.cols)
						{
							continue;
						}*/
					a = a < 0 ? a = 0 : a;
					a = a >= src.cols ? a = src.cols - 1 : a;
					if (src.channels() == 1)
					{
						value[0] += kernel[i] * src.at<uchar>(x, a);
					}
					else if (src.channels() == 3)
					{
						value[0] += kernel[i] * src.at<Vec3b>(x, a)[0];
						value[1] += kernel[i] * src.at<Vec3b>(x, a)[1];
						value[2] += kernel[i] * src.at<Vec3b>(x, a)[2];
					}
				}
				if (src.channels() == 1)
				{
					res.at<uchar>(x, y) = value[0];
				}
				else if (src.channels() == 3)
				{
					res.at<Vec3b>(x, y)[0] = value[0];
					res.at<Vec3b>(x, y)[1] = value[1];
					res.at<Vec3b>(x, y)[2] = value[2];
				}
			}
		}
		Mat res2 = res.clone();
		for (int x = 0; x < src.cols; x++)
		{
			for (int y = 0; y < src.rows; y++)
			{
				double value[3] = { 0,0,0 };
				for (int i = 0; i < ksize; i++)
				{
					int a = i - ksize / 2 + y;
					/*					if (a < 0 || a >= src.cols)
										{
											continue;
										} */
					a = a < 0 ? a = 0 : a;
					a = a >= src.rows ? a = src.rows - 1 : a;
					if (src.channels() == 1)
					{
						value[0] += kernel[i] * res.at<uchar>(a, x);
					}
					else if (src.channels() == 3)
					{
						value[0] += kernel[i] * res.at<Vec3b>(a, x)[0];
						value[1] += kernel[i] * res.at<Vec3b>(a, x)[1];
						value[2] += kernel[i] * res.at<Vec3b>(a, x)[2];
					}
				}
				if (src.channels() == 1)
				{
					res2.at<uchar>(y, x) = value[0];
				}
				else if (src.channels() == 3)
				{
					res2.at<Vec3b>(y, x)[0] = value[0];
					res2.at<Vec3b>(y, x)[1] = value[1];
					res2.at<Vec3b>(y, x)[2] = value[2];
				}
			}
		}
		dst = res2;
	}

	//高斯金字塔
	static void GaussOctaves(Mat& src, vector<vector<Mat>>& dst, int O, double segma0, int S)
	{
		Mat s = src;
		for (int i = 0; i < O; i++)
		{
			vector<Mat> d;
			if (dst.size() > 0)
			{
				resize(dst[dst.size() - 1][S], s, Size(s.cols / 2, s.rows / 2));
			}
			GaussOctave(s, d, i, segma0, S);
			dst.push_back(d);

		}
	}
	//单层高斯金字塔
	static void GaussOctave(Mat& src, vector<Mat>& dst, int o, double seg0, int S)
	{
		for (int i = 0; i < S + 3; i++)
		{
			double seg = seg0 * pow(2, o + i * 1.0 / S);
			int k = 11;// round(6 * seg + 1);
			Mat d;
			if (dst.size() == 0)
			{
				GaussImageSeparation(src, d, k, seg);
			}
			else
			{
				GaussImageSeparation(dst[dst.size() - 1], d, k, seg);
			}
			dst.push_back(d);
		}
	}

	//差分金字塔
	static void GaussDogs(vector<vector<Mat>>& src, vector<vector<Mat>>& dst)
	{
		for (int i = 0; i < src.size(); i++)
		{
			vector<Mat> dog;
			for (int j = 1; j < src[i].size(); j++)
			{
				Mat d;
				GaussD(src[i][j], src[i][j - 1], d);
				dog.push_back(d);
			}
			dst.push_back(dog);
		}
	}
	//差分
	static void GaussD(Mat& src1, Mat& src2, Mat& dst)
	{
		dst = src1.clone();
		for (int i = 0; i < src1.rows; i++)
		{
			for (int j = 0; j < src1.cols; j++)
			{
				if (src1.channels() == 1)
				{
					dst.at<uchar>(i, j) = /*abs*/(src1.at<uchar>(i, j) - src2.at<uchar>(i, j));
				}
				else
				{
					dst.at<Vec3b>(i, j)[0] = src1.at<Vec3b>(i, j)[0] - src2.at<Vec3b>(i, j)[0];
					dst.at<Vec3b>(i, j)[1] = src1.at<Vec3b>(i, j)[1] - src2.at<Vec3b>(i, j)[1];
					dst.at<Vec3b>(i, j)[2] = src1.at<Vec3b>(i, j)[2] - src2.at<Vec3b>(i, j)[2];
				}

			}
		}
	}

	//极值点
	static void LimitPoints(vector<vector<Mat>> src, vector<vector<Vec3f>>& dst)
	{
		for (int i = 0; i < src.size(); i++)
		{
			vector<Vec3f> d;
			for (int j = 1; j < src[i].size() - 1; j++)
			{
				LimitPoint(src[i][j], src[i][j - 1], src[i][j + 1], j, d);
			}
			dst.push_back(d);
		}
	}
	//某层极值点
	static void LimitPoint(Mat src, Mat up, Mat down, int layer, vector<Vec3f>& dst)
	{
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				double value = src.at<uchar>(i, j);
				bool max = true;
				bool min = true;
				for (int h = -1; h < 2; h++)
				{
					for (int v = -1; v < 2; v++)
					{
						if (!max && !min)
						{
							h = 2;
							break;
						}

						if (i + h < 0 || i + h >= src.rows || j + v < 0 || j + v >= src.cols)
							continue;

						if (value >= up.at<uchar>(i + h, j + v) || value >= down.at<uchar>(i + h, j + v))
							min = false;
						if (value <= up.at<uchar>(i + h, j + v) || value <= down.at<uchar>(i + h, j + v))
							max = false;
						if (h == 0 && v == 0)
						{

						}
						else
						{
							if (value >= src.at<uchar>(i + h, j + v))
								min = false;
							if (value <= src.at<uchar>(i + h, j + v))
								max = false;
						}
					}
				}
				if (max || min)
				{
					//Vec3f p(j*1.0/ src.cols, i*1.0/ src.rows, layer); 
					Vec3f p(j, i, layer);
					dst.push_back(p);
				}
			}
		}
	}

	//精确定位
	static void ExactPoints(vector<vector<Mat>> src, vector<vector<Vec3f>> points, vector<vector<Vec3f>>& dst)
	{
		for (int i = 0; i < points.size(); i++)
		{
			vector<Vec3f> d;
			for (int j = 0; j < points[i].size(); j++)
			{
				Vec3f p;
				if (ChoosePoint(src[i], points[i][j], p))
				{
					d.push_back(p);
				}
			}
			dst.push_back(d);
		}

	}

	static bool ChoosePoint(vector<Mat> dog, Vec3f p, Vec3f& outP)
	{
		int MAXITER = 5;

		int x = p[0];
		int y = p[1];
		int l = p[2];

		float Xc = 0, Xr = 0, Xl = 0;

		int c = dog[l].cols;
		int r = dog[l].rows;

		int i = 0;
		for (; i < MAXITER; i++)
		{
			int xl = fmax(x - 1, 0);
			int yl = fmax(y - 1, 0);
			int xr = fmin(c - 1, x + 1);
			int yr = fmin(r - 1, y + 1);
			Mat src = dog[l];
			Mat down = dog[l - 1];
			Mat up = dog[l + 1];

			Vec3f dD((src.at<uchar>(y, xr) - src.at<uchar>(y, xl)) / 2.0,
				(src.at<uchar>(yr, x) - src.at<uchar>(yl, x)) / 2.0,
				(up.at<uchar>(y, x) - down.at<uchar>(y, x)) / 2.0);

			float dxx = (src.at<uchar>(y, xr) + src.at<uchar>(y, xl) - src.at<uchar>(y, x) * 2) / 1.0;
			float dyy = (src.at<uchar>(yr, x) + src.at<uchar>(yl, x) - src.at<uchar>(y, x) * 2) / 1.0;
			float dll = (up.at<uchar>(y, x) + down.at<uchar>(y, x) - src.at<uchar>(y, x) * 2) / 1.0;
			float dxy = (src.at<uchar>(yr, xr) + src.at<uchar>(yl, xl) - src.at<uchar>(yr, xl) - src.at<uchar>(yl, xr)) / 4.0;
			float dxl = (up.at<uchar>(y, xr) + down.at<uchar>(y, xl) - up.at<uchar>(y, xl) - down.at<uchar>(y, xr)) / 4.0;
			float dyl = (up.at<uchar>(yr, x) + down.at<uchar>(yl, x) - up.at<uchar>(yl, x) - down.at<uchar>(yr, x)) / 4.0;

			float data[9] = { dxx,dxy,dxl,dxy,dyy,dyl,dxl,dyl,dll };
			Mat hes = Mat(3, 3, CV_32F, data);
			//X = -hes.inv * dD
			Vec3f X;
			solve(hes, dD, X, DECOMP_LU);

			Xc = -X[0];
			Xr = -X[1];
			Xl = -X[2];


			if (abs(Xc) < 0.5&&abs(Xr) < 0.5&&abs(Xl) < 0.5)
			{
				break;
			}

			x += round(Xc);
			y += round(Xr);
			l += round(Xl);

			if (x < 1 || x >= c - 1 || y < 1 || y >= r - 1 || l < 1 || l>3)
			{
				return false;
			}
		}

		if (i >= MAXITER)
		{
			return false;
		}

		{//|dx| = D + 1/2*dD*X               |dx|<0.03  (0.04/O)
			int xl = fmax(x - 1, 0);
			int yl = fmax(y - 1, 0);
			int xr = fmin(c - 1, x + 1);
			int yr = fmin(r - 1, y + 1);
			Mat src = dog[l];
			Mat down = dog[l - 1];
			Mat up = dog[l + 1];

			Vec3f dD((src.at<uchar>(y, xr) - src.at<uchar>(y, xl)) / 2.0,
				(src.at<uchar>(yr, x) - src.at<uchar>(yl, x)) / 2.0,
				(up.at<uchar>(y, x) - down.at<uchar>(y, x)) / 2.0);

			float t = dD.dot(Vec3f(Xc, Xr, Xl));
			float contr = src.at<uchar>(y, x) / 255.0 + t * 0.5; 
			if (abs(contr) < 0.04 / 3)
			{
				return false;
			}
		}

		//outP = Vec3f(1.0*x / c, 1.0*y / r, l);
		outP = Vec3f(x, y, l);

		return true;
	}

	//消除边缘响应
	static void EdgeInfluence(vector<vector<Mat>> src, vector<vector<Vec3f>> points, vector<vector<Vec3f>>& dst)
	{
		for (int i = 0; i < points.size(); i++)
		{
			int c = src[i][0].cols;
			int r = src[i][0].rows;
			vector<Vec3f> d;
			for (int j = 0; j < points[i].size(); j++)
			{ 
				if (HessianD(src[i], points[i][j]))
				{
					Vec3f p(1.0*points[i][j][0] / c, 1.0*points[i][j][1] / r, points[i][j][2]);
					d.push_back(p);
				}
			}
			dst.push_back(d);
		}
	}
	//Tr(H) * Tr(H) / Det(H) < (r+1)*(r+1)/r   (r=10) 
	static bool HessianD(vector<Mat> dog, Vec3f p)
	{
		int x = p[0];
		int y = p[1];
		int l = p[2];

		int c = dog[l].cols;
		int r = dog[l].rows;

		int xl = fmax(x - 1, 0);
		int yl = fmax(y - 1, 0);
		int xr = fmin(c - 1, x + 1);
		int yr = fmin(r - 1, y + 1);
		Mat src = dog[l];

		float dxx = (src.at<uchar>(y, xr) + src.at<uchar>(y, xl) - src.at<uchar>(y, x) * 2) / 1.0;
		float dyy = (src.at<uchar>(yr, x) + src.at<uchar>(yl, x) - src.at<uchar>(y, x) * 2) / 1.0;
		float dxy = (src.at<uchar>(yr, xr) + src.at<uchar>(yl, xl) - src.at<uchar>(yr, xl) - src.at<uchar>(yl, xr)) / 4.0;

		float tr = dxx + dyy;
		float det = dxx * dyy - dxy * dxy;
		float threhold = 10.0; 
		if (det <= 0 || (tr * tr *threhold) >= (threhold + 1)*(threhold + 1)*det)
		{
			return false;
		}
		return true;
	}
};
