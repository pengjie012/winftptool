#ifndef HTTPSENDFILE_H_
#define HTTPSENDFILE_H_
#include "PublicFun.h"
bool sendfile(ftpInfo oneftpInfo);
//loglevel   1.  ��ӡ����   2. ��ӡ�ļ�·��    return=filenum  -1 error   dstdir���Ϊ��ʹ��ftpuser���·���� ��Ϊ��Ϊ����·��
int CommSendDir_Custom(char* lpDest, unsigned short sDestPort,char* srcdir,char* dstdir,char* filterfiletype,
char* ftpuser,char* remotedir,int maxfilenum=2000,int loglevel=1,bool sendokdelete=true);
#endif
