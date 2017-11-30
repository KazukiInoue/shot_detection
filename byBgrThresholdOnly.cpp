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
	const int COLOR_TH = 100; // ����������臒l�A���m�N���ɂ͑Ή��ł��Ȃ�


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

					//�t�@�C�����I�[�v���ł������̊m�F
					if (!cap.isOpened()) {
						std::cerr << "cap can't be opened!";
					}

					//----------�V���b�g���o�J�n----------
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

					// 1�t���[���ڂ͕ۑ�
					// 2�t���[���ڈȍ~�ŃV���b�g���o���ꂽ�t���[����ۑ�
					// �ۑ����閼�O��(������̖��O)_(�t���[���ԍ�)_(�t���[���̎���).png

					for (int frameItr = 0; frameItr <= cap.get(cv::CAP_PROP_FRAME_COUNT) - 4; frameItr++) { /*�Ō�̐��t���[���̓V���b�g���o���Ȃ�*/
						//1�t���[����
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
						//2�t���[���ڈȍ~
						else {
							preFrame.img = nowFrame.img.clone();
							cap >> nowFrame.img;
							cv::resize(nowFrame.img, nowFrame.img, cv::Size(), width / double(nowFrame.img.cols), height / double(nowFrame.img.rows));

							cv::split(preFrame.img, preFrame.channels);
							cv::split(nowFrame.img, nowFrame.channels);

							//----------�F�Ō��o----------
							double colorDiff = 0;
							cv::Mat colorDiffMat[3];

							for (int c = 0; c < 3; c++) {
								cv::absdiff(preFrame.channels[c], nowFrame.channels[c], colorDiffMat[c]);

								//��f�l�̑��a/��f�� = ��f�l�̕��ϒl
								colorDiff += cv::mean(colorDiffMat[c])[0];
							}

							cout << colorDiff << endl;
							cv::imshow("video", nowFrame.img);

							// �V���b�g���o���ꂽ���̏���
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
