#include "stdafx.h"
#include "myftp.h"
#include "PublicFun.h"
#include <sys/stat.h> 
#include <io.h>
#include "BaseLog.h"
myftp::myftp(void)
{
	dealnum=0;
	curl=NULL;
	m_dealfilenum=0;
	memset(username,0,64);
	memset(password,0,64);
	memset(ip,0,24);
	memset(server_url,0,256);
	memset(login_info,0,256);
	m_is_del_path=false;
}


myftp::~myftp(void)
{
}
bool myftp::init_curl()
{
// 	if(curl==NULL)
// 	{
		curl = curl_easy_init();	
// 	}
	return true;

}
void myftp::ftp_disconnect_server()
{
	if( curl == NULL )
		return;
	curl_easy_cleanup(curl);
// 	curl =NULL;
}
struct FtpFile
{
    const char *filename;
    FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream)
    {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");
        if(!out->stream)
        return -1; /* failure, can't open file to write */
    }

    return fwrite(buffer, size, nmemb, out->stream);
}

int myftp::sftp_Get_dir(const char* localfile)
{
	FILE *listfile = NULL;
    listfile=fopen(localfile ,"wb");
    if(listfile==NULL)
    {
        return -1;
    }

	CURLcode res;
    struct curl_slist *headerlist = NULL;

    char szCmd_1[1024] = {0}; /* SFTP������������ */
    strcat(szCmd_1, "cd ~ \"\0");
    headerlist = curl_slist_append(headerlist, szCmd_1);

    curl_easy_reset(curl);
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
    curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
    curl_easy_setopt(curl, CURLOPT_URL, server_url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);  /// ����ע�⣺���һ�������٣�����
   // curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"pwd");
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, listfile);

    res = curl_easy_perform(curl);
    curl_slist_free_all (headerlist);
	 fclose(listfile);
    if( res == CURLE_OK )
    {
        return 0;
    }
    else
    {
        return -1;
    }

}

bool myftp::iniftp(int ftptype,const char* user,const char* pass,const char* ftpip,int ftpport,int mode)
{
	m_mode=mode;
	if(ftptype==0)
	{
		WriteLog( "libcurl set PORT mode *** ( ����ģʽ )" );
		m_use_port_mode=true;
	}
	
	else
	{
		 WriteLog( "libcurl set PASV mode *** ( ����ģʽ )" );
		 m_use_port_mode=false;
	}


	 m_is_del_path=false;


	dealtype=ftptype;
	port=ftpport;
	strcpy(username,user);
	strcpy(password,pass);
	strcpy(ip,ftpip);
	sprintf(server_url, "ftp://%s:%d", ip, port);

	if(mode==0)
    {
        sprintf(server_url, "ftp://%s:%d", ip, port);
    }
    else if(mode==1)
    {
        sprintf(server_url, "sftp://%s:%d", ip, port);
    }
	else
	{
		 sprintf(server_url, "ftp://%s:%d", ip, port);
	}
	sprintf(login_info, "%s:%s", username, password);
	printf("login_info:%s\r\n",login_info);
	return true;
}
bool myftp::ftp_connect_server()
{
	CURLcode res;
	/* ���curl��� */
	curl = curl_easy_init();
	if(curl==NULL||login_info==NULL||server_url ==NULL)
	{
		return false;
	}


	curl_easy_reset(curl);
	/* ��ʾ��ϸ������Ϣ */
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        /* �����û��������� */
	curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
	/* ���÷�������ַ */
	curl_easy_setopt(curl, CURLOPT_URL, server_url);
	res = curl_easy_perform(curl);

	if( res==0 )
	{
		WriteLog("�������������ϴ�)����ftp�������ɹ�������������");
		return true;
	}
	else
	{
		WriteLog("�������������ϴ�)����ftp������ʧ�ܣ�����������");
		return false;
	}
}
int myftp::ftp_upload_rmfile(const char *remotepath, const char *localpath)
{
	WriteLog("ftp_upload_rmfile[%s->%s]", localpath,remotepath);
	CURLcode res;
	
	if(curl==NULL||server_url ==NULL||remotepath==NULL||localpath==NULL)
	{
		return -1;
	}

	int fd;
	FILE * local_file ;
// 	double total_time, size_upload;
	char remote_path[1024];
	memset(remote_path,0,1024);
	struct stat file_info;
	/* ƴ���������ļ��� */
	if(m_mode!=1)
	{
		sprintf(remote_path, "%s/%s.tmp", server_url, remotepath);
	}
	else
	{
		sprintf(remote_path, "%s/~/%s.tmp", server_url, remotepath);
	}
	char remote_path2tmp[1024];
	memset(remote_path2tmp,0,1024);
	sprintf(remote_path2tmp, "/%s.tmp",  remotepath);

	string curltmpfile="";
	CString allfilename=CString(remote_path2tmp);
	CString curlfilename;
	if( GetUnxiFileName( allfilename,curlfilename))
	{
		CString curlfilepapth;
		if( GetFilePath(allfilename,curlfilepapth))
		{
			char curltmpfile1[1024];
			memset(curltmpfile1,0,1024);
			sprintf(curltmpfile1,"%s.in.%s.",curlfilepapth,curlfilename);
			curltmpfile=curltmpfile1;

		}

	}

	
		/* ����ϴ��ļ��Ĵ�С */
	fd = open(localpath, 0);
	if( fd == -1 )
	{
 		WriteLog("Open %s failed!\n", localpath);
		return -2;
	}
	fstat(fd, &file_info);
	close(fd);
	if(file_info.st_size>1024*1024*500)
	{
		WriteLog("upload a big file [%d]  , this file is too big !",file_info.st_size);
		remove(localpath);
		return 0;
	}

	/* ��Ҫ�ϴ����ļ� */
	local_file = fopen(localpath, "rb");
	if( local_file == NULL )
	{
 		WriteLog("Open %s failed!\n", localpath);
		return -2;
	}
	/* �����ϴ���ʶCURLOPT_UPLOAD(0:���� 1���ϴ�) */
	curl_easy_reset(curl);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1) ;

	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
	curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
//	printf("login_info:%s\r\n",login_info);
	/* ����Ŀ���ļ�·�� */
	curl_easy_setopt(curl, CURLOPT_URL, remote_path);
	
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10*60);
	/* ����Դ�ļ�·�� */
	curl_easy_setopt(curl, CURLOPT_INFILE, local_file);
	
	/* �����ϴ���С */
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_info.st_size);
	
//	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, file_info.st_size);//�ϵ�����
	//	curl_easy_setopt(curl, CURLOPT_RESUME_FROM, file_info.st_size);//�ϵ�����
	if(!m_use_port_mode)
	curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 0);//ֱ�ӱ���
	else
	{
	curl_easy_setopt(curl, CURLOPT_FTPPORT, "_");//����ģʽ
		curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, 0);//����ģʽ
	}


	/* �Զ������������ϲ����ڵ���;Ŀ¼ */
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1L); //alarm
	/* ��ʼ�ϴ� */
	res = curl_easy_perform(curl);
	
	/* ��ȡ�ϴ��ٶȼ�ʱ�� */
// 	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
// 	curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &size_upload);
	
	fclose(local_file);
	
	if( res == CURLE_OK )
	{
// 		dealfilenum++;
			 WriteLog("upload a file [%s] is success",localpath);
        int retflg=0;
        if(m_mode==1)
        {
            retflg=sftp_rename_item(remote_path2tmp, remotepath);
        }
        else
        {
            retflg=ftp_rename_item(remote_path2tmp, remotepath);
        }
        if(retflg==0)
        {
          //  g_StatDataNum.Insert();
            WriteLog("rename a file [%s] is success",remotepath);
            unlink(localpath);
            return 0;
        }
        else
        {
            WriteLog("false:rename is failed [%s] !",remotepath);
            return -1;
        }
	}
	else
	{
		
		if(ftp_delete_file(remote_path2tmp)==0)
		{
			WriteLog("false: upload file [%s] is failed and delete!",localpath);
		}
		else
		{
			WriteLog("false: upload file [%s] is failed and delete error!",localpath);
	
			if(ftp_delete_file(curltmpfile.c_str())==0)
			{

			}
			else
			{
				WriteLog("false: file [%s]  delete error!",curltmpfile.c_str());
			}
			

		}
		return -1;
	}	
}
/**
 *@brief    sftp_rename_item - ������SFTP�������ϵ��ļ����Ŀ¼
 * @par     curlhandle(i): CURL���
 * @par     oldname(i)   : ������
 * @par     newname(i)   : ������
 * @return(o)    : �����ɹ�����0��ʧ�ܷ���-1
 */
 
 int myftp::sftp_rename_item(const char *oldname, const char *newname)
{
	string oldnamestr="./";
	oldnamestr+=oldname;

	string newnamestr="./";
	newnamestr+=newname;

    CURLcode res;
    struct curl_slist *headerlist = NULL;

    
    char remote_path[1024];
	memset(remote_path,0,1024);

	/* ƴ���������ļ��� */
	sprintf(remote_path, "%s/%s", server_url, "~");

    
    if( curl == NULL || oldname == NULL || newname == NULL )
    {
        return -1;
    }
    char szCmd_1[1024] = {0}; /* SFTP������������ */
    strcat(szCmd_1, "rename \"\0");
 
    strcat(szCmd_1,oldnamestr.c_str());
    strcat(szCmd_1,"\" \"\0");
    strcat(szCmd_1,newnamestr.c_str());
    strcat(szCmd_1,"\"\"\0");
   // WriteLog("sftp_rename_item[%s->%s]", oldnamestr.c_str(),newnamestr.c_str());
    headerlist = curl_slist_append(headerlist, szCmd_1);

    curl_easy_reset(curl);

    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
    }
	//WriteLog("server_url��[%s]",remote_path);
    curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
    curl_easy_setopt(curl, CURLOPT_URL, remote_path);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);  /// ����ע�⣺���һ�������٣�����
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

    if ( m_use_port_mode )
    {
        // printf( "����ΪPORT MODE****************\n" );
        curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, 0);//����ģʽ
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 0);//����ģʽ
    }
	
    res = curl_easy_perform(curl);
    curl_slist_free_all (headerlist);

    if( res == CURLE_OK )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
//��ȡSFTPĿ¼
string myftp::sftp_pwd_item()
{
	 
	
}





/**
 *@brief    ftp_rename_item - ������FTP�������ϵ��ļ����Ŀ¼
 * @par     curlhandle(i): CURL���
 * @par     oldname(i)   : ������
 * @par     newname(i)   : ������
 * @return(o)    : �����ɹ�����0��ʧ�ܷ���-1 
*/
int myftp::ftp_rename_item(const char *oldname, const char *newname)
{
	CURLcode res;
	struct curl_slist *headerlist = NULL;
	
	char szCmd_1[256] = "RNFR "; /* FTP������������ */
	char szCmd_2[256] = "RNTO ";
	
	if( curl == NULL || oldname == NULL || newname == NULL )
		return -1;
	
	strcat(szCmd_1, oldname);
	strcat(szCmd_2, newname);
	
	headerlist = curl_slist_append(headerlist, szCmd_1);
	headerlist = curl_slist_append(headerlist, szCmd_2);
	curl_easy_reset(curl);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
	curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
	curl_easy_setopt(curl, CURLOPT_URL, server_url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);	/// ����ע�⣺���һ�������٣�����
	curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

	
    if ( m_use_port_mode )
    {
        // printf( "����ΪPORT MODE****************\n" );
        curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, 0);//����ģʽ
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 0);//����ģʽ
    }
	

	res = curl_easy_perform(curl);
	curl_slist_free_all (headerlist);
	
	if( res == CURLE_OK )
		return 0;
	else
		return -1;
}

