// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "afx.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib,"libcurl_imp.lib")
#pragma comment(lib,"libhttpclientD.lib")
#pragma comment(lib,"zlib.lib")
#else
//#pragma comment(lib,"libcurl.lib")
#pragma comment(lib,"libcurl.lib")
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib")
#pragma comment(lib,"libssh2.lib")
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"Crypt32.lib")
//#pragma comment(linker,"/SUBSYSTEM:Windows /ENTRY:mainCRTStartup")
#endif

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
