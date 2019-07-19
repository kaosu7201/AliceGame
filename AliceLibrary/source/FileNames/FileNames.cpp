#include <Windows.h>
#include "FileNames.h"

list<string> GetTexPackFilePathInDir(const string& dir_name)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	list<string> file_names;
	string str;
	// png,jpg,bmp,binの拡張子のファイルのみを読み込む
	string extension[] = { "png" ,"jpg", "bmp", "bin" };

	for (int i = 0; i < 4; i++) {

		string search_name = dir_name + "*." + extension[i];
		hFind = FindFirstFile(search_name.c_str(), &win32fd);

		if (hFind == INVALID_HANDLE_VALUE) {
			continue;
		}
		do {
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			}
			else {
				str = dir_name + win32fd.cFileName;
				if (str.find("bin") == string::npos) {
					file_names.push_back(str);
				}
				else {
					file_names.push_front(str);
				}
			}
		} while (FindNextFile(hFind, &win32fd));

		FindClose(hFind);
	}
	return file_names;
}