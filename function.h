#pragma once

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

using namespace std;

void byBgrThresholdOnly();
void byBgrThresholdAndColorThemes();
void inspectThemesConsensus(bool &isBoundary, vector<vector<double>> nowColorThemes, vector<vector<double>> preColorThemes);
void judgeByBgrThreshold(bool &isPossibleBoundary, cv::Mat nowFrame, cv::Mat preFrame);
void kMeansColorSubtraction(cv::Mat &dst, std::vector<std::vector<double>> &clusterInfo, cv::Mat src, const int clusterNum);
void selectPrincipalColorThemes(vector<vector<double>> &colorThemes, cv::Mat src, const int clusterNum, const int numToExtract, vector<vector<double>> clusterInfo);

void testExtractedColorInputPart();

void intValue2Name(string& name, int value);