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
	static void initDB();									//初始化数据库
	static bool createConnection();							//连接数据库
	static void createTable();								//创建表
	static bool readFromDb(QStringList &list);				//从数据库中读出数据显示到音乐里列表中
	static bool readFromDb2(vector<MusicListSaveFormatEntity> &vec);	//从数据库中读出数据显示到m3u文件中
	static bool clearDB();									//清空数据库
	static bool delLine(const QString &title);				//删除一条记录
	//插入一行记录
	static bool insertLine(const QString &title, const QString &artist, const QString &time, const QString &filePath);
};

#endif //DBMODULE_H