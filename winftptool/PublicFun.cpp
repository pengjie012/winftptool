#include "StdAfx.h"
#include "PublicFun.h"
#include <sys/stat.h> 
#include <io.h>

string g_rootpth="";
int CreatePath(const char* folder,bool all)
{
	 int errcount = 0;
	 
	 if(_access(folder, 0) == 0)
		 ::SetFileAttributes(folder, 0);
	 
	 if(all)
	 {
		 TCHAR path[MAX_PATH];
		 memset(path,0,MAX_PATH*sizeof(TCHAR));
		 
		 for(int i= 0 ; folder[i] ; ++i)
		 {
			 if(folder[i] == '\\') 
				 errcount += CreatePath(path,FALSE);
			 path[i] = folder[i];
		 }
		 errcount += CreatePath(path,FALSE);
	 }
	 else if(_access(folder,0) != 0 )
	 {
		 if(!::CreateDirectory(folder, NULL))
			 errcount ++; 
	 }
	 
	 return errcount;
}
void SplitString(const string &src_str, const string &separator,
				 vector<string> &part_strings)
{
	if (src_str.empty() || separator.empty())
		return;

	string tmp_str;
	const char *head_pos = src_str.c_str();
	const char *tail_pos = NULL;
	unsigned int separator_len = separator.size();

	while (head_pos != NULL)
	{
		tail_pos = strstr(head_pos, separator.c_str());

		if (tail_pos != NULL)
		{
			tmp_str = string(head_pos, tail_pos - head_pos);
			part_strings.push_back(tmp_str);

			head_pos = tail_pos + separator_len;
			tail_pos = NULL;
		}
		else
		{
			if (strlen(head_pos) > 0)
			{
				part_strings.push_back(head_pos);
			}

			break;
		}
	}
}

CString GetApiStr(const char* node , const char* key, const char* defaultvalue,const char* inifilepath)
{

	CString ReturnStr;
	CString DefaultStr;
	DefaultStr.Format("%s","null");
	CString session;
	session.Format("%s",node);
	CString keystr;
	keystr.Format("%s",key);
	::GetPrivateProfileString(session,keystr,DefaultStr,ReturnStr.GetBuffer(100),100,inifilepath);

	ReturnStr.ReleaseBuffer();
	if(ReturnStr.Compare("null")==0||(ReturnStr.IsEmpty()))
	{
		
		return "";	
	}
	ReturnStr.TrimLeft();
	ReturnStr.TrimRight();
	return ReturnStr;
}

CString GetFileDir1(CString filepath)
{
	CString res;
	int pos1;
	pos1=filepath.ReverseFind('\\');
	res=filepath.Left(pos1);
	return res;
}
 
CString GetCurWorkDir()
{
	CString retstr;

	char	ReSetup[MAX_PATH];
	memset(ReSetup, 0, MAX_PATH);
	GetModuleFileNameA(NULL, ReSetup, MAX_PATH);
	CString tmpstr=GetFileDir1(ReSetup);
	


	return tmpstr;
}

/**

 * check a file is a directory or  file

 @return 0 is a directory, -1 not exit ,1 file

 */

 bool IsDirectory(const char* filename)
{

  DWORD dwAttr = ::GetFileAttributes(filename);  //�õ��ļ�����
  if (dwAttr == 0xFFFFFFFF)    // �ļ���Ŀ¼������
    return false;
    else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)  // �����Ŀ¼

    return true; 

  else
    return false;

}
 //�ж��ļ��Ƿ����
 bool FileIsExist(const char* filename)
 {
	 if(_access(filename,0)!=-1)
	 {
		return true;
	 }
	return false;
 }
 //��ȡ�ļ���С
unsigned int GFileSize(string filename)
{
    struct stat file;

    int result=stat(filename.c_str(),&file);

    if(result==0)
    {
        return file.st_size;
    }
    else
    {
        return -1;
    }

    return -1;
}
//���ļ�
bool ReadFromFile(const char * filename,char* filebuf, int len)
{

    FILE *pfile=fopen(filename,"rb");
    if(pfile)
    {
        fread(filebuf,1,len,pfile);
        fclose(pfile); 
    }
    return true;
}
/**
 * ����ַ���
 * @param buf 
 * @param len 
 * @param e 
 * @param sVector 
 * @param isdelempty 
 */
void SplitStoV(char* buf,int len,char* e, std::vector<string>& sVector, bool isdelempty)
{
    sVector.clear();
    int count=0;
    char * tp=buf;
    int splitword=strlen(e);
    char * p=(char*)strstr(buf,e);
    while(p!=NULL)
    {
        *p = 0;
        int off=p-tp;
        if(! ( isdelempty && off==0 ) )
        {
            sVector.push_back(tp);
            count++;
        }
        tp=p+splitword;
        len-=(off+splitword);
        p=(char*)strstr(tp,e);
    }
    if(len>0)
    {
        sVector.push_back(tp);
        count++;
        return;
    }
    if( !isdelempty && len == 0 )
    {
        sVector.push_back(tp);
        count++;
    }
    return;
}
CString GetRunPath()
{
	CString sPath;
	::GetModuleFileName (NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos);
	return sPath;
}
/**
 * �ָ��ǰ���ַ���
 *@param str ԭ�� 
 *@param sp  �ָ 
*/
string GetForntStr(string str,string sp)
{
	int p=str.find(sp);
	if(p!=string::npos)
	{
		return str.substr(0,p);
	}
	return "";
}
/**
 *�����ļ�
 *@P strDir Ҫ��������ʼĿ¼
 *@P filearr ����������
 *@P fixstr filetype
*/

void TravelFolder(CString strDir,CString fixstr,vector<CString> &filearr)
{
    CFileFind filefind;                                         //����CFileFind���ͱ���
    CString strWildpath = strDir + _T("\\*.*");     //�����ļ����г���
    if(filefind.FindFile(strWildpath, 0))	                   //��ʼ�����ļ�
    {
        BOOL bRet = TRUE;
        while(bRet)
        {
            bRet = filefind.FindNextFile();                 //ö��һ���ļ�
            if(filefind.IsDots())                                 //�����. �� .. ����һ��
                continue;
            if(!filefind.IsDirectory())                          //������Ŀ¼�����ļ�����ӡ����
            {
				CString fileName= filefind.GetFileName();
				int pos=fileName.ReverseFind('.');
				CString strLastName=fileName.Right(fileName.GetLength()-pos-1);
				if(strLastName.Compare("tmp")==0||strLastName.Compare("temp")==0)
				{
					continue;
				}
				if(strLastName.Compare(fixstr)==0||fixstr.CompareNoCase("all")==0)
				{
					 CString strTextOut;
					 if(strDir.ReverseFind('\\')==(strDir.GetLength()-1))
					 {
						strTextOut = strDir  +fileName;
					 }
					 else
					 {
						strTextOut = strDir + CString(_T("\\")) +fileName;
					 }
					 filearr.push_back(strTextOut);
				}
            }
            else                                                    //�������Ŀ¼���ݹ���øú���
            {
                CString strTextOut = strDir + CString(_T("\\")) + filefind.GetFileName();

                TravelFolder(strTextOut,fixstr,filearr);//�ݹ���øú�����ӡ��Ŀ¼����ļ�
            }
        }
        filefind.Close();
    }
}
void TraveloneFolder(CString strDir,CString fixstr,vector<CString> &filearr)
{
    CFileFind filefind;                                         //����CFileFind���ͱ���
    CString strWildpath = strDir + _T("\\*.*");     //�����ļ����г���
    if(filefind.FindFile(strWildpath, 0))	                   //��ʼ�����ļ�
    {
        BOOL bRet = TRUE;
        while(bRet)
        {
            bRet = filefind.FindNextFile();                 //ö��һ���ļ�
            if(filefind.IsDots())                                 //�����. �� .. ����һ��
                continue;
            if(!filefind.IsDirectory())                          //������Ŀ¼�����ļ�����ӡ����
            {
				CString fileName= filefind.GetFileName();
				int pos=fileName.ReverseFind('.');
				CString strLastName=fileName.Right(fileName.GetLength()-pos-1);
				if(strLastName.Compare("tmp")==0||strLastName.Compare("temp")==0)
				{
					continue;
				}
				if(strLastName.Compare(fixstr)==0||fixstr.CompareNoCase("all")==0)
				{
					 CString strTextOut;
					 if(strDir.ReverseFind('\\')==(strDir.GetLength()-1))
					 {
						strTextOut = strDir  +fileName;
					 }
					 else
					 {
						strTextOut = strDir + CString(_T("\\")) +fileName;
					 }
					 filearr.push_back(strTextOut);
				}
            }
            else                                                    //�������Ŀ¼���ݹ���øú���
            {
                CString strTextOut = strDir + CString(_T("\\")) + filefind.GetFileName();

                TravelFolder(strTextOut,fixstr,filearr);//�ݹ���øú�����ӡ��Ŀ¼����ļ�
            }
        }
        filefind.Close();
    }
}
int search_direct(const char *dir,std::vector<string>& files)
{
	int retnum=0;
	CFileFind filefind;                                         //����CFileFind���ͱ���
    CString strWildpath;
	strWildpath.Format("%s\\*.*", dir);     //�����ļ����г���
    if(filefind.FindFile(strWildpath, 0))	                   //��ʼ�����ļ�
    {
        BOOL bRet = TRUE;
        while(bRet)
        {
            bRet = filefind.FindNextFile();                 //ö��һ���ļ�
            if(filefind.IsDots())                                 //�����. �� .. ����һ��
                continue;
            if(filefind.IsDirectory())                          //������Ŀ¼�����ļ�����ӡ����
            {
				CString fileName= filefind.GetFileName();
				string filenamestr=fileName;
				files.push_back(filenamestr);
				retnum++;
            }
            else                                                    //�������Ŀ¼���ݹ���øú���
            {
               continue;
            }
        }
        filefind.Close();
    }
	return retnum;
}
//ɾ��Ŀ¼
 void DeleteDirectories(CString csPath)  
{     
	  CFileFind finder;  
	 CString tempPath;  
	 tempPath.Format("%s%s", csPath, "//*.*");  
	 BOOL bWork = finder.FindFile(tempPath);  
	 while(bWork)  
	 {         
	      bWork = finder.FindNextFile();  
	      if(!finder.IsDots())  
		  {  
		        if(finder.IsDirectory())  
		        {  
		            DeleteDirectories(finder.GetFilePath());  
		        }  
		        else  
		        {  
		            DeleteFile(finder.GetFilePath());  
		        }  
		   }  
	 }  
	 finder.Close();  
	 RemoveDirectory(csPath);  
 } 
void copypath(CString srcDir,CString dstDir)
{
	CString fix;
	fix.Format("all");
	vector<CString> filearr;
	TravelFolder(srcDir,fix,filearr);
	int arrsize=filearr.size();
	for(int n=0;n<arrsize;n++)
	{
		CString file;
		file.Format("%s",filearr.at(n));
		file.Replace(srcDir,dstDir);
		CString filepath;
		bool flg=GetFilePathWin(file,filepath);
		if(flg)
		{
			CreatePath(filepath);
		}
		rename(filearr.at(n),file);
	}
	return;

}
//д�ļ�
bool WriteToFile(const char * filename,const char * buf,unsigned int len)
{
    FILE *pfile=fopen(filename,"wb");
    if(pfile)
    {
        fwrite(buf,1,len,pfile);
        fclose(pfile);
    }
    else
    {
        return false;
    }
    return true;
}
//�ж��Ƿ�ȫ����ĸ
bool AllIsAlpha(const char* p)
{
	int len=strlen(p);
	int i=0;
	for(;i<len;i++)
	{
	//	if(!isalpha(p[i]))
		//if(p[i]<'A'||(p[i]>'Z'&&p[i]<'a')||p[i]>'z')
		//	break;
		if(p[i]<'0'||(p[i]>'9'&&p[i]<'A')||(p[i]>'Z'&&p[i]<'a')||p[i]>'z')
			break;
	}
	if(i<len)
	{
		return false;
	}
	return true;

}
//��ȡ�ļ���
bool GetSonFileName(const CString allfilename,CString &filename)
{

		int n=allfilename.ReverseFind('\\');
		if(n==-1)//δ�ҵ�
		{
			filename=allfilename;
		}
		else
		{
			filename=allfilename.Mid(n+1);
		}
	return true;

}
bool GetUnxiFileName(const CString allfilename,CString &filename)
{

		int n=allfilename.ReverseFind('/');
		if(n==-1)//δ�ҵ�
		{
			filename=allfilename;
		}
		else
		{
			filename=allfilename.Mid(n+1);
		}
	return true;

}
//"/"�ָ�
bool GetFilePath(const CString allfilename,CString &filepapth)
{

		int n=allfilename.ReverseFind('/');
		if(n==-1)//δ�ҵ�
		{
			return false;
		}
		else
		{
			filepapth=allfilename.Left(n);
		}
	return true;

}
//"/"�ָ�
bool GetFilePathWin(const CString allfilename,CString &filepapth)
{

		int n=allfilename.ReverseFind('\\');
		if(n==-1)//δ�ҵ�
		{
			return false;
		}
		else
		{
			filepapth=allfilename.Left(n);
		}
	return true;

}
//utf-8תunicode
wchar_t *Utf_8ToUnicode(char* szU8) 
 {
   //UTF8 to Unicode
    //��������ֱ�Ӹ��ƹ���������룬��������ʱ�ᱨ���ʲ���16������ʽ
     
    //Ԥת�����õ�����ռ�Ĵ�С
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
    //����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
    wchar_t* wszString = new wchar_t[wcsLen + 2];
    //ת��
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
    //������'\0'
    wszString[wcsLen] = '\0';
    return wszString;
 }
//unicodeתutf-8
char* UnicodeToUTF_8First(CString str)
{   
	int u8Len =WideCharToMultiByte(CP_UTF8, NULL,CStringW(str),str.GetLength(), NULL, 0, NULL, NULL);
    char* szU8 = new  char[u8Len + 1];
    WideCharToMultiByte(CP_UTF8, NULL, CStringW(str), str.GetLength(), szU8, u8Len, NULL, NULL);
    szU8[u8Len] = '\0';
    return szU8;  
}
//UTF-8��GB2312��ת��
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

//GB2312��UTF-8��ת��
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

/*
*	�ж��ַ�����ָ���ַ����Ƿ�Ϊutf8
*	����
*		const char *str: Ҫ�����ַ���
*		long length: Ҫ�����ַ����ĳ���
*		int nWords: Ҫ��������ַ���
*/
bool IsWordsUTF8(const char *str, long length, int nWords)
{
	int i;
	int nBytes=0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	unsigned char chr;
	BOOL bAllAscii=TRUE; //���ȫ������ASCII, ˵������UTF-8
	
	if ( -1 == nWords)
	{
		nWords = (int)length;
	}

	for(i=0;i<length;i++)
	{
		if (0 >= nWords)
		{
			return !bAllAscii;
		}		

		chr= *(str+i);
		if( (chr&0x80) == 0 ) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx;����ASCII����������λΪ1
			nWords--;
		else
			bAllAscii= FALSE;

		if(nBytes==0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
		{
			if(chr>=0x80)
			{
				nWords--;
				
				if(chr>=0xFC&&chr<=0xFD)//1111 1100		1111 1101
					nBytes=6;
				else if(chr>=0xF8)//1111 1000
					nBytes=5;
				else if(chr>=0xF0)//1111 0000
					nBytes=4;
				else if(chr>=0xE0)//1110 0000
					nBytes=3;
				else if(chr>=0xC0)//1100 0000
					nBytes=2;
				else
				{
					return FALSE;
				}

				nBytes--;
			}
		}
		else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return FALSE;
			}

			nBytes--;
		}
	}
	if( nBytes > 0 ) //Υ������
	{
		return FALSE;
	}
	if( bAllAscii ) //���ȫ������ASCII, ˵������UTF-8
	{
		return FALSE;
	}
	return TRUE;
}