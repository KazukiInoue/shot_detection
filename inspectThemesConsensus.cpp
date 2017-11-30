#include "function.h"

#include <numeric>

void inspectThemesConsensus(bool &isBoundary, vector<vector<double>> nowColorThemes, vector<vector<double>> preColorThemes) {

	const int colorThemesNum = nowColorThemes.size();

	int consensusCount = 0;
	const int colorThereshold = 200; /*�����F���ƌ��Ȃ��邢��������臒l*/
	const int countThreshold = 10;   /*����color themes���ƌ��Ȃ��邢��������臒l*/

	// color themes�̈�v�x�𒲂ׂ�
	for (int nowItr = 0; nowItr < colorThemesNum; nowItr++) {

		vector<double> colorDiffVec;

		for (int preItr = 0; preItr < colorThemesNum; preItr++) {

			double tmpColorDiff = 0;

			for (int c = 0; c < 3; c++) {
				tmpColorDiff += pow(nowColorThemes[nowItr][c] - preColorThemes[preItr][c], 2);
			}

			colorDiffVec.push_back(tmpColorDiff);
		}

		// �C���f�b�N�X��ۂ����܂܏����\�[�g
		std::vector<int> index(colorDiffVec.size());
		std::iota(index.begin(), index.end(), 0);

		std::sort(
			index.begin(),
			index.end(),
			[&](int x, int y) {return colorDiffVec[x] < colorDiffVec[y]; }
		);

		if (colorDiffVec[index[0]] < colorThereshold) {
			consensusCount += 1;
		}
	}

	if (consensusCount < 10) {
		isBoundary = true;
	}
	else {
		isBoundary = false;
	}

}