#include "accessDirectory.h"
#include "function.h"

using namespace std;

class Img {
public:
	cv::Mat img;
	cv::Mat channels[3];
};

void byBgrThresholdOnly() {

	const int width = 256;
	const int height = 256;
	const int COLOR_TH = 100; // いい感じの閾値、モノクロには対応できない


	for (int entireItr = 0; entireItr < 1; entireItr++) {
		//----------動画ファイルを開く----------
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
			//videoDir = "../../src_data/OMV200/";

			videoDir = "C:/Users/LAB/Videos/test/";
			toDir = "../../src_data/shots_OMV200/";
		}
		else {
			videoDir = "../..recommendation_test/new_short_videos/dummy/";
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

					//ファイルがオープンできたかの確認
					if (!cap.isOpened()) {
						std::cerr << "cap can't be opened!";
					}

					//----------ショット検出開始----------
					Img preFrame;
					Img nowFrame;

					string fileName;
					stringstream tmp_fileName(videoList[vidItr]);
					string tmp_string;
					vector<string> splitedFileName;
					while (getline(tmp_fileName, tmp_string, '.')) {
						splitedFileName.push_back(tmp_string);
					}
					fileName = splitedFileName[0]; // video00001

					// 1フレーム目は保存
					// 2フレーム目以降でショット検出されたフレームを保存
					// 保存する名前は(元動画の名前)_(フレーム番号)_(フレームの時刻).png

					for (int frameItr = 0; frameItr <= cap.get(cv::CAP_PROP_FRAME_COUNT) - 4; frameItr++) { /*最後の数フレームはショット検出しない*/
						//1フレーム目
						if (frameItr == 0) {
							cap >> nowFrame.img;
							cv::resize(nowFrame.img, nowFrame.img, cv::Size(), width / double(nowFrame.img.cols), height / double(nowFrame.img.rows));

							cv::imshow("video", nowFrame.img);



							string frame2Name;
							intValue2Name(frame2Name, frameItr + 1);
							double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
							string outputName = toDir + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
							//cv::imwrite(outputName, nowFrame.img);
						}
						//2フレーム目以降
						else {
							preFrame.img = nowFrame.img.clone();
							cap >> nowFrame.img;
							cv::resize(nowFrame.img, nowFrame.img, cv::Size(), width / double(nowFrame.img.cols), height / double(nowFrame.img.rows));

							cv::split(preFrame.img, preFrame.channels);
							cv::split(nowFrame.img, nowFrame.channels);

							//----------色で検出----------
							double colorDiff = 0;
							cv::Mat colorDiffMat[3];

							for (int c = 0; c < 3; c++) {
								cv::absdiff(preFrame.channels[c], nowFrame.channels[c], colorDiffMat[c]);

								//画素値の総和/画素数 = 画素値の平均値
								colorDiff += cv::mean(colorDiffMat[c])[0];
							}

							cout << colorDiff << endl;
							cv::imshow("video", nowFrame.img);

							// ショット検出された時の処理
							if (colorDiff > COLOR_TH) {
								string frame2Name;
								intValue2Name(frame2Name, frameItr + 1);
								double elapsedSec = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
								string outputName = toDir + fileName + "/" + fileName + "_" + frame2Name + "_" + to_string(elapsedSec) + ".png";
								//cv::imwrite(outputName, nowFrame.img);
								std::cout << "detect!" << std::endl;
								cv::waitKey(0);

							}
							else {
								if (cv::waitKey(30) >= 0) {
									break;
								};
							}
						}
					}
				}


			}
		}
	}
}
