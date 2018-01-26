#include "filter.h"

namespace filter
{
	void bilateral(cv::Mat src, cv::Mat dst, std::vector<double> &fg, std::vector<double> &bg)
	{
		const int size = 15;
		double sigmaS = 9 / 3, sigmaC = 120 / 3;

		double weight_sum = 0;
		double data = 0;

		double kernel[size*size];

		double dist = 0;
		double color = 0;

		int i, j, x, y;
		int dx, dy;

		double k = 0;

		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				dx = x - (size / 2);
				dy = y - (size / 2);

				weight_sum = 0;
				for (j = 0; j < size; j++){
					for (i = 0; i < size; i++){
						if (dy + j < 0 || dy + j >= src.rows || dx + i < 0 || dx + i >= src.cols){
							kernel[j*size + i] = 0;
						}
						else{
							dist = sqrt((x - (dx + i))*(x - (dx + i)) + (y - (dy + j))*(y - (dy + j)));
							color = sqrt(pow((double)src.data[y * src.step + x * src.elemSize()] - (double)src.data[(dy + j) * src.step + (dx + i) * src.elemSize()], 2));

							if (dy + j == y && dx + i == x){
								k = 1;
							}
							else{
								//k = sqrt(pow(fg[y*src.cols + x] - fg[(dy + j)*src.cols + (dx + i)], 2));
								k = fg[(dy + j)*src.cols + (dx + i)];
							}

							kernel[j*size + i] = gauss(dist, sigmaS) * gauss(color, sigmaC);
						}
						weight_sum += kernel[j*size + i];
					}
				}
				data = 0;
				for (j = 0; j < size; j++){
					for (i = 0; i < size; i++){
						if (dy + j < 0 || dy + j >= src.rows || dx + i < 0 || dx + i >= src.cols) continue;
						data += (double)src.data[(dy + j)*src.step + (dx + i) * src.elemSize()] * kernel[j*size + i];
					}
				}

				data /= weight_sum;
				if (data < 0)  data = 0;
				if (data > 255)  data = 255;

				dst.data[y*src.step + x * src.elemSize()] = (uchar)data;

			}
		}


	}

	void bilateral(cv::Mat src, cv::Mat dst, std::vector<double> &fg)
	{
		const int size = 15;
		double sigmaS = 20 / 3, sigmaC = 150 / 3;

		double weight_sum = 0;
		double data = 0;

		double kernel[size*size];

		double dist = 0;
		double color = 0;

		int i, j, x, y;
		int dx, dy;

		double k = 0;

		for (y = 0; y < src.rows; y++){
			for (x = 0; x < src.cols; x++){
				dx = x - (size / 2);
				dy = y - (size / 2);

				weight_sum = 0;
				for (j = 0; j < size; j++){
					for (i = 0; i < size; i++){
						if (dy + j < 0 || dy + j >= src.rows || dx + i < 0 || dx + i >= src.cols){
							kernel[j*size + i] = 0;
						}
						else{
							dist = sqrt((x - (dx + i))*(x - (dx + i)) + (y - (dy + j))*(y - (dy + j)));
							color = sqrt(pow((double)src.data[y * src.step + x * src.elemSize()] - (double)src.data[(dy + j) * src.step + (dx + i) * src.elemSize()], 2));
							k = 50 / ( fg[y*src.cols + x] - (fg[y*src.cols + x] - fg[(dy + j)*src.cols + (dx + i)]));
							
								//k = fg[(dy + j)*src.cols + (dx + i)];
							

							kernel[j*size + i] = gauss(dist, sigmaS) * gauss(color, k);
						}
						weight_sum += kernel[j*size + i];
					}
				}
				data = 0;
				for (j = 0; j < size; j++){
					for (i = 0; i < size; i++){
						if (dy + j < 0 || dy + j >= src.rows || dx + i < 0 || dx + i >= src.cols) continue;
						data += (double)src.data[(dy + j)*src.step + (dx + i) * src.elemSize()] * kernel[j*size + i];
					}
				}

				data /= weight_sum;
				if (data < 0)  data = 0;
				if (data > 255)  data = 255;

				dst.data[y*src.step + x * src.elemSize()] = (uchar)data;

			}
		}


	}

	double gauss(double x, double sigma)
	{
		return (1 / sqrt(2 * M_PI*sigma*sigma)) * exp(-(x*x) / (2 * sigma*sigma));
	}

}