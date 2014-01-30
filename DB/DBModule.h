#ifndef DBMODULE_H
#define DBMODULE_H

#include "head.h"
#include "MusicListSaveFormatEntity.h"

class DBModule : public QObject
{
	Q_OBJECT

public:
	DBModule();
	~DBModule();

public:
	static void initDB();									//��ʼ�����ݿ�
	static bool createConnection();							//�������ݿ�
	static void createTable();								//������
	static bool readFromDb(QStringList &list);				//�����ݿ��ж���������ʾ���������б���
	static bool readFromDb2(vector<MusicListSaveFormatEntity> &vec);	//�����ݿ��ж���������ʾ��m3u�ļ���
	static bool clearDB();									//������ݿ�
	static bool delLine(const QString &title);				//ɾ��һ����¼
	//����һ�м�¼
	static bool insertLine(const QString &title, const QString &artist, const QString &time, const QString &filePath);
};

#endif //DBMODULE_H