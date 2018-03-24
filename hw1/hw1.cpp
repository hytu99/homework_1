#include"stdio.h"
#include"process.h"
#include"io.h"
#include"string.h"
#include"stdlib.h"
#include<string>
using namespace std;

#define WORD_MAX_LENTH 1024
#define isLetter(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
#define isNumber(c) (c >= '0' && c <= '9')

int characterNum = 0;
int wordNum = 0;
int lineNum = 0;
typedef struct Node 
{
	char word[WORD_MAX_LENTH];
	int time;
	struct Node *next;
	int length;
	int lenWithoutLastNum;
}wordNode;

int isLetterOrNumber(char ch)
{
	if (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return 1;
	else
		return 0;
}

void Count(char *fileName) 
{
	FILE *fp = NULL;
	char ch;
	char lastChar;
	char tempWord[WORD_MAX_LENTH];//for preserve word temprorily
	wordNode wNode;
	int i = 0, j = 0, firstFlag = 1;

	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("cannot open this file .%s\n", fileName);
		getchar();
		exit(0);
	}
	
	while ((ch = fgetc(fp)) != EOF)
	{	
		if(ch >= 0 && ch <= 127)
			characterNum++;
		if (ch == '\n') 
		{
			lineNum++;
			lastChar = ch;//Preserve last character
		}
		if (firstFlag == 1 ||!((isLetter(ch) || isNumber(ch))))
		{	
			//the first character  or   not letter or number
			firstFlag = 0;
			if (isLetter(ch) || isNumber(ch)) 
			{	
				tempWord[i++] = ch;
			}

			while ((ch = fgetc(fp)) && (isLetter(ch) || isNumber(ch) ))
			{
				characterNum++;
				tempWord[i++] = ch;
			}	
			tempWord[i] = '\0';

			if (i >= 4 && isLetter(tempWord[0]) && isLetter(tempWord[1]) && isLetter(tempWord[2]) && isLetter(tempWord[3]))
			{
				// it's a word
				wordNum++;
			}

			if (ch >= 0 && ch <= 127)
				characterNum++;
			if (ch == '\n')
			{
				lineNum++;
				lastChar = ch;//Preserve last character
			}

			i = 0;

		}
	}

	fclose(fp);
	if (lastChar != '\n')
		lineNum++;
	return;
}
void TraverseFolder(string folderPath) 
{
	//Traverse a folder using Depth First Search Traversal

	if (folderPath.empty())
	{
		printf("The folder path is empty!\n");
		exit(0);
	}

	_finddata_t fileInfo;
	string filePath;
	filePath = folderPath + "\\*";
	long Handle = _findfirst(filePath.c_str(), &fileInfo);
	
	if (Handle == -1L) 
	{
		printf("cannot match the folder path\n");
		exit(0);
	}
	do 
	{
		//Judge if there are subdirectories
		if (fileInfo.attrib & _A_SUBDIR) 
		{
			//Exclude two cases
			if ((strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0)) 
			{	
				//Traverse subdirectories
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

int main(int argc, char *argv[]) 
{

	/*_finddata_t fileInfo;
	long Handle = _findfirst(argv[1], &fileInfo);
	if (Handle != -1L && (fileInfo.attrib & _A_SUBDIR) == 0)
	{
		//It's a file name and not a folder path 
		printf("%s\n", fileInfo.name);
	}
	else
	{	
		//it's a folder path
		TraverseFolder(argv[1]);
	}*/
	char s[] = "D:\\test\\123.txt";
	Count(s);
	printf("%d %d %d\n", characterNum, lineNum, wordNum);
	getchar();
	return 0;
}