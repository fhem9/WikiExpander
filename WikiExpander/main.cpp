//#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//wiki_expander.cpp
/*
TCHAR *findstr_reverse(//const TCHAR *str,//const TCHAR *find,//const TCHAR* end)
{
	int len=lstrlen(str);
	if(!end){
		end=str+len;
	}else{
		len=end-str;
	}
	int findlen=strlen(find);
	if(findlen>len)return NULL;
	if(findlen==len)return lstrcmp(str,find)==0? str:NULL;
	TCHAR *findread=find+findlen;
	while(end>=str)
	{
		int i=0;
		while(find<=findread-i)
		{
			//버그!!!! 
			//puts("버그!");
			if(end[i]!=findread[i])
			{
				end+=i;
				//puts("findread not equal"); 
				goto notmatch1;//break abnormally
			}
			i--; 
		}
		success:
			return end;
		notmatch1:
			continue;
	}
	return NULL;
}
*/

bool showprog=true;
void ExpandWiki(TCHAR *mapview,FILE *out)
{
	TCHAR *reader=mapview;
	TCHAR *end=mapview+lstrlen(mapview);
	
	TCHAR *commentStart=strstr(reader,"[1]");
	if(!commentStart)
	{
		fputs(reader,out);
		return;
	}
	commentStart= strstr(commentStart+1,"[1]");
	//puts(commentStart);
	while(*reader!=NULL&&reader<commentStart)
	{
		TCHAR *c=reader;
		if((char)*c=='['&&isdigit(c[1]))
		{
			TCHAR buf[8];
			memset(buf,0,sizeof(TCHAR)*8);
			TCHAR *d=c+1;
			while(*d!=NULL&&*d!=']')
			{
				buf[d-c-1]=*d;
				d++;
			}
			if(*d==NULL)
			{
				fputc(*d,out);
				//fclose(out);
				return;//abort!!
			}
			if(*d==']')
			{
				//주석 참조찾음!!
				int commentnum=strtoul(buf,NULL,10);
				fprintf(out,"([",buf);
				TCHAR *commentAddr=strstr(commentStart,buf);			//findstr_reverse(mapview,buf,NULL);
				if(!commentAddr)
				{
					fputs("\n주석을 찾을수 없습니다!!)",out);
					continue;
				}
				//주석을 전개한다
				sprintf(buf,"\n[%d]",commentnum+1);//다음주석전까지
				TCHAR *commentend=strstr(commentAddr,buf);
				int commentlen=0;
				if(commentend)
				{
					commentend--;
					commentlen=commentend-commentAddr;
				}
				for(int j=0;j<commentlen-1;++j)
				{
					fputc(commentAddr[j],out);
				}
				//fputs(commentAddr,out);
				fputc(')',out);
				fputc(' ',out);
				fputc(' ',out);
				//전개끝
				++reader;
				++reader;
			}
		}
		else
		{
			if(showprog==true){
				putc(*c,stdout);
			}
				
			fputc(*c,out);
		}
		++reader;
	}
}

int main(int argc, char** argv)
{
	TCHAR infilename[MAX_PATH];
	TCHAR outfilename[MAX_PATH];
	memset(infilename,0,MAX_PATH);
	memset(outfilename,0,MAX_PATH);
	puts("input wiki file name:");
	scanf("%240s",infilename);
	lstrcpy(outfilename,infilename);
	lstrcat(outfilename,"_out.txt");
	puts(outfilename);
	puts("progress?");
	int c=getche();
	showprog= (c=='y'||c=='Y');
	//printf("prog=%s",showprog? "true":"false");
	//system("pause");
	HANDLE hFile = CreateFile(infilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		puts("error opening This File");
		return -1;
	}
	HANDLE hImgMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hImgMap == NULL)
	{
		puts("error Creating File Mapping!!");
		return -1;
	}
	
	PVOID pImgView = MapViewOfFile(hImgMap, FILE_MAP_READ, 0, 0, 0);
	if (pImgView == NULL)
	{
		puts("error reading files");
		return -1;
	}
	FILE *out=fopen(outfilename,"wt");
	if(out==NULL)
	{
		puts("erroer Creating out file");
		return -1;
	}
	
	ExpandWiki((TCHAR *)pImgView,out);
	fclose(out);
	UnmapViewOfFile(pImgView);
	CloseHandle(hImgMap);
	CloseHandle(hFile);
	puts("success");
	system("pause");
	return 0;
}
