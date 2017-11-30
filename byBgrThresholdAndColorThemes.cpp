#include "accessDirectory.h"
#include "function.h"

using namespace std;

void byBgrThresholdAndColorThemes() {

	const int width = 256;
	const int height = 256;

	for (int entireItr = 0; entireItr < 1; entireItr++) {
		//----------����t�@�C�����J��----------
		bool isTrain = true;
		if (entireItr == 0) {
			isTrain = true;
		}
		else {
			isTrain = false;
		}

		string videoDir = "";
		string toDir = "";

		if (isTrain) {
			videoDir = "../../src_data/OMV200/";
			toDir = "/../src_data/shots_OMV200/";
		}
		else {
			videoDir = "../../recommendation_test/new_short_videos/dummy/";
			toDir = "../../shot_detection/output_first_frames_of_shots/recommendation_test/";
		}

		std::vector<std::string> videoList = Dir::read(videoDir);

		for (int vidItr = 0; vidItr < videoList.size(); vidItr++) {

			string fileName;
			stringstream tmp_fileName(videoList[vidItr]);
			string tmp_string;
			vector<string> splitedFileName;

			while (getline(tmp_fileName, tmp_string, '.')) {
				splitedFileName.push_back(tmp_string);
			}
			fileName = splitedFileName[0];

			if (splitedFileName.size() > 1) {
				if (splitedFileName[1] == "mp4") {
					string videoFile = videoDir + videoList[vidItr];

					cout << videoFile << endl;
					cv::VideoCapture cap(videoFile);

					//�t�@�C�����I�[�v���ł������̊m�F
					if (!cap.isOpened()) {
						std::cerr << "cap can't be opened!";
						// �G���[���������ċ����I����������
					}

					//----------�V���b�g���o�J�n----------

					cv::Mat preFrame;
					cv::Mat nowFrame;

					const int clusterNum = 20; /*5�ȏ���w�肵�Ă�������*/
					const int numToExtract = 20; /*clusterNum�ȉ��̒l���w�肵�Ă�������*/

					vector<vector<double>> preColorThemes(numToExtract, vector<double>(3, 0));
					vector<vector<double>> nowColorThemes(numToExtract, vector<double>(3, 0));

					string fileName;
					stringstream tmp_fileName(videoList[vidItr]);
					string tmp_string;
					vector<string> splitedFileName;
					while (getline(tmp_fileName, tmp_string, '.')) {
						splitedFileName.push_back(tmp_string);
					}
					fileName = splitedFileName[0]; // video00001

					/* 1�t���[���ڂ͕ۑ�
					   2�t���[���ڈȍ~�ŃV���b�g���o���ꂽ�t���[����ۑ�
					   �ۑ����閼�O��(������̖��O)_(�t���[���ԍ�)_(�t���[���̎���).png*/

					for (int i = 0; i < cap.get(cv::CAP_PROP_FRAME_COUNT) - 4; i++) { //�Ō�̐��t���[���̓V���b�g���o���Ȃ�
						//1�t���[����
						if (i == 0) {
							cap >> nowFrame;
							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));

							cv::imshow("video", nowFrame);

							// extract color themes
							cv::Mat subtImg(nowFrame.size(), nowFrame.type());
							vector<vector<double>> clusterInfo; /*select_principal_color_themes�Ŏg�p����*/

							kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
							selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

							// �ۑ�
						/*	string frame2Name;
							intValue2Name(frame2Name, i + 1);
							double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
							string outputName = toDir + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
							cv::imwrite(outputName, nowFrame);*/
						}
						//2�t���[���ڈȍ~
						else {
							preFrame = nowFrame.clone();
							preColorThemes = nowColorThemes;

							cap >> nowFrame;

							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));
							cv::imshow("video", nowFrame);

							// 1.�O�t���[���Ƃ�BGR�̒l�̍����v�Z���A臒l�ȏ�Ȃ玟�̏�����
							// 2.�O�t���[���Ƃ�color theme(�悭�g���Ă���J���[�p���b�g�݂����Ȃ��́j�̕ω���p���ăV���b�g���o

							// judge by bgr values
							bool isPossibleBoundary = false;
							judgeByBgrThreshold(/*&*/isPossibleBoundary, nowFrame, preFrame);

							if (isPossibleBoundary) {

								// extract color themes
								cv::Mat subtImg(nowFrame.size(), nowFrame.type());
								vector<vector<double>> clusterInfo; /*select_principal_color_themes�Ŏg�p����*/

								kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
								selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

								// color themes�̃p���b�g�Ƃ��Ă̈�v�x�����ăV���b�g���o
								bool isBoundary = false;
								inspectThemesConsensus(/*&*/isBoundary, nowColorThemes, preColorThemes);

								//�V���b�g���o���ꂽ��ۑ�
								if (isBoundary) {
									string frame2Name;
									intValue2Name(frame2Name, i + 1);
									double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
									string outputName = toDir + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
									//cv::imwrite(outputName, nowFrame);
									std::cout << "detect!" << std::endl;
									cv::waitKey(0);

								}
							}


							cv::waitKey(30);

						}

					}
				}
			}
		}
	}
}