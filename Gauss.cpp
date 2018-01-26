#include "Gauss.h"
#include <iostream>

Gauss::Gauss()
{
	int i, j;
	isAveCalc = false;
	isData = false;
	for (i = 0; i < 3; i++){
		ave[i] = 0;
		for (j = 0; j < 3; j++){
			sigma.at<double>(i, j) = 0;
		}
	}


}

void Gauss::dataSet(cv::Vec3d color, cv::Point p)
{
	int i = 0;
	isData = true;
	data.push_back(color);
	point.push_back(p);
}
void Gauss::dataClear(void)
{
	std::vector<cv::Vec3d>().swap(data);
	std::vector<cv::Point>().swap(point);
	isData = false;
}

void Gauss::calcAverage(void)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < 3; i++){
		ave[i] = 0;
		for (j = 0; j < (int)data.size(); j++){
			ave[i] += data[j][i];
		}
	}

	for (i = 0; i < 3; i++){
		ave[i] /= (double)data.size();
	}

	for (i = 0; i < (int)point.size(); i++){
		p_ave += point[i];
	}

	p_ave.x /= (double)point.size();
	p_ave.y /= (double)point.size();

	isAveCalc = true;

}

void Gauss::calcSigma(void)
{
	int x = 0;
	int y = 0;
	int n = 0;
	double xsum = 0;

	if (!isAveCalc){
		std::cout << "no average calc" << std::endl;
	}

	for (y = 0; y < 3; y++){
		for (x = 0; x < 3; x++){
			xsum = 0;
			for (n = 0; n < (int)data.size(); n++){
				xsum += data[n][x] * data[n][y];
			}
			xsum /= (double)data.size();
			sigma.at<double>(y, x) = xsum - (ave[x] * ave[y]);
		}
	}
}

void Gauss::output(void)
{
	if (!isAveCalc){
		std::cout << "no average calc" << std::endl;
	}

	std::cout << data.size() << std::endl;

	std::cout << "Sigma" << std::endl;
	std::cout << sigma << std::endl;

	std::cout << "average" << std::endl;
	for (int i = 0; i < 3; i++){
		std::cout << ave[i] << ", ";
	}
	std::cout << std::endl;
}

double Gauss::distribution(cv::Vec3d color)
{
	int i;
	double p = 0;
	cv::Mat x = (cv::Mat_<double>(3, 1));
	cv::Mat ex = (cv::Mat_<double>(1, 1));

	for (i = 0; i < 3; i++){
		x.at<double>(i, 0) = (double)color[i] - ave[i];
	}

	ex = x.t() * sigma.inv(cv::DECOMP_SVD) * x;

	ex /= -2;

	p = 1 / (pow(sqrt(2 * M_PI), 3) * sqrt(cv::determinant(sigma)));
	p *= exp(ex.at<double>(0, 0));

	return p;
}
