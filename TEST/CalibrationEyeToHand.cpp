#include "CalibrationEyeToHand.h"


void CalibrationEyeToHand::EyeToHand(vector<double*> hand, vector<double*> eye, double out[16])
{
	vector<Mat> Ra;
	vector<Mat> Rb;
	vector<Mat> Ta;
	vector<Mat> Tb;

	for (int i = 0; i < hand.size(); i++)
	{
		double* r0 = hand[i%hand.size()];
		double* r1 = hand[(i+1)% hand.size()];
		double* n0 = eye[i%hand.size()];
		double* n1 = eye[(i + 1) % hand.size()];
		Mat tempA_0 = (Mat_<double>(4, 4, CV_64FC1) << r0[0], r0[1], r0[2], r0[3], r0[4], r0[5], r0[6], r0[7], r0[8], r0[9], r0[10], r0[11], r0[12], r0[13], r0[14], r0[15]);
		Mat tempA_1 = (Mat_<double>(4, 4, CV_64FC1) << r1[0], r1[1], r1[2], r1[3], r1[4], r1[5], r1[6], r1[7], r1[8], r1[9], r1[10], r1[11], r1[12], r1[13], r1[14], r1[15]);
		Mat tempb_0 = (Mat_<double>(4, 4, CV_64FC1) << n0[0], n0[1], n0[2], n0[3], n0[4], n0[5], n0[6], n0[7], n0[8], n0[9], n0[10], n0[11], n0[12], n0[13], n0[14], n0[15]);
		Mat tempb_1 = (Mat_<double>(4, 4, CV_64FC1) << n1[0], n1[1], n1[2], n1[3], n1[4], n1[5], n1[6], n1[7], n1[8], n1[9], n1[10], n1[11], n1[12], n1[13], n1[14], n1[15]);

		Mat tempA = tempA_0.inv() * tempA_1;
		Mat tempb = tempb_0.inv() * tempb_1;

		Ra.push_back((Mat_<double>(3, 3, CV_64FC1) << tempA.at<double>(0,0), tempA.at<double>(0, 1), tempA.at<double>(0, 2), tempA.at<double>(1, 0), tempA.at<double>(1, 1), tempA.at<double>(1, 2), tempA.at<double>(2, 0), tempA.at<double>(2, 1), tempA.at<double>(2, 2)));
		Rb.push_back((Mat_<double>(3, 3, CV_64FC1) << tempb.at<double>(0, 0), tempb.at<double>(0, 1), tempb.at<double>(0, 2), tempb.at<double>(1, 0), tempb.at<double>(1, 1), tempb.at<double>(1, 2), tempb.at<double>(2, 0), tempb.at<double>(2, 1), tempb.at<double>(2, 2)));

		Mat tempTa = (Mat_<double>(3, 1, CV_64FC1) << tempA.at<double>(0, 3), tempA.at<double>(1, 3), tempA.at<double>(2, 3));
		Mat tempTb = (Mat_<double>(3, 1, CV_64FC1) << tempb.at<double>(0, 3), tempb.at<double>(1, 3), tempb.at<double>(2, 3));

		Ta.push_back(tempTa);
		Tb.push_back(tempTb);
	}

	vector<vector<double>> ra;
	vector<vector<double>> rb;

	for (int i = 0; i < Ra.size(); i++)
	{
		ra.push_back(rodrigues(Ra[i]));
		rb.push_back(rodrigues(Rb[i]));
	}

	vector<vector<double>> pa;
	vector<vector<double>> pb;

	for (int i = 0; i < ra.size(); i++)
	{
		pa.push_back(amendRodrigues(ra[i]));
		pb.push_back(amendRodrigues(rb[i]));
	}
	
	Mat A;
	Mat b;

	for (int i = 0; i < pa.size(); i++)
	{
		double add[3] = { pa[i][0] + pb[i][0],pa[i][1] + pb[i][1], pa[i][2] + pb[i][2] };
		double sub[3] = { pb[i][0] - pa[i][0],pb[i][1] - pa[i][1], pb[i][2] - pa[i][2] };
		Mat tempA = (Mat_<double>(3, 3, CV_64FC1) << 0, -add[2], add[1], add[2], 0, -add[0], -add[1], add[0], 0);
		Mat tempb = (Mat_<double>(3, 1, CV_64FC1) << sub[0], sub[1], sub[2]);
		A.push_back(tempA);
		b.push_back(tempb);
 	}

	Mat A_i;
	invert(A, A_i, DECOMP_SVD); 
	Mat pcg = A_i * b;
	pcg = 2 * pcg / sqrt(1 + norm(pcg)*norm(pcg)); 
	Mat eyeM = Mat::eye(3, 3, CV_64FC1); 
	double px[3] = { pcg.at<double>(0,0),pcg.at<double>(1,0), pcg.at<double>(2,0) }; 
	Mat sknewPcg = (Mat_<double>(3, 3, CV_64FC1) << 0, -px[2], px[1], px[2], 0, -px[0], -px[1], px[0], 0); 
	Mat rcg = (1 - norm(pcg) * norm(pcg) / 2) * eyeM + 0.5 * (pcg*pcg.t() + sqrt(4 - norm(pcg)*norm(pcg)) * sknewPcg);

	Mat AA;
	Mat bb;
	for (int i = 0; i < Ra.size(); i++)
	{
		Mat tempAA = Ra[i] - eyeM;
		Mat tempbb = rcg * Tb[i] - Ta[i];
		AA.push_back(tempAA);
		bb.push_back(tempbb);
	}

	Mat AA_i;
	invert(AA, AA_i, DECOMP_SVD);
	Mat tcg = AA_i * bb;

	out[0] = rcg.at<double>(0, 0);
	out[1] = rcg.at<double>(0, 1);
	out[2] = rcg.at<double>(0, 2);
	out[3] = tcg.at<double>(0, 0);

	out[4] = rcg.at<double>(1, 0);
	out[5] = rcg.at<double>(1, 1);
	out[6] = rcg.at<double>(1, 2);
	out[7] = tcg.at<double>(1, 0);

	out[8] = rcg.at<double>(2, 0);
	out[9] = rcg.at<double>(2, 1);
	out[10] = rcg.at<double>(2, 2);
	out[11] = tcg.at<double>(2, 0);

	out[12] = 0;
	out[13] = 0;
	out[14] = 0;
	out[15] = 1;

}

vector<double> CalibrationEyeToHand::rodrigues(Mat m)
{
	double theta = (m.at<double>(0,0) + m.at<double>(1, 1) + m.at<double>(2, 2) - 1) / 2;
	theta = acos(theta);
	Mat mat_T = m.t();
	Mat mat(3, 3, CV_64FC1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.at<double>(i, j) = m.at<double>(i, j) - mat_T.at<double>(i, j);
			mat.at<double>(i, j) = mat.at<double>(i, j) / (2 * sin(theta));
		}
	} 
	return vector<double>{mat.at<double>(2, 1), mat.at<double>(0, 2), mat.at<double>(1, 0)};
}

vector<double> CalibrationEyeToHand::amendRodrigues(vector<double> v)
{
	float theta = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= theta;
	v[1] /= theta;
	v[2] /= theta;
	v[0] = 2 * sin(theta / 2) * v[0];
	v[1] = 2 * sin(theta / 2) * v[1];
	v[2] = 2 * sin(theta / 2) * v[2];
	return vector<double>{v[0], v[1], v[2]};
}
