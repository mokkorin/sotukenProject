#include <opencv\cv.h>
#include <opencv\highgui.h>

#define M_PI 3.1415926535897932384626433832795

class Gauss
{
public:
	Gauss();
	void dataSet(cv::Vec3d color, cv::Point p);
	void dataClear(void);
	void calcAverage(void);
	void calcSigma(void);
	void output(void);
	double distribution(cv::Vec3d color);
	bool isAveCalc;
	bool isData;
	double ave[3];
	cv::Point p_ave;


protected:
	std::vector< cv::Vec3d > data;
	std::vector< cv::Point > point;


private:
	cv::Mat sigma = (cv::Mat_<double>(3, 3));

};
