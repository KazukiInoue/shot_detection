#include "function.h"

#include <numeric>

using namespace std;

void testCheckColorThemesDistanceBetweenImages(vector<vector<vector<double>>> selectedAllImagesThemes) {

	for (int imgItr = 0; imgItr < selectedAllImagesThemes.size(); imgItr++) {
		for (int subImgItr = 0; subImgItr < selectedAllImagesThemes.size(); subImgItr++) {

			for (int themeItr = 0; themeItr < selectedAllImagesThemes.front().size(); themeItr++) {

				vector<double> colorDiffVec;

				for (int subThemeItr = 0; subThemeItr < selectedAllImagesThemes.front().size(); subThemeItr++) {

					vector<double> nowColor = selectedAllImagesThemes[imgItr][themeItr];
					vector<double> compareColor = selectedAllImagesThemes[subImgItr][subThemeItr];

					double tmpColorDiff = 0;

					for (int c = 0; c < 3; c++) {
						tmpColorDiff += pow(nowColor[c] - compareColor[c], 2);
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

				cout << "画像" << imgItr << "の右から" << to_string(themeItr + 1) << "番目の色に最も近い色は"
					<< "画像" << subImgItr << "の中では" << index[0] + 1 << "番目の色です 色距離は" << colorDiffVec[index[0]] << "です" << endl;
				
			}
			
			cout << endl;
		}
	}

	system("pause");
}

void testShowExtractedColorThemes(vector<vector<vector<double>>> &selectedAllImagesThemes, vector<string> imgPath, const int clusterNum, const int numToExtract) {

	for (int imgItr = 0; imgItr < imgPath.size(); imgItr++) {

		cv::Mat src = cv::imread(imgPath[imgItr]);
		if (src.empty()) {
			cerr << "src doesn't exist!!" << endl;
		}

		cv::resize(src, src, cv::Size(), 256 / double(src.cols), 256 / double(src.rows));

		vector<vector<double>> colorThemes(numToExtract, vector<double>(3, 0));
		cv::Mat subtImg(src.size(), src.type());
		vector<vector<double>> clusterInfo; /*select_principal_color_themesで使用する*/

		// 色の選択
		kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, src, clusterNum);
		selectPrincipalColorThemes(/*&*/colorThemes, subtImg, clusterNum, numToExtract, clusterInfo);


		// 全画像のcolorThemeを一つのvectorに格納
		selectedAllImagesThemes[imgItr] = colorThemes;


		// 選択された色の表示
		cv::Mat combinedColor;

		for (int themeItr = 0; themeItr < numToExtract; themeItr++) {

			cv::Mat selectedColorImg(cv::Size(50, 50), CV_8UC3, cv::Scalar(0, 0, 0));
			cv::Mat channels[3];
			cv::split(selectedColorImg, channels);


			for (int c = 0; c < 3; c++) {
				channels[c] = cv::saturate_cast<uchar>(colorThemes[themeItr][c]);
			}

			cv::merge(channels, 3, selectedColorImg);

			if (themeItr == 0) {
				combinedColor = selectedColorImg.clone();
			}
			else {
				cv::hconcat(selectedColorImg, combinedColor, combinedColor); /*右にある色ほど重要度が高い色*/
			}
		}

		cv::namedWindow("selected Color in image:" + to_string(imgItr), CV_WINDOW_FREERATIO);
		cv::imshow("selected Color in image:" + to_string(imgItr), combinedColor);
		cv::imshow("image:" + to_string(imgItr), src);
	}

	cv::waitKey(0);
}

void testExtractedColorInputPart() {

	const int clusterNum = 20; /*5以上を指定してください*/
	const int numToExtract = 20; /*clusterNum以下の値を指定してください*/

	vector<string> imgPath;
	imgPath.push_back("../test_images/frame1.jpg");
	imgPath.push_back("../test_images/frame2.jpg");

	// 全ての画像で得られるcolorTheme　変数名が複雑になってきたので修正する
	vector<vector<vector<double>>> selectedAllImagesThemes(imgPath.size(), vector<vector<double>>(numToExtract, vector<double>(3, 0)));

	testShowExtractedColorThemes(/*&*/selectedAllImagesThemes, imgPath, clusterNum, numToExtract);

	testCheckColorThemesDistanceBetweenImages(selectedAllImagesThemes);
}