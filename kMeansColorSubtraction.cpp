#include "function.h"

using namespace std;

void kMeansColorSubtraction(cv::Mat &dst, std::vector<std::vector<double>> &clusterInfo, cv::Mat src, const int clusterNum) {

	cv::Mat points;

	src.convertTo(points, CV_32FC3);

	points = points.reshape(3, src.rows * src.cols);

	// BGR��Ԃ�k-means�N���X�^�����O
	cv::Mat_<int> clusters(points.size(), CV_32SC1); // �e�s�N�Z���̃��x�����i�[ (���͉摜�̃s�N�Z����)
	cv::Mat centers; // �e�N���X�^�̏d�S���i�[ (�N���X�^��, �����ʎ���=3)

	cv::kmeans(points, clusterNum, clusters,
		cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, /*iteration=*/10,/*epsilon=*/ 1.0),
		1, cv::KMEANS_PP_CENTERS, centers);


	cv::MatIterator_<cv::Vec3b> itd = dst.begin<cv::Vec3b>(), itd_end = dst.end<cv::Vec3b>();

	vector<double> pixelNum(clusterNum, 0);
	vector<vector<double>> posiCenter(clusterNum, vector<double>(2, 0));

	for (int itr = 0; itd != itd_end; ++itd, ++itr) {

		int label = clusters(itr); // label=0,1..,18,19

		// ��Ŏg���N���X�^�̈ʒu���������Ŏ��W
		pixelNum[label] += 1.0;

		double x = itr % src.cols;
		double y = (itr - x) / src.cols;
		posiCenter[label][0] += x;
		posiCenter[label][1] += y;

		// ��f�l���N���X�^���S�l�̐F�ɒu��
		cv::Vec3f &bgr = centers.ptr<cv::Vec3f>(label)[0];

		for (int c = 0; c < 3; c++) {
			(*itd)[c] = cv::saturate_cast<uchar>(bgr[c]);
		}
	}

	for (int clusterItr = 0; clusterItr < posiCenter.size(); clusterItr++) {

		double x_center = posiCenter[clusterItr][0] / pixelNum[clusterItr];
		double y_center = posiCenter[clusterItr][1] / pixelNum[clusterItr];
		double pixelRatio = pixelNum[clusterItr] / (src.cols * src.rows);

		double blue_center = (double)centers.ptr<float>(clusterItr)[0];
		double green_center = (double)centers.ptr<float>(clusterItr)[1];
		double red_center = (double)centers.ptr<float>(clusterItr)[2];

		vector<double> tmpClusterInfo;
		tmpClusterInfo.push_back(x_center);
		tmpClusterInfo.push_back(y_center);
		tmpClusterInfo.push_back(pixelRatio);
		tmpClusterInfo.push_back(blue_center);
		tmpClusterInfo.push_back(green_center);
		tmpClusterInfo.push_back(red_center);
		clusterInfo.push_back(tmpClusterInfo);
	}
}