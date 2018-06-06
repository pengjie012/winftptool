#include "afx.h"
#include <string>
#include <vector>
#include <map>

using namespace std;
struct ftpInfo{
	CString localpath;
	CString remotepath;
	CString ip;
	int port;
	CString user;
	CString pass;
	int upmode;
	int type;
	//CString Post;
};
extern string g_rootpth;
CString GetCurWorkDir();
int search_direct(const char *dir,std::vector<string>& files);

bool GetFilePathWin(const CString allfilename,CString &filepapth);
void SplitString(const string &src_str, const string &separator,vector<string> &part_strings);
CString GetApiStr(const char* node , const char* key, const char* defaultvalue,const char* inifilepath);
unsigned int GFileSize(string filename);
//��ȡ�ļ�
bool ReadFromFile(const char * filename,char* filebuf, int len);
//�ַ����ָ�
void SplitStoV(char* buf,int len,char* e, std::vector<string>& sVector, bool isdelempty);
string GetForntStr(string str,string sp);
//����Ŀ¼
void TravelFolder(CString strDir,CString fixstr,vector<CString> &filearr);
//д�ļ�
bool WriteToFile(const char * filename,const char * buf,unsigned int len);
//�ж��ļ��Ƿ����
bool FileIsExist(const char* filename);
//�ж��Ƿ�ȫΪ��ĸ
bool AllIsAlpha(const char* p);
//����ļ���
bool GetSonFileName(const CString allfilename,CString &filename);
bool GetUnxiFileName(const CString allfilename,CString &filename);
//utf-8 -> Unicode
wchar_t *Utf_8ToUnicode(char* szU8);
// Unicode -> utf8
char* UnicodeToUTF_8First(CString str);
char* U2G(const char* utf8);
bool IsWordsUTF8(const char *str, long length, int nWords);
CString GetRunPath();
int CreatePath(const char* folder,bool all=true);
bool GetFilePath(const CString allfilename,CString &filepapth);
bool IsDirectory(const char* filename);
int CreatePath(const char* folder,bool all);
 void DeleteDirectories(CString csPath) ;

void copypath(CString srcDir,CString dstDir);