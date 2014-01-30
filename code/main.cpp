#include "stdafx.h"
#include "widget.h"
#include "static.h"
#include <QApplication>
#include <qtsingleapplication.h>

//����֧��
void initqt_zh_CN()
{
    //���ñ����ʽ
#ifdef _WIN32_WINNT
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#else
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
#endif
    QTranslator *translator = new QTranslator();
    translator ->load(QString(":/qt_zh_CN/qt_zh_CN"), QCoreApplication::applicationDirPath(), NULL, NULL);
    qApp ->installTranslator(translator);
}

//�ļ�����
//void connectFileToProgram()
//{
//    QSettings *ukenvFileReg = new QSettings("HKEY_CLASSES_ROOT\\.{fileType}", QSettings::NativeFormat);   //

//    //�ж�UKEnv�����Ƿ�����ע����У�����������ȷ�Ĵ򿪷�ʽ������򿪷�ʽ����û����д��
//    QString currentValue = ukenvFileReg->value("Default").toString();

//    if (currentValue.isEmpty() || currentValue != "{Program_Type}")
//    {
//        //QMessageBox::information(NULL,"information","Unregistered or Uncorrect");
//        ukenvFileReg->setValue("Default", "{Program_Type}");
//        QSettings *ukenvIconReg = new QSettings("HKEY_CLASSES_ROOT\\.{fileType}\\DefaultIcon", QSettings::NativeFormat);
//        ukenvIconReg->setValue("Default", QString(qApp->argv()[0]) + ",0");   //����ǰ���еĳ���ͼ������
//        delete ukenvIconReg;

//        QSettings *ukenvProgramReg = new QSettings("HKEY_CLASSES_ROOT\\{Program_Type}\\shell\\open\\command", QSettings::NativeFormat);
//        ukenvProgramReg->setValue("Default", QString(qApp->argv()[0]) + " %1"); //����ǰ���еĳ���·������
//        delete ukenvProgramReg;
//    }
//    delete ukenvFileReg;
//}

int main(int argc, char *argv[])
{
	QtSingleApplication app(argc, argv);
	//QApplication app(argc, argv);
	if (app.isRunning())
	{
		//qDebug() << "�����Ѿ�������";
		return EXIT_SUCCESS;
	}
    app.setApplicationName("CZPlayer");
	
    initqt_zh_CN();//����֧��
    //connectFileToProgram();

    //��������qss��ʽ��
    QApplication::setStyle(new QPlastiqueStyle);
    QFile file(":/qss/musicPlayer.qss");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());
    file.close();

    //��ʾ������
    Widget w;
    w.show();

    return app.exec();
}
