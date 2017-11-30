#include "function.h"

#include <numeric>

void inspectThemesConsensus(bool &isBoundary, vector<vector<double>> nowColorThemes, vector<vector<double>> preColorThemes) {

	const int colorThemesNum = nowColorThemes.size();

	int consensusCount = 0;
	const int colorThereshold = 200; /*同じ色だと見なせるいい感じの閾値*/
	const int countThreshold = 10;   /*同じcolor themesだと見なせるいい感じの閾値*/

	// color themesの一致度を調べる
	for (int nowItr = 0; nowItr < colorThemesNum; nowItr++) {

		vector<double> colorDiffVec;

		for (int preItr = 0; preItr < colorThemesNum; preItr++) {

			double tmpColorDiff = 0;

			for (int c = 0; c < 3; c++) {
				tmpColorDiff += pow(nowColorThemes[nowItr][c] - preColorThemes[preItr][c], 2);
			}

			colorDiffVec.push_back(tmpColorDiff);
		}

		// インデックスを保ったまま昇順ソート
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