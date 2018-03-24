#include"stdio.h"
#include"process.h"
#include"io.h"
#include"string.h"
#include<string>
using namespace std;

void TraverseFolder(string folderPath) {
	//Traverse a folder using Depth First Search Traversal

	if (folderPath.empty()){
		printf("The folderPath is empty!\n");
		exit(0);
	}

	_finddata_t fileInfo;
	string filePath;
	filePath = folderPath + "\\*";
	long Handle = _findfirst(filePath.c_str(), &fileInfo);
	
	if (Handle == -1L) {
		printf("cannot match the folder path\n");
		exit(0);
	}
	do {
		//Judge if there are subdirectories
		if (fileInfo.attrib & _A_SUBDIR) 
		{
			//Exclude two cases
			if ((strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0)) 
			{
				string newPath;
				newPath = folderPath + "\\" + fileInfo.name;
				TraverseFolder(newPath);
			}
		}
		else
		{
			printf("%s\n", fileInfo.name);
		}
	} while (_findnext(Handle, &fileInfo) == 0);

	_findclose(Handle);
	
}

int main(int argc, char *argv[]) {
	/*FILE *fp;
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("cannot open this file .%s\n", argv[1]);
		exit(0);
	}
	fclose(fp);*/
	TraverseFolder(argv[1]);
	getchar();
	return 0;
}