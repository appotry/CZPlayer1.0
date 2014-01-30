#ifndef CLIENTBASEOPERATING_H
#define CLIENTBASEOPERATING_H

#include "head.h"

class ClientBaseOperating : public QObject
{
	Q_OBJECT

public:
	ClientBaseOperating();
	~ClientBaseOperating();

public:
	static void touchConfigFile();				//�ж������ļ��Ƿ����
	static void mkdirDownloadDir();				//����һ�����ظ���Ŀ¼
	static void mkdirAlbumDir();				//����һ���ļ������洢���ص�ר��
	static void mkdirLrcDir();					//����һ�����ظ��Ŀ¼

	static void initConfigDialog();				//��ʼ�����öԻ���
	static void initDownloadConfig();			//��ʼ������ҳ
	static void initLrcConfig();				//��ʼ�����ҳ
	static void initHotKeyConfig();				//��ʼ��ȫ���ȼ�ҳ

	static void initLrcPropery();				//���ø������
	static void initHotKeyPropery();			//����ȫ���ȼ�
};

#endif // CLIENTBASEOPERATING_H