#include "function.h"

void judgeByBgrThreshold(bool &isPossibleBoundary, cv::Mat nowFrame, cv::Mat preFrame) {

	const int diffThreshold = 100; // いい感じの閾値、モノクロには対応できない

	cv::Mat nowChannels[3];
	cv::Mat preChannels[3];

	cv::split(nowFrame, nowChannels);
	cv::split(preFrame, preChannels);

	cv::Mat colorDiffMat[3];
	double colorDiff = 0;

	for (int c = 0; c < 3; c++) {
		cv::absdiff(nowChannels[c], preChannels[c], colorDiffMat[c]);

		//画素値の総和/画素数 = 画素値の平均値
		colorDiff += cv::mean(colorDiffMat[c])[0];
	}

	cout << colorDiff << endl;

	// ショット検出された時の処理
	if (colorDiff > diffThreshold) {
		isPossibleBoundary = true;
	}
	else {
		isPossibleBoundary = false;
	}
}