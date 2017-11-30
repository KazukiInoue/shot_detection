#include "function.h"

void judgeByBgrThreshold(bool &isPossibleBoundary, cv::Mat nowFrame, cv::Mat preFrame) {

	const int diffThreshold = 100; // ����������臒l�A���m�N���ɂ͑Ή��ł��Ȃ�

	cv::Mat nowChannels[3];
	cv::Mat preChannels[3];

	cv::split(nowFrame, nowChannels);
	cv::split(preFrame, preChannels);

	cv::Mat colorDiffMat[3];
	double colorDiff = 0;

	for (int c = 0; c < 3; c++) {
		cv::absdiff(nowChannels[c], preChannels[c], colorDiffMat[c]);

		//��f�l�̑��a/��f�� = ��f�l�̕��ϒl
		colorDiff += cv::mean(colorDiffMat[c])[0];
	}

	cout << colorDiff << endl;

	// �V���b�g���o���ꂽ���̏���
	if (colorDiff > diffThreshold) {
		isPossibleBoundary = true;
	}
	else {
		isPossibleBoundary = false;
	}
}