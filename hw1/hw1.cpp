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
typedef struct wordNode 
{
	char word[WORD_MAX_LENGTH];
	char wordPreLow[WORD_MAX_LENGTH];//word excluding numbers in the end
	int time;
	struct wordNode *next;
}wordNode, *wordList;

typedef struct phraseNode
{
	int time;
	struct phraseNode *next;
	wordList word1;
	wordList word2;
}phraseNode, *phraseList;

wordList headList[HASHMOD];
phraseList headList_Phrase[HASHMOD];
wordNode wordTop10[11];
phraseNode phraseTop10[11];

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

int ELFhash_Phrase(char* key1, char* key2)
{
	unsigned long h = 0, g;
	while (*key1)
	{
		h = (h << 4) + *key1++;
		g = h & 0xF0000000L;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	while (*key2)
	{
		h = (h << 4) + *key2++;
		g = h & 0xF0000000L;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return(h % HASHMOD);
}

wordList CountFrequency(char word[], int lengthPre) 
{
	char* wordPreLow = new char[lengthPre + 1];
	int h;
	wordList wNode = NULL, p, q = NULL;

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
				return p;
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
	return wNode;
}

void CountFrequency_Phrase(wordList word1, wordList word2) {

	int h;
	phraseList pNode, p, q = NULL;
	h = ELFhash_Phrase(word1->wordPreLow, word2->wordPreLow);

	if (headList_Phrase[h] == NULL)
	{
		pNode = new phraseNode;
		pNode->time = 1;
		pNode->word1 = word1;
		pNode->word2 = word2;
		headList_Phrase[h] = pNode;
		headList_Phrase[h]->next = NULL;

	}
	else
	{
		p = headList_Phrase[h];
		while (p != NULL)
		{
			if (strcmp(p->word1->wordPreLow, word1->wordPreLow) == 0 && strcmp(p->word2->wordPreLow, word2->wordPreLow) == 0)
			{
				p->time++;
				break;
			}
			q = p;
			p = p->next;
		}
		if (p == NULL)
		{
			pNode = new phraseNode;
			pNode->time = 1;
			pNode->word1 = word1;
			pNode->word2 = word2;
			pNode->next = NULL;
			q->next = pNode;
		}
	}
}

void CountQuantity(const char* fileName) 
{
	FILE *fp = NULL;
	char ch;
	char lastChar;
	char tempWord[WORD_MAX_LENGTH];//for preserving word temprorily
	wordList wNode, wNodePre = NULL;
	int i = 0, j = 0, flag = 0;

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
			while ((ch = fgetc(fp)) != EOF && (isLetter(ch) || isNumber(ch)))
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
				wNode = CountFrequency(tempWord, j + 1);
				if (flag == 1)
				{
					CountFrequency_Phrase(wNodePre,wNode);
				}
				//strcpy(wNodePre->word, wNode->word);
				//strcpy(wNodePre->wordPreLow, wNode->wordPreLow);
				wNodePre = wNode;
				flag = 1;
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
	int i, j;
	wordNode *p;
	phraseNode *q;
	_finddata_t fileInfo;
	long Handle = _findfirst(argv[1], &fileInfo);

	if (Handle != -1L && (fileInfo.attrib & _A_SUBDIR) == 0)
	{
		//It's a file name and not a folder path 
		CountQuantity(argv[1]);
	}
	else
	{	
		//it's a folder path
		TraverseFolder(argv[1]);
	}
	//char s[] = "D:\\test\\newsample";
	//CountQuantity(s);

	//TraverseFolder(s);
	
	printf("characters: %d\n", characterNum);
	printf("words: %d\n", wordNum);
	printf("lines: %d\n", lineNum);
	
	for (i = 0; i < HASHMOD; i++)
	{
		//find top 10 words
		if (headList[i] != NULL)
		{	
			p = headList[i];
			while (p != NULL) 
			{	
				//if(p->time >= 80000)
				//	printf("%s: %d\n", headList[i]->word, headList[i]->time);
				for (j = 9; j >= 0 && p->time > wordTop10[j].time || ((p->time == wordTop10[j].time) && (strcmp(p->word, wordTop10[j].word) < 0)); j--)
				{
					strcpy(wordTop10[j + 1].word, wordTop10[j].word);
					wordTop10[j + 1].time = wordTop10[j].time;
				}
				if (j < 9) 
				{
					strcpy(wordTop10[j + 1].word, p->word);
					wordTop10[j + 1].time = p->time;
				}
				p = p->next;
			}
		}
	}

	for (i = 0; i <= 9; i++)
	{
		printf("%s: %d\n", wordTop10[i].word, wordTop10[i].time);
	}

	for (i = 0; i < HASHMOD; i++)
	{
		//find top 10 phrases
		
		if (headList_Phrase[i] != NULL)
		{
			q = headList_Phrase[i];
			while (q != NULL)
			{
				for (j = 9; j >= 0 && q->time > phraseTop10[j].time || ((q->time == phraseTop10[j].time) && ((strcmp(q->word1->word, phraseTop10[j].word1->word) < 0)|| (strcmp(q->word2->word, phraseTop10[j].word2->word) < 0))); j--)
				{
					phraseTop10[j + 1].word1 = phraseTop10[j].word1;
					phraseTop10[j + 1].word2 = phraseTop10[j].word2;
					phraseTop10[j + 1].time = phraseTop10[j].time;
				}
				if (j < 9)
				{
					phraseTop10[j + 1].word1 = q->word1;
					phraseTop10[j + 1].word2 = q->word2;
					phraseTop10[j + 1].time = q->time;
				}
				q = q->next;
			}
		}
	}

	for (i = 0; i <= 9; i++)
	{
		printf("%s %s: %d\n", phraseTop10[i].word1->word, phraseTop10[i].word2->word, phraseTop10[i].time);
	}

	getchar();
	return 0;
}