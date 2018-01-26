#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Gauss.h"
#include <random>

#define CLUSTER_SIZE 5

class Cluster : public Gauss
{
public:
	double distance(cv::Vec3d color, cv::Point p, bool flag);
	bool isSmall(void);
private:
};


namespace kmeans
{
	void run(cv::Mat src, cv::Rect roi, std::vector<Cluster> &cluster);
	void run(cv::Mat src, cv::Mat mask, std::vector<Cluster> &cluster);
	void run(cv::Mat src, std::vector<Cluster> &cluster);
}