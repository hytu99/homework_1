#include"stdio.h"
#include"process.h"
#include"io.h"
#include"string.h"
#include"stdlib.h"
#include<string>
using namespace std;

#define WORD_MAX_LENGTH 250
#define isLetter(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
#define isNumber(c) (c >= '0' && c <= '9')
#define HASHMOD 21248619

int characterNum = 0;
int wordNum = 0;
int lineNum = 0;
typedef struct Node 
{
	char word[WORD_MAX_LENGTH];
	char wordPreLow[WORD_MAX_LENGTH];//word excluding numbers in the end
	int time;
	struct Node *next;
}wordNode, *wordList;

wordList headList[HASHMOD];

int isLetterOrNumber(char ch)
{
	if (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return 1;
	else
		return 0;
}

void Mystrncpy_lwr(char* &s, char *t, int k)
{

	int i;
	for(i = 0; i < k; i++)
	{	
		s[i] = t[i];
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 32;
	}
	s[i] = '\0';
}

int ELFhash(char* key) 
{
	unsigned long h = 0, g;
	while (*key)
	{
		h = (h << 4) + *key++;
		g = h & 0xF0000000L;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return(h % HASHMOD);
}

void CountFrequency(char word[], int length, int lengthPre) 
{
	char* wordPreLow = new char[lengthPre + 1];
	int h;
	wordList wNode,p,q = NULL;

	Mystrncpy_lwr(wordPreLow, word, lengthPre);

	h = ELFhash(wordPreLow);
	if(headList[h] == NULL)
	{
		wNode = new wordNode;
		wNode->time = 1;
		strcpy(wNode->word, word);
		strcpy(wNode->wordPreLow, wordPreLow);
		wNode->next = NULL;
		headList[h] = wNode;
		headList[h]->next = NULL;
	}
	else
	{
		p = headList[h];
		while (p != NULL)
		{
			if (strcmp(p->wordPreLow, wordPreLow) == 0)
			{
				p->time++;
				if (strcmp(word, p->word) < 0)
				{
					strcpy(p->word, word);
				}
				break;
			}
			q = p;
			p = p->next;
		}
		if (p == NULL)
		{
			wNode = new wordNode;
			wNode->time = 1;
			strcpy(wNode->word, word);
			strcpy(wNode->wordPreLow, wordPreLow);
			wNode->next = NULL;
			q->next = wNode;
		}
	}
}

void CountQuantity(const char* fileName) 
{
	FILE *fp = NULL;
	char ch;
	char lastChar;
	char tempWord[WORD_MAX_LENGTH];//for preserving word temprorily
	int i = 0, j = 0, firstFlag = 1;

	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("cannot open this file %s\n", fileName);
		getchar();
		exit(0);
	}
	
	while ((ch = fgetc(fp)) != EOF)
	{	
		if(ch > 0 && ch <= 127 && ch != '\n')
			characterNum++;
		if (ch == '\n') 
		{
			lineNum++;
			lastChar = ch;//Preserve last character
		}
		if (isLetter(ch) || isNumber(ch))
		{
			tempWord[i++] = ch;
			while ((ch = fgetc(fp)) && (isLetter(ch) || isNumber(ch)))
			{
				characterNum++;
				tempWord[i++] = ch;
			}
			tempWord[i] = '\0';

			if (i >= 4 && isLetter(tempWord[0]) && isLetter(tempWord[1]) && isLetter(tempWord[2]) && isLetter(tempWord[3]))
			{
				// it's a word
				wordNum++;
				for (j = i - 1; isNumber(tempWord[j]); j--)
				{
				}
				CountFrequency(tempWord, i, j + 1);
			}

			if (ch > 0 && ch <= 127 && ch != '\n')
				characterNum++;
			if (ch == '\n')
			{
				lineNum++;
				lastChar = ch;//Preserve last character
			}

			i = 0;
		}
		else
		{
			continue;
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
			//printf("%s\n", fileInfo.name);
			string fullName;
			fullName = folderPath + "\\" + fileInfo.name;
			CountQuantity(fullName.c_str());
		}
	} while (_findnext(Handle, &fileInfo) == 0);

	_findclose(Handle);

}

int main(int argc, char *argv[]) 
{
	int i;
	wordNode *p;
	/*_finddata_t fileInfo;
	long Handle = _findfirst(argv[1], &fileInfo);
	int i;

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
	CountQuantity(s);

	//TraverseFolder(s);
	
	printf("characters: %d\n", characterNum);
	printf("words: %d\n", wordNum);
	printf("lines: %d\n", lineNum);
	
	/*for (i = 0; i < HASHMOD; i++)
	{
		if (headList[i] != NULL)
		{	
			p = headList[i];
			while (p != NULL) 
			{	
				if(p->time >= 80000)
					printf("%s :%d\n", headList[i]->word, headList[i]->time);
				p = p->next;
			}
		}
	}*/

	getchar();
	return 0;
}