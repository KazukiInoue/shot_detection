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
			

				// �C���f�b�N�X��ۂ����܂܏����\�[�g
				std::vector<int> index(colorDiffVec.size());
				std::iota(index.begin(), index.end(), 0);

				std::sort(
					index.begin(),
					index.end(),
					[&](int x, int y) {return colorDiffVec[x] < colorDiffVec[y]; }
				);

				cout << "�摜" << imgItr << "�̉E����" << to_string(themeItr + 1) << "�Ԗڂ̐F�ɍł��߂��F��"
					<< "�摜" << subImgItr << "�̒��ł�" << index[0] + 1 << "�Ԗڂ̐F�ł� �F������" << colorDiffVec[index[0]] << "�ł�" << endl;
				
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
		vector<vector<double>> clusterInfo; /*select_principal_color_themes�Ŏg�p����*/

		// �F�̑I��
		kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, src, clusterNum);
		selectPrincipalColorThemes(/*&*/colorThemes, subtImg, clusterNum, numToExtract, clusterInfo);


		// �S�摜��colorTheme�����vector�Ɋi�[
		selectedAllImagesThemes[imgItr] = colorThemes;


		// �I�����ꂽ�F�̕\��
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
				cv::hconcat(selectedColorImg, combinedColor, combinedColor); /*�E�ɂ���F�قǏd�v�x�������F*/
			}
		}

		cv::namedWindow("selected Color in image:" + to_string(imgItr), CV_WINDOW_FREERATIO);
		cv::imshow("selected Color in image:" + to_string(imgItr), combinedColor);
		cv::imshow("image:" + to_string(imgItr), src);
	}

	cv::waitKey(0);
}

void testExtractedColorInputPart() {

	const int clusterNum = 20; /*5�ȏ���w�肵�Ă�������*/
	const int numToExtract = 20; /*clusterNum�ȉ��̒l���w�肵�Ă�������*/

	vector<string> imgPath;
	imgPath.push_back("../test_images/frame1.jpg");
	imgPath.push_back("../test_images/frame2.jpg");

	// �S�Ẳ摜�œ�����colorTheme�@�ϐ��������G�ɂȂ��Ă����̂ŏC������
	vector<vector<vector<double>>> selectedAllImagesThemes(imgPath.size(), vector<vector<double>>(numToExtract, vector<double>(3, 0)));

	testShowExtractedColorThemes(/*&*/selectedAllImagesThemes, imgPath, clusterNum, numToExtract);

	testCheckColorThemesDistanceBetweenImages(selectedAllImagesThemes);
}