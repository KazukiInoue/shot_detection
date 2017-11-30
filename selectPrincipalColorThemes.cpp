#include <numeric>

#include "function.h"

using namespace std;

void selectPrincipalColorThemes(vector<vector<double>> &colorThemes, cv::Mat src, const int clusterNum,const int numToExtract ,vector<vector<double>> clusterInfo) {

	/* clusterInfo =[x_center, y_center, pixelRatio, blue_center, green_center, red_center] */
	/* Please referto k_means_color_subtraction.cpp */

	const int omega[2] = { 1, 30 };
	const int outputThemesNum = 5;

	const double imgCenter[2] = { (double)src.cols / 2, (double)src.rows / 2 };

	vector<vector<double>> lScore(clusterNum, vector<double>(2, 0));
	vector<double> aScore(clusterNum, 0);
	vector<vector<double>> bgrVal(clusterNum, vector<double>(3, 0));

	for (int itr = 0; itr < clusterNum; itr++) {

		lScore[itr][0] = clusterInfo[itr][0];
		lScore[itr][1] = clusterInfo[itr][1];

		aScore[itr] = clusterInfo[itr][2];

		bgrVal[itr][0] = clusterInfo[itr][3];
		bgrVal[itr][1] = clusterInfo[itr][4];
		bgrVal[itr][2] = clusterInfo[itr][5];
	}

	vector<double> pScore(clusterNum, 0);
	vector<double> sScore(clusterNum, 0);

	for (int clusterItr = 0; clusterItr < clusterNum; clusterItr++) {

		// pScore
		double subtDistance = 0;
		double orgDistance = 0;
		for (int posiDim = 0; posiDim < 2; posiDim++) {
			subtDistance += pow(lScore[clusterItr][posiDim] - imgCenter[posiDim], 2);
			orgDistance += pow(imgCenter[posiDim], 2);
		}

		subtDistance = sqrt(subtDistance);
		orgDistance = sqrt(orgDistance);

		pScore[clusterItr] = 1 - subtDistance / orgDistance;

		// sScore
		for (int subClusterItr = 0; subClusterItr < clusterNum; subClusterItr++) {

			double colorDistance = 0;
			for (int c = 0; c < 3; c++) {
				colorDistance += pow(bgrVal[clusterItr][c] - bgrVal[subClusterItr][c], 2);
			}

			sScore[clusterItr] += aScore[subClusterItr] * colorDistance;
		}

		sScore[clusterItr] /= clusterNum;
	}

	std::vector<double> finalScore(clusterNum, 0);

	for (int clusterItr = 0; clusterItr < clusterNum; clusterItr++) {
		finalScore[clusterItr] = omega[0] * aScore[clusterItr] + omega[1] * pScore[clusterItr] * sScore[clusterItr];
	}

	// インデックスを保ったまま昇順ソート
	std::vector<int> index(finalScore.size());
	std::iota(index.begin(), index.end(), 0);

	std::sort(
		index.begin(),
		index.end(),
		[&](int x, int y) {return finalScore[x] < finalScore[y]; }
	);

	//std::cout << "after sort" << std::endl;
	//for (auto& i : index) {
	//	std::cout << i << " " << finalScore[i] << std::endl;
	//}

	for (int itr = 0; itr < numToExtract; itr++) {

		int tmpIndex = index[clusterNum - 1 - itr];

		colorThemes[itr] = bgrVal[tmpIndex];
	}
}