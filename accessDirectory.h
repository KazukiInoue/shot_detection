
// About "class Dir", I refer to http://tanjoin.hatenablog.com/entry/20110810/1312966845


#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>
#include<fstream>

using namespace std;

// �t�H���_�̒��̃t�@�C����񋓂���N���X
class Dir
{
public:
	// �R���X�g���N�^
	Dir(void) {}
	// �f�X�g���N�^
	virtual ~Dir(void) {}

	// �t�@�C���ꗗ�擾
	// folder : �t�H���_�̐�΃p�X����͂Ƃ��� 
	// �� : "D:\\Users\\Pictures\\"
	static vector<string> read(string folder) {
		// �錾
		vector<string> fileList;
		HANDLE hFind;
		WIN32_FIND_DATA fd;

		// �t�@�C���������̂��߂Ƀ��C���h�J�[�h�ǉ�
		// �� : "D:\\Users\\Pictures\\*.*"
		stringstream ss;
		ss << folder;
		string::iterator itr = folder.end();
		itr--;
		if (*itr != '\\') ss << '\\';
		ss << "*.*";

		// �t�@�C���T��
		// FindFirstFile(�t�@�C����, &fd);
		hFind = FindFirstFile(ss.str().c_str(), &fd);

		// �������s
		if (hFind == INVALID_HANDLE_VALUE) {
			std::cout << "�t�@�C���ꗗ���擾�ł��܂���ł���" << std::endl;
			exit(1); // �G���[�I��
		}

		// �t�@�C���������X�g�Ɋi�[���邽�߂̃��[�v
		do {

			//�t�@�C���������X�g�Ɋi�[
			char *file = fd.cFileName;
			string str = file;
			fileList.push_back(str);

			//// �t�H���_�͏���
			//if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			//	&& !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			//{
			//	//�t�@�C���������X�g�Ɋi�[
			//	char *file = fd.cFileName;
			//	string str = file;
			//	fileList.push_back(str);
			//}

		} while (FindNextFile(hFind, &fd)); //���̃t�@�C����T��

											// hFind�̃N���[�Y
		FindClose(hFind);

		return fileList;
	}
};