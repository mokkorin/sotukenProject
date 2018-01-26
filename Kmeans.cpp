#include "Kmeans.h"

double Cluster::distance(cv::Vec3d color, cv::Point p, bool flag)
{
	double dist = 0;
	int i;

	for (i = 0; i < 3; i++){
		dist += (ave[i] - color[i]) * (ave[i] - color[i]);
	}
	if (flag){
		dist += (double)(p_ave.x - p.x) * (double)(p_ave.x - p.x) + (double)(p_ave.y - p.y) * (double)(p_ave.y - p.y);
	}
	return sqrt(dist);

}

bool Cluster::isSmall(void)
{
	if (point.size() < (size_t)50){
		return true;
	}
	else{
		return false;
	}
}

namespace kmeans
{
	void run(cv::Mat src, cv::Rect roi, std::vector<Cluster> &cluster)
	{
		std::mt19937 mt(99);
		int x, y;
		int i, j;
		char t;
		double p;
		double diff = 0;
		bool flag = false;
		std::vector<Cluster>::iterator rmv_itr;
		cv::Mat img;
		src.copyTo(img);

		cv::Vec3d color(0, 0, 0);
		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				if (y > roi.tl().y && x > roi.tl().x && y < roi.br().y && x < roi.br().x)	continue;
				color[0] = (double)src.data[y * src.step + x * src.elemSize()];
				color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
				color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];
				cluster[mt() % (int)cluster.size()].dataSet(color, cv::Point(x, y));
			}
		}

		cv::namedWindow("k-means", cv::WINDOW_AUTOSIZE);
		cv::imshow("k-means", img);
		cv::waitKey(0);
		for (t = 0; t != 27; t = cv::waitKey(12)){

			/* 分散と平均計算 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isData){
					cluster[i].calcAverage();
					//cluster[i].calcSigma();
					/*std::cout << i << std::endl;
					cluster[i].output();
					std::cout << std::endl;*/
					//cluster[i].dataClear();
				}
			}
			std::cout << "before : " << cluster.size() << std::endl;

			/* クラスタ統合処理 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (!cluster[i].isAveCalc)	continue;
				/* 小さいクラスタであれば消去 */
				if (cluster[i].isSmall()){
					cluster[i].dataClear();
					cluster[i].isAveCalc = false;
					continue;
				}
				for (j = 0; j < (int)cluster.size(); j++){
					if (i == j || !(cluster[j].isAveCalc))	continue;
					color[0] = cluster[j].ave[0];
					color[1] = cluster[j].ave[1];
					color[2] = cluster[j].ave[2];
					/* クラスタの色空間距離が規定値以下であったら近いクラスタを消去 */
					if (cluster[i].distance(color, cluster[j].p_ave, true) < 5){
						cluster[j].dataClear();
						cluster[j].isAveCalc = false;
					}
				}
			}

			/* データを持たないクラスタの削除処理 */
			rmv_itr = std::remove_if(cluster.begin(), cluster.end(), [&](Cluster cls){return !(cls.isData); });
			for (; rmv_itr != cluster.end(); cluster.pop_back());
			std::cout << "after : " << cluster.size() << std::endl;

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].dataClear();
			}

			/* どのクラスタに含まれるのかチェック */
			for (y = 0; y < src.rows; y++){
				for (x = 0; x < src.cols; x++){
					if (y > roi.tl().y && x > roi.tl().x && y < roi.br().y && x < roi.br().x){
						img.data[y * img.step + x * img.elemSize()] = 0;
						img.data[y * img.step + x * img.elemSize() + 1] = 0;
						img.data[y * img.step + x * img.elemSize() + 2] = 0;
						continue;
					}
					color[0] = (double)src.data[y * src.step + x * src.elemSize()];
					color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
					color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

					flag = false;
					j = 0;
					for (i = 0; i < (int)cluster.size(); i++){
						if (cluster[i].isAveCalc){
							if (flag){
								if (p > cluster[i].distance(color, cv::Point(x, y), true)){
									p = cluster[i].distance(color, cv::Point(x, y), true);
									j = i;
								}
							}
							else{
								p = cluster[i].distance(color, cv::Point(x, y), true);
								j = i;
								flag = true;
							}
						}
					}
					cluster[j].dataSet(color, cv::Point(x, y));
					img.data[y * img.step + x * img.elemSize()] = (uchar)cluster[j].ave[0];
					img.data[y * img.step + x * img.elemSize() + 1] = (uchar)cluster[j].ave[1];
					img.data[y * img.step + x * img.elemSize() + 2] = (uchar)cluster[j].ave[2];
				}
			}

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].isAveCalc = false;
			}
			cv::imshow("k-means", img);

		}

		for (i = 0; i < (int)cluster.size(); i++){
			if (cluster[i].isData){
				cluster[i].calcAverage();
				cluster[i].calcSigma();
				cluster[i].dataClear();
				/*std::cout << i << std::endl;
				cluster[i].output();
				std::cout << std::endl;
				*/
			}
		}

	}

	void run(cv::Mat src, cv::Mat mask, std::vector<Cluster> &cluster)
	{
		std::mt19937 mt(99);
		int x, y;
		int i, j;
		char t;
		double p;
		double diff = 0;
		bool flag = false;
		std::vector<Cluster>::iterator rmv_itr;
		cv::Mat img;
		src.copyTo(img);

		cv::Vec3d color(0, 0, 0);
		
		for (i = 0; i < (int)cluster.size(); i++){
			cluster[i].dataClear();
		}

		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				if (mask.data[y*mask.step + x*mask.elemSize()] != 255)continue;
				color[0] = (double)src.data[y * src.step + x * src.elemSize()];
				color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
				color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];
				cluster[mt() % (int)cluster.size()].dataSet(color, cv::Point(x, y));
			}
		}

		cv::namedWindow("k-means-mask", cv::WINDOW_AUTOSIZE);
		cv::imshow("k-means-mask", img);
		cv::waitKey(0);
		for (t = 0; t != 27; t = cv::waitKey(12)){

			/* 分散と平均計算 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isData){
					cluster[i].calcAverage();
					//cluster[i].calcSigma();
					/*std::cout << i << std::endl;
					cluster[i].output();
					std::cout << std::endl;*/
					//cluster[i].dataClear();
				}
			}
			std::cout << "before : " << cluster.size() << std::endl;

			/* クラスタ統合処理 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (!cluster[i].isAveCalc)	continue;
				/* 小さいクラスタであれば消去 */
				/*if (cluster[i].isSmall()){
					cluster[i].dataClear();
					cluster[i].isAveCalc = false;
					continue;
				}*/
				for (j = 0; j < (int)cluster.size(); j++){
					if (i == j || !(cluster[j].isAveCalc))	continue;
					color[0] = cluster[j].ave[0];
					color[1] = cluster[j].ave[1];
					color[2] = cluster[j].ave[2];
					/* クラスタの色空間距離が規定値以下であったら近いクラスタを消去 */
					if (cluster[i].distance(color, cluster[j].p_ave, false) < 20){
						cluster[j].dataClear();
						cluster[j].isAveCalc = false;
					}
				}
			}

			/* データを持たないクラスタの削除処理 */
			rmv_itr = std::remove_if(cluster.begin(), cluster.end(), [&](Cluster cls){return !(cls.isData); });
			for (; rmv_itr != cluster.end(); cluster.pop_back());
			std::cout << "after : " << cluster.size() << std::endl;

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].dataClear();
			}

			/* どのクラスタに含まれるのかチェック */
			for (y = 0; y < src.rows; y++){
				for (x = 0; x < src.cols; x++){
					if (mask.data[y*mask.step + x*mask.elemSize()] != 255){
						img.data[y * img.step + x * img.elemSize()] = 0;
						img.data[y * img.step + x * img.elemSize() + 1] = 0;
						img.data[y * img.step + x * img.elemSize() + 2] = 0;
						continue;
					}
					color[0] = (double)src.data[y * src.step + x * src.elemSize()];
					color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
					color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

					flag = false;
					j = 0;
					for (i = 0; i < (int)cluster.size(); i++){
						if (cluster[i].isAveCalc){
							if (flag){
								if (p > cluster[i].distance(color, cv::Point(x, y), false)){
									p = cluster[i].distance(color, cv::Point(x, y), false);
									j = i;
								}
							}
							else{
								p = cluster[i].distance(color, cv::Point(x, y), false);
								j = i;
								flag = true;
							}
						}
					}
					cluster[j].dataSet(color, cv::Point(x, y));
					img.data[y * img.step + x * img.elemSize()] = (uchar)cluster[j].ave[0];
					img.data[y * img.step + x * img.elemSize() + 1] = (uchar)cluster[j].ave[1];
					img.data[y * img.step + x * img.elemSize() + 2] = (uchar)cluster[j].ave[2];
				}
			}

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].isAveCalc = false;
			}
			cv::imshow("k-means-mask", img);

		}

		for (i = 0; i < (int)cluster.size(); i++){
			if (cluster[i].isData){
				cluster[i].calcAverage();
				cluster[i].calcSigma();
				cluster[i].dataClear();
				/*std::cout << i << std::endl;
				cluster[i].output();
				std::cout << std::endl;
				*/
			}
		}
	}

	void run(cv::Mat src, std::vector<Cluster> &cluster)
	{
		std::mt19937 mt(99);
		int x, y;
		int i, j;
		char t;
		double p;
		bool flag = false;
		std::vector<Cluster>::iterator rmv_itr;
		cv::Mat img;
		src.copyTo(img);

		cv::Vec3d color(0, 0, 0);
		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				color[0] = (double)src.data[y * src.step + x * src.elemSize()];
				color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
				color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];
				cluster[mt() % (int)cluster.size()].dataSet(color, cv::Point(x, y));
			}
		}

		cv::namedWindow("k-means"+std::to_string(src.cols), cv::WINDOW_AUTOSIZE);
		cv::imshow("k-means" + std::to_string(src.cols), img);
		cv::waitKey(0);
		for (t = 0; t != 27; t = cv::waitKey(12)){

			/* 分散と平均計算 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (cluster[i].isData){
					cluster[i].calcAverage();
					cluster[i].calcSigma();
					/*std::cout << i << std::endl;
					cluster[i].output();
					std::cout << std::endl;*/
					//cluster[i].dataClear();
				}
			}
			std::cout << "before : " << cluster.size() << std::endl;

			/* クラスタ統合処理 */
			for (i = 0; i < (int)cluster.size(); i++){
				if (!cluster[i].isAveCalc)	continue;
				/* 小さいクラスタであれば消去 */
				if (cluster[i].isSmall()){
					cluster[i].dataClear();
					cluster[i].isAveCalc = false;
					continue;
				}
				for (j = 0; j < (int)cluster.size(); j++){
					if (i == j || !(cluster[j].isAveCalc))	continue;
					color[0] = cluster[j].ave[0];
					color[1] = cluster[j].ave[1];
					color[2] = cluster[j].ave[2];
					/* クラスタの色空間距離が規定値以下であったら近いクラスタを消去 */
					if (cluster[i].distance(color, cluster[j].p_ave, true) < 10){
						cluster[j].dataClear();
						cluster[j].isAveCalc = false;
					}
				}
			}

			/* データを持たないクラスタの削除処理 */
			rmv_itr = std::remove_if(cluster.begin(), cluster.end(), [&](Cluster cls){return !(cls.isData); });
			for (; rmv_itr != cluster.end(); cluster.pop_back());
			std::cout << "after : " << cluster.size() << std::endl;

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].dataClear();
			}

			/* どのクラスタに含まれるのかチェック */
			for (y = 0; y < src.rows; y++){
				for (x = 0; x < src.cols; x++){
					color[0] = (double)src.data[y * src.step + x * src.elemSize()];
					color[1] = (double)src.data[y * src.step + x * src.elemSize() + 1];
					color[2] = (double)src.data[y * src.step + x * src.elemSize() + 2];

					flag = false;
					j = 0;
					for (i = 0; i < (int)cluster.size(); i++){
						if (cluster[i].isAveCalc){
							if (flag){
								if (p > cluster[i].distance(color, cv::Point(x, y), true)){
									p = cluster[i].distance(color, cv::Point(x, y), true);
									j = i;
								}
							}
							else{
								p = cluster[i].distance(color, cv::Point(x, y), true);
								j = i;
								flag = true;
							}
						}
					}
					cluster[j].dataSet(color, cv::Point(x, y));
					img.data[y * img.step + x * img.elemSize()] = (uchar)cluster[j].ave[0];
					img.data[y * img.step + x * img.elemSize() + 1] = (uchar)cluster[j].ave[1];
					img.data[y * img.step + x * img.elemSize() + 2] = (uchar)cluster[j].ave[2];
				}
			}

			for (i = 0; i < (int)cluster.size(); i++){
				cluster[i].isAveCalc = false;
			}
			cv::imshow("k-means" + std::to_string(src.cols), img);

		}

		for (i = 0; i < (int)cluster.size(); i++){
			if (cluster[i].isData){
				cluster[i].calcAverage();
				cluster[i].calcSigma();
				cluster[i].dataClear();
				/*std::cout << i << std::endl;
				cluster[i].output();
				std::cout << std::endl;*/
				
			}
		}

		img.copyTo(src);
	}
}