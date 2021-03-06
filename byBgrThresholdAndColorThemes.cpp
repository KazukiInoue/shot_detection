#include "accessDirectory.h"
#include "function.h"

using namespace std;

void byBgrThresholdAndColorThemes() {

	const int width = 256;
	const int height = 256;

	const int clusterNum = 20; /*5以上を指定してください*/
	const int numToExtract = 20; /*clusterNum以下の値を指定してください*/

	bool isTrain = true;
	int indexValue = 1;

	string videoDir[2] = {
		"../../src_data/IMV133/",
		"../../src_data/OMV200/"};

	string toDir[2] = {
		"../../src_data/shots_IMV133_threshold=130_themes=6/", 
		"../../src_data/shots_OMV200_threshold=130_themes=6/"};


	for (int entireItr = 0; entireItr < 1; entireItr++) {
		//----------動画ファイルを開く----------


		std::vector<std::string> videoList = Dir::read(videoDir[entireItr]);

		for (int vidItr = 0; vidItr < videoList.size(); vidItr++) {

			stringstream tmpFileName(videoList[vidItr]);
			string tmpString;
			vector<string> splitedFileName;

			while (getline(tmpFileName, tmpString, '.')) {
				splitedFileName.push_back(tmpString);
			}

			if (splitedFileName.size() > 1) { //..というファイルが入力されることを防ぐ
				if (splitedFileName[1] == "mp4") {
					string videoFile = videoDir[entireItr] + videoList[vidItr];

					cout << videoFile << endl;
					cv::VideoCapture cap(videoFile);

					//ファイルがオープンできたかの確認
					if (!cap.isOpened()) {
						std::cerr << "cap can't be opened!" << endl;
						exit(1);
					}

					//----------ショット検出開始----------

					cv::Mat preFrame;
					cv::Mat nowFrame;

					vector<vector<double>> preColorThemes(numToExtract, vector<double>(3, 0));
					vector<vector<double>> nowColorThemes(numToExtract, vector<double>(3, 0));

					string fileName= splitedFileName[0]; // video00001

					/* 1フレーム目は保存
					   2フレーム目以降でショット検出されたフレームを保存
					   保存する名前は(元動画の名前)_(フレーム番号)_(フレームの時刻).png*/

					for (int frameItr = 0;; frameItr++) { //最後の数フレームはショット検出しない
						//1フレーム目
						if (frameItr == 0) {

							cap >> nowFrame;
							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));

							// cv::imshow("video", nowFrame);

							// extract color themes
							cv::Mat subtImg(nowFrame.size(), nowFrame.type());
							vector<vector<double>> clusterInfo; /*select_principal_color_themesで使用する*/

							kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
							selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

							// 保存
							string frame2Name;
							intValue2Name(frame2Name, frameItr + indexValue);
							double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
							string outputName = toDir[entireItr] + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
							cv::imwrite(outputName, nowFrame);
						}
						//2フレーム目以降
						else {

							preFrame = nowFrame.clone();
							preColorThemes = nowColorThemes;

							cap >> nowFrame;

							if (nowFrame.empty()) {
								break;
							}

							cv::resize(nowFrame, nowFrame, cv::Size(), width / double(nowFrame.cols), height / double(nowFrame.rows));

							//cv::imshow("video", nowFrame);

							// 1.前フレームとのBGRの値の差を計算し、閾値以上なら次の処理へ
							bool isPossibleBoundary = false;
							judgeByBgrThreshold(/*&*/isPossibleBoundary, nowFrame, preFrame);

							if (isPossibleBoundary) {

								// 2.前フレームとのcolor theme(よく使われているカラーパレットみたいなもの）の変化を用いてショット検出
								cv::Mat subtImg(nowFrame.size(), nowFrame.type());
								vector<vector<double>> clusterInfo; /*select_principal_color_themesで使用する*/

								kMeansColorSubtraction(/*&*/subtImg, /*&*/clusterInfo, nowFrame, clusterNum);
								selectPrincipalColorThemes(/*&*/nowColorThemes, subtImg, clusterNum, numToExtract, clusterInfo);

								bool isBoundary = false;
								inspectThemesConsensus(/*&*/isBoundary, nowColorThemes, preColorThemes);

								//ショット検出されたら保存
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