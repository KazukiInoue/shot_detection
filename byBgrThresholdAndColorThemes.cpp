#include "accessDirectory.h"
#include "function.h"

using namespace std;

void byBgrThresholdAndColorThemes() {

	const int width = 256;
	const int height = 256;

	const int clusterNum = 20; /*5�ȏ���w�肵�Ă�������*/
	const int numToExtract = 20; /*clusterNum�ȉ��̒l���w�肵�Ă�������*/

	bool isTrain = true;
	int indexValue = 1;

	string videoDir[2] = {
		"../../src_data/IMV133/",
		"../../src_data/OMV200/"};

	string toDir[2] = {
		"../../src_data/shots_IMV133_threshold=130_themes=6/", 
		"../../src_data/shots_OMV200_threshold=130_themes=6/"};


	for (int entireItr = 0; entireItr < 1; entireItr++) {
		//----------����t�@�C�����J��----------


		std::vector<std::string> videoList = Dir::read(videoDir[entireItr]);

		for (int vidItr = 0; vidItr < videoList.size(); vidItr++) {

			stringstream tmpFileName(videoList[vidItr]);
			string tmpString;
			vector<string> splitedFileName;

			while (getline(tmpFileName, tmpString, '.')) {
				splitedFileName.push_back(tmpString);
			}

			if (splitedFileName.size() > 1) { //..�Ƃ����t�@�C�������͂���邱�Ƃ�h��
				if (splitedFileName[1] == "mp4") {
					string videoFile = videoDir[entireItr] + videoList[vidItr];

					cout << videoFile << endl;
					cv::VideoCapture cap(videoFile);

					//�t�@�C�����I�[�v���ł������̊m�F
					if (!cap.isOpened()) {
						std::cerr << "cap can't be opened!" << endl;
						exit(1);
					}

					//----------�V���b�g���o�J�n----------

					cv::Mat preFrame;
					cv::Mat nowFrame;

					vector<vector<double>> preColorThemes(numToExtract, vector<double>(3, 0));
					vector<vector<double>> nowColorThemes(numToExtract, vector<double>(3, 0));

					string fileName= splitedFileName[0]; // video00001

					/* 1�t���[���ڂ͕ۑ�
					   2�t���[���ڈȍ~�ŃV���b�g���o���ꂽ�t���[����ۑ�
					   �ۑ����閼�O��(������̖��O)_(�t���[���ԍ�)_(�t���[���̎���).png*/

					for (int frameItr = 0;; frameItr++) { //�Ō�̐��t���[���̓V���b�g���o���Ȃ�
						//1�t���[����
						if (frameItr == 0) {

							cap >> nowFrame;
							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));

							// cv::imshow("video", nowFrame);

							// extract color themes
							cv::Mat subtImg(nowFrame.size(), nowFrame.type());
							vector<vector<double>> clusterInfo; /*select_principal_color_themes�Ŏg�p����*/

							kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
							selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

							// �ۑ�
							string frame2Name;
							intValue2Name(frame2Name, frameItr + indexValue);
							double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
							string outputName = toDir[entireItr] + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
							cv::imwrite(outputName, nowFrame);
						}
						//2�t���[���ڈȍ~
						else {

							preFrame = nowFrame.clone();
							preColorThemes = nowColorThemes;

							cap >> nowFrame;

							if (nowFrame.empty()) {
								break;
							}

							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));

							//cv::imshow("video", nowFrame);

							// 1.�O�t���[���Ƃ�BGR�̒l�̍����v�Z���A臒l�ȏ�Ȃ玟�̏�����
							bool isPossibleBoundary = false;
							judgeByBgrThreshold(/*&*/isPossibleBoundary, nowFrame, preFrame);

							if (isPossibleBoundary) {

								// 2.�O�t���[���Ƃ�color theme(�悭�g���Ă���J���[�p���b�g�݂����Ȃ��́j�̕ω���p���ăV���b�g���o
								cv::Mat subtImg(nowFrame.size(), nowFrame.type());
								vector<vector<double>> clusterInfo; /*select_principal_color_themes�Ŏg�p����*/

								kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
								selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

								bool isBoundary = false;
								inspectThemesConsensus(/*&*/isBoundary, nowColorThemes, preColorThemes);

								//�V���b�g���o���ꂽ��ۑ�
								if (isBoundary) {
									string frame2Name;
									intValue2Name(frame2Name, frameItr + indexValue);
									double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
									string outputName = toDir[entireItr] + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
									cv::imwrite(outputName, nowFrame);
									/*std::cout << "detect!" << std::endl;
									cv::waitKey(0);*/
								}
							}

							//cv::waitKey(30);

						}

					}
				}
			}
		}
	}
}