#include "stdafx.h"
#include "DBModule.h"

DBModule::DBModule()
{

}

DBModule::~DBModule()
{

}

//��ʼ�����ݿ�
void DBModule::initDB()
{
	//�ж����ݿ��ļ��Ƿ����
	bool create = !QFile::exists("music.sqlite");

	if (!DBModule::createConnection())
	{
		QMessageBox::warning(0, tr("��ʾ"), tr("�������ݿ����!"));
		return;
	}
	if (create)
	{
		DBModule::createTable();
	}
}

//�������ݿ�
bool DBModule::createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("music.sqlite");//�����ݿ��ļ������ڣ����Զ�����һ��

	if (!db.open())
	{
		QMessageBox::warning(0, tr("���ݿ�򿪴���!"),
			db.lastError().databaseText());
		return false;
	}
	return true;
}

//���û�д������򴴽�
void DBModule::createTable()
{
	QSqlQuery query;
	query.exec("drop table musicList");

	query.exec("create table musicList (title varchar(30) primary key not null, "
		"artist varchar(20), time varchar(10), filePath varchar(50))");
}

//�����ݿ��ж��������ļ���Ϣ�������ֲ����б�
bool DBModule::readFromDb( QStringList &list )
{
	QSqlQuery query;
	query.exec("select filePath from musicList");

	while (query.next())
	{
		QString filePath = query.value(0).toString();
		list << filePath;
	}

	if (!list.isEmpty())
	{
		return true;
	}

	return false;
}

//�����ݿ��ж���������ʾ��m3u�ļ���
bool DBModule::readFromDb2( vector<MusicListSaveFormatEntity> &vec )
{
	QSqlQuery query;
	query.exec("select title, time, filePath from musicList");

	while (query.next())
	{
		MusicListSaveFormatEntity entity(query.value(0).toString().toStdString(),
									query.value(1).toString().toStdString(),
									query.value(2).toString().toStdString());
		vec.push_back(entity);
	}

	if (!vec.empty())
	{
		return true;
	}

	return false;
}

//������ݿ�
bool DBModule::clearDB()
{
	QSqlQuery query;
	return query.exec("delete from musicList");
}

//ɾ��һ����¼
bool DBModule::delLine( const QString &title )
{
	QSqlQuery query;
	query.prepare("delete from musicList where title = ?");
	query.addBindValue(title);
	return query.exec();
}

//����һ�м�¼
bool DBModule::insertLine( const QString &title, const QString &artist, const QString &time, const QString &filePath )
{
	QSqlQuery query;
	query.prepare("insert into musicList values(?, ?, ?, ?)");
	query.addBindValue(title);
	query.addBindValue(artist);
	query.addBindValue(time);
	query.addBindValue(filePath);
	return query.exec();
}