/**
 *@brief ftp_delete_file - ɾ��FTP�������ϵ�ĳ���ļ�
 *@par curhandle(i): CURL���
 *@par filename(i) : Ҫɾ���������ļ���
 *@return(i)   : ɾ���ɹ�����0��ʧ�ܷ���-1 
*/
int myftp::ftp_delete_file(const char *filename)
{



	CURLcode res;
	char szCmd[256] = "DELE ";  /* FTP��ɾ���ļ����� */

	struct curl_slist *headerlist = NULL;
	
	if( curl == NULL || filename == NULL )
		return -1;
	
	strcat(szCmd, filename);
	curl_easy_reset(curl);
	curl_easy_setopt(curl, CURLOPT_VERBOSE,1);
	curl_easy_setopt(curl, CURLOPT_URL,server_url);

	headerlist = curl_slist_append(headerlist, szCmd);
	curl_easy_setopt(curl, CURLOPT_USERPWD, login_info);
	curl_easy_setopt(curl, CURLOPT_QUOTE, headerlist);
	if ( m_use_port_mode )
    {
        // printf( "����ΪPORT MODE****************\n" );
        curl_easy_setopt(curl, CURLOPT_FTPPORT, "-");
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, 0);//����ģʽ
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 0);//����ģʽ
    }

	res = curl_easy_perform(curl);
	curl_slist_free_all (headerlist);
	if( res == CURLE_OK )
	{	
		return 0;
	}
	else
	{
		return -1;
	}
}
/**
 * @brief ftp_upmove_dir - �ϴ�����ĳ��Ŀ¼��FTP��������ɾ��ԴĿ¼�е��ļ�������ԴĿ¼�ṹ��
 * @par  curlhandle(i): CRUL���
 * @par  remotepath(i): Զ��Ŀ¼·��
 * @par  localpath(i) : ����Ŀ¼·��
 * @return(o)    : �ɹ�����0��ʧ�ܷ���-1
 */
int myftp::ftp_upmove_dir(const char *remotepath, const char *localpath)
{

	CFileFind filefind;                                         //����CFileFind���ͱ���
    CString strWildpath ;     //�����ļ����г���
	strWildpath.Format("%s\\*.*",localpath);
	if(filefind.FindFile(strWildpath, 0))	                   //��ʼ�����ļ�
    {
        BOOL bRet = TRUE;
        while(bRet)
        {
            bRet = filefind.FindNextFile();                 //ö��һ���ļ�
            if(filefind.IsDots())                                 //�����. �� .. ����һ��
                continue;

			CString fileName= filefind.GetFileName();

            if(filefind.IsDirectory())//Ŀ¼�ϱ�
            {

				char remotepathtmp[512];
				int len=0;
				if(!m_is_del_path)
				{
					len=sprintf(remotepathtmp,"%s/%s",remotepath,fileName);
				}
				else
				{
					len=sprintf(remotepathtmp,"%s",remotepath);
				}
				remotepathtmp[len]=0;


				char localpathtmp[512];
				len=sprintf(localpathtmp,"%s\\%s",localpath,fileName);
				localpathtmp[len]=0;


				if(ftp_upmove_dir(remotepathtmp, localpathtmp)!=0)
				{
					filefind.Close();
					return -1;
				}
			}
			else//�ļ��ϱ�
			{
				
				int pos=fileName.ReverseFind('.');
				CString strLastName=fileName.Right(fileName.GetLength()-pos-1);
				if(strLastName.Compare("tmp")==0||strLastName.Compare("temp")==0)
				{
					continue;
				}

				char localfilename[512];
				int len=sprintf(localfilename,"%s\\%s",localpath,fileName);
				localfilename[len]=0;
				char remotefilename[512];
				len=sprintf(remotefilename,"%s/%s",remotepath,fileName);
				remotefilename[len]=0;
				if(ftp_upload_rmfile(remotefilename,localfilename)!=0)
				{
					filefind.Close();
					return -1;
				}
			}
		}
		filefind.Close();
	}
	else
	{
			WriteLog("false: ftp_upmove_dir open die error [%s]",localpath);
	}
	return 0;
}