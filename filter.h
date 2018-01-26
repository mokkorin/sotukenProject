#include <opencv\cv.h>
#include <opencv\highgui.h>

#define M_PI 3.1415926535897932384626433832795


namespace filter
{
	double gauss(double x, double sigma);
	void bilateral(cv::Mat src, cv::Mat dst, std::vector<double> &fg, std::vector<double> &bg);
	void bilateral(cv::Mat src, cv::Mat dst, std::vector<double> &fg);
}