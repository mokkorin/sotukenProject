#include "filter.h"
#include "Kmeans.h"

#define GENERATE 0

namespace my
{
	void resize(cv::Mat src)
	{
		// 出力画像
		cv::Mat dst;

		// 画像リサイズ
		cv::resize(src, dst, cv::Size(), 0.6, 0.6);

		cv::namedWindow("RESIZE", cv::WINDOW_AUTOSIZE);
		cv::imshow("RESIZE", dst);

		cv::imwrite("tori.jpg", dst);

	}

	void colorDiff(cv::Mat src)
	{
		int x, y, i, j;
		double r = 0, g = 0, b = 0, c = 0;
		double dr = 0, dg = 0, db = 0, dc = 0;
		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				
			}
		}
	}
};

void MouseCall(int event, int x, int y, int flags, void *data);

void g_cut(cv::Mat src, cv::Mat &dst, cv::Rect roi);
void g_cut(cv::Mat src, cv::Mat &dst, cv::Mat mask, cv::Mat mb);
void likeliHood(cv::Mat src, cv::Mat dst, std::vector<Cluster> cluster);
void likeliHood(cv::Mat src, cv::Mat dst, cv::Mat mask, std::vector<Cluster> cluster);
void likeliHood(cv::Mat src, std::vector<Cluster> cluster, std::vector<double> &like);


int main(void)
{
	// 入力画像
	cv::Mat src = cv::imread("tori.jpg", cv::IMREAD_UNCHANGED);
	cv::Point first(227, 189);
	cv::Point end(614, 536);
	cv::Mat maskF, copy, maskB, mask;
	cv::Mat gc;

	if (src.empty()) return 0;
		
	/*my::resize(src);
	cv::waitKey(0);
	return 0;*/
	//inu
	/*
	first.x = 131;
	first.y = 89;
	end.x = 429;
	end.y = 322;
	*/


	//hito2
	
	/*first.x = 455;
	first.y = 198;
	end.x = 689;
	end.y = 558;*/
	

	//tori
	
	first.x = 129;
	first.y = 89;
	end.x = 296;
	end.y = 264;
	

	//hana
	/*
	first.x = 185;
	first.y = 109;
	end.x = 401;
	end.y = 302;
	*/

	//hito1
	/*first.x = 658;
	first.y = 177;
	end.x = 753;
	end.y = 515;*/

	//risu
	/*first.x = 49;
	first.y = 349;
	end.x = 385;
	end.y = 633;*/

	cv::Mat b, g, r, bi, likeB, likeF;
	std::vector<cv::Mat> bgr;
	std::vector<cv::Mat> re;

	cv::Rect roi(first.x - 10, first.y - 10, end.x - first.x + 20, end.y - first.y + 20);
	cv::Mat fg_image = cv::Mat(src, roi);

	double ss[5] = { 30, 10, 150, 300, 400 };
	double sr[5] = { 1, 5, 10, 20, 50 };

	std::string file_name;
	
	//Cluster fore[CLUSTER_SIZE], back[CLUSTER_SIZE];
	std::vector<Cluster> fore(fg_image.cols + fg_image.rows);
	std::vector<Cluster> back(src.cols + src.rows);
	//std::vector<Cluster> fore(10);
	//std::vector<Cluster> back(10);


	/*cv::bilateralFilter(src, copy, 15, 100, 9, cv::BORDER_DEFAULT);

	copy.copyTo(src);*/


	cv::Mat sample = cv::Mat::ones(fg_image.rows, fg_image.cols, CV_8UC1) * 255;

	std::vector<double> bg(fg_image.cols * fg_image.rows, 0);
	std::vector<double> fg(fg_image.cols * fg_image.rows, 0);

	src.copyTo(copy);
	src.copyTo(gc);
	cv::rectangle(copy, roi, cv::Scalar(0, 0, 255), 2, 8);

	cv::cvtColor(fg_image, maskF, cv::COLOR_BGR2GRAY);
	cv::cvtColor(fg_image, likeF, cv::COLOR_BGR2GRAY);
	cv::cvtColor(fg_image, likeB, cv::COLOR_BGR2GRAY);

	cv::namedWindow("main", cv::WINDOW_AUTOSIZE);
	cv::imshow("main", copy);
	cv::waitKey(0);
	kmeans::run(src, roi, back);
	
	likeliHood(fg_image, back, fg);

	/*std::vector<Cluster> cluster(10);
	for (int i = 0; i < 10; i++){
		if (i == 3){
			cluster[i].dataSet(cv::Vec3d(0, 0, 0));
		}
	}
	std::cout << cluster.size() << std::endl;
	*/
	//kmeans::run(fg_image, sample, fore);
	//
	//std::cout << "run k-means method" << std::endl;
	//kmeans::run(src, roi, back);

	//std::cout << "run likelihood method" << std::endl;
	
	likeliHood(fg_image, likeF, back);
	cv::namedWindow("fore", cv::WINDOW_AUTOSIZE);
	cv::imshow("fore", likeF);
	/*g_cut(src, gc, roi);
	cv::namedWindow("grabcut-roi", cv::WINDOW_AUTOSIZE);
	cv::imshow("grabcut-roi", gc);
	fg_image = gc(roi);
	cv::cvtColor(gc, mask, CV_BGR2GRAY);
	cv::threshold(mask, mask, 1, 255, cv::THRESH_BINARY_INV);
	cv::namedWindow("mb", cv::WINDOW_AUTOSIZE);
	cv::imshow("mb", mask);
	cv::waitKey(0);

	maskB = mask(roi);
	cv::Mat gcm;
	fg_image.copyTo(gcm);
	g_cut(fg_image, gcm, maskF, maskB);
	cv::namedWindow("grabcut-mask", cv::WINDOW_AUTOSIZE);
	cv::imshow("grabcut-mask", gcm);*/
	//kmeans::run(fg_image, likeF, fore);

	/*cv::namedWindow("back", cv::WINDOW_AUTOSIZE);
	char c;
	for (c = 0; c != 27; c = cv::waitKey(0)){
		likeliHood(fg_image, likeB, mask, back);
		cv::threshold(likeB, mask, 0, 255, cv::THRESH_OTSU);
		cv::imshow("back", mask);
	}*/
	//kmeans::run(likeh, fore);

	
	for (int i = 0; i < fore.size(); i++){
		fore[i].dataClear();
		back[i].dataClear();
	}

	cv::bilateralFilter(src, bi, 15, 100, 9, cv::BORDER_DEFAULT);

	cv::namedWindow("honke", cv::WINDOW_AUTOSIZE);
	cv::imshow("honke", bi);

	b = cv::Mat::zeros(fg_image.rows, fg_image.cols, CV_8UC1);
	g = cv::Mat::zeros(fg_image.rows, fg_image.cols, CV_8UC1);
	r = cv::Mat::zeros(fg_image.rows, fg_image.cols, CV_8UC1);

	cv::split(fg_image, bgr);

	filter::bilateral(bgr[0], b, fg);
	filter::bilateral(bgr[1], g, fg);
	filter::bilateral(bgr[2], r, fg);

	re.push_back(b);
	re.push_back(g);
	re.push_back(r);

	cv::merge(re, fg_image);

	cv::namedWindow("filter", cv::WINDOW_AUTOSIZE);
	cv::imshow("filter", fg_image);
	g_cut(src, mask, roi);
	cv::namedWindow("g-cut-f", cv::WINDOW_AUTOSIZE);
	cv::imshow("g-cut-f", mask);
	
	g_cut(gc, mask, roi);
	cv::namedWindow("g-cut", cv::WINDOW_AUTOSIZE);
	cv::imshow("g-cut", mask);

#if GENERATE == 1
	int i, j;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 5; j++){
			cv::bilateralFilter(src, honke, 15, ss[i], sr[j], cv::BORDER_DEFAULT);
			file_name = "inu\\s" + std::to_string((int)ss[i]) + "r" + std::to_string((int)sr[j]) + ".jpg";
			cv::imwrite(file_name, honke);
		}
	}
#elif GENERATE == 2	
	int i, j;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 5; j++){
			file_name = "inu\\s" + std::to_string((int)ss[i]) + "r" + std::to_string((int)sr[j]) + ".jpg";
			gc = cv::imread(file_name, cv::IMREAD_UNCHANGED);
			file_name = "inu\\cut\\s" + std::to_string((int)ss[i]) + "r" + std::to_string((int)sr[j]) + ".jpg";
			g_cut(gc, roi, file_name);
		}
	}
#endif

	std::cout << "Press any key..." << std::endl;
	cv::waitKey(0);

	cv::destroyAllWindows();
	
	return 0;
}
void MouseCall(int event, int x, int y, int flags, void *data)
{
	cv::Mat *ptr;
	ptr = static_cast<cv::Mat*> (data);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		std::cout << (int)(ptr->data[y*ptr->step + x*ptr->elemSize()]) << std::endl;
		break;
	default:
		break;
	}

}

void g_cut(cv::Mat src, cv::Mat &dst, cv::Rect roi)
{
	cv::Mat mask, bg, fg;

	roi.x += 10;
	roi.y += 10;
	roi.width -= 20;
	roi.height -= 20;

	cv::grabCut(src, dst, roi, bg, fg, 1, cv::GC_INIT_WITH_RECT);
	cv::compare(dst, cv::GC_PR_FGD, mask, cv::CMP_EQ);
	src.copyTo(dst, mask);
}

void g_cut(cv::Mat src, cv::Mat &dst, cv::Mat mask, cv::Mat mb)
{
	cv::Mat fore, bg, fg;
	cv::Rect roi;
	int x, y;

	mask.copyTo(fore);
	for (y = 0; y < mask.rows; y++){
		for (x = 0; x < mask.cols; x++){
			if (mask.data[y*mask.step + x*mask.elemSize()] == 255){
				fore.data[y*mask.step + x*mask.elemSize()] = cv::GC_FGD;
			}
		}
	}

	for (y = 0; y < mask.rows; y++){
		for (x = 0; x < mask.cols; x++){
			if (mb.data[y*mb.step + x*mb.elemSize()] == 255){
				fore.data[y*fore.step + x*fore.elemSize()] = cv::GC_BGD;
			}
		}
	}


	cv::grabCut(src, fore, roi, bg, fg, 1, cv::GC_INIT_WITH_MASK);

	for (y = 0; y < fore.rows; y++){
		for (x = 0; x < fore.cols; x++){
			if (fore.data[y*fore.step + x*fore.elemSize()] == cv::GC_BGD || fore.data[y*fore.step + x*fore.elemSize()] == cv::GC_PR_BGD){
				dst.data[y*dst.step + x*dst.elemSize()] = 0;
				dst.data[y*dst.step + x*dst.elemSize() + 1] = 0;
				dst.data[y*dst.step + x*dst.elemSize() + 2] = 0;
			}
		}
	}
}

//likelihood to image
void likeliHood(cv::Mat src, cv::Mat dst, std::vector<Cluster> cluster)
{
	int i, j, x, y;
	cv::Vec3d color;
	double p = 0;
	double max = 0;
	double min = 999999;

	for (y = 0; y < src.rows; y++){
		for (x = 0; x < src.cols; x++){

			color[0] = (double)src.data[y * src.step + x * src.elemSize()];
			color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
			color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

			p = 0;
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isAveCalc){
					p += cluster[i].distribution(color);
				}
			}
			p /= (double)cluster.size();
			p = -1 * std::log(p);
			if (max < p){
				max = p;
			}
			if (min > p){
				min = p;
			}
		}
	}

	for (y = 0; y < src.rows; y++){
		for (x = 0; x < src.cols; x++){
			
			color[0] = (double)src.data[y * src.step + x * src.elemSize()];
			color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
			color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

			p = 0;
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isAveCalc){
					p += cluster[i].distribution(color);
				}
			}
			p /= (double)cluster.size();
			p = -1 * std::log(p);
			p = ((p - min) / (max - min)) * 255;

			if (p >= 255){
				p = 255;
			}
			dst.data[y*dst.step + x*dst.elemSize()] = (uchar)p;
		}
	}
	

	std::cout << max << std::endl;
	std::cout << min << std::endl;

}

//likelihood to image with mask
void likeliHood(cv::Mat src, cv::Mat dst, cv::Mat mask, std::vector<Cluster> cluster)
{
	int i, j, x, y;
	cv::Vec3d color;
	double p = 0;
	double max = 0;
	double min = 999999;

	for (y = 0; y < src.rows; y++){
		for (x = 0; x < src.cols; x++){
			if (mask.data[y * mask.step + x * mask.elemSize()] == 255)	continue;
			color[0] = (double)src.data[y * src.step + x * src.elemSize()];
			color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
			color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

			p = 0;
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isAveCalc){
					p += cluster[i].distribution(color);
				}
			}
			p /= (double)cluster.size();
			p = -1 * std::log(p);
			if (max < p){
				max = p;
			}
			if (min > p){
				min = p;
			}
		}
	}

	for (y = 0; y < src.rows; y++){
		for (x = 0; x < src.cols; x++){
			if (mask.data[y * mask.step + x * mask.elemSize()] == 255){
				dst.data[y*dst.step + x*dst.elemSize()] = 255;
			}
			else{
				color[0] = (double)src.data[y * src.step + x * src.elemSize()];
				color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
				color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

				p = 0;
				for (i = 0; i < (int)cluster.size(); i++){
					if (cluster[i].isAveCalc){
						p += cluster[i].distribution(color);
					}
				}
				p /= (double)cluster.size();
				p = -1 * std::log(p);
				p = ((p - min) / (max - min)) * 255;

				if (p >= 255){
					p = 255;
				}

				dst.data[y*dst.step + x*dst.elemSize()] = (uchar)p;
			}
		}
	}


	std::cout << max << std::endl;
	std::cout << min << std::endl;

}

void likeliHood(cv::Mat src, std::vector<Cluster> cluster, std::vector<double> &like)
{
	int i, j, x, y;
	cv::Vec3d color;
	double p = 0;
	double max = 0;
	double min = 99999;
	for (y = 0; y < src.rows; y++){
		for (x = 0; x < src.cols; x++){
			
			color[0] = (double)src.data[y * src.step + x * src.elemSize()];
			color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
			color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

			p = 0;
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isAveCalc){
					p += cluster[i].distribution(color);
				}
			}
			p /= (double)cluster.size();
			p = -1 * std::log(p);
			if (max < p)	max = p;
			if (min > p)	min = p;
			
			like[y*src.cols + x] = p;
		}
	}

	for (i = 0; i < (int)like.size(); i++){
		like[i] = ((like[i] - min) / (max - min)) * (10-1) + 1;
	}
	std::cout << max << std::endl;
	std::cout << min << std::endl;
}
