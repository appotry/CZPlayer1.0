#include "stdafx.h"
#include "ClientBaseOperating.h"
#include "static.h"
#include "czplayerconfig.h"
#include "generalsettingspage.h"
#include "downloadsettingspage.h"
#include "lrcsettingspage.h"
#include "hotkeyssettingspage.h"
#include "proxysettingspage.h"
#include "lrcdemo.h"
#include "musiclrc.h"
#include "globalhotkey.h"

ClientBaseOperating::ClientBaseOperating()
{

}

ClientBaseOperating::~ClientBaseOperating()
{

}

//�ж������ļ��Ƿ����
void ClientBaseOperating::touchConfigFile()
{
	if (!QFile::exists(CZPlayerConfigName))
	{
		CZPlayerConfig::setValue("MUSICDIR_WIN", "E:\\CZPlayer");
		CZPlayerConfig::setValue("MUSICDIR_X11", "/CZPlayer");
		CZPlayerConfig::setValue("ALBUMDIR_WIN", "E:\\CZPlayer\\albumImages");
		CZPlayerConfig::setValue("ALBUMDIR_X11", "/CZPlayer/albumImages");
		CZPlayerConfig::setValue("LRCDIR_WIN", "E:\\CZPlayer\\lrc");
		CZPlayerConfig::setValue("LRCDIR_X11", "/CZPlayer/lrc");
		CZPlayerConfig::setValue("FONT", "΢���ź�");
		CZPlayerConfig::setValue("DEFAULTFONT", "΢���ź�");
		CZPlayerConfig::setValue("FONTSIZE", "30");
		CZPlayerConfig::setValue("FONTTYPE", "����");
		CZPlayerConfig::setValue("DEFAULTFONTTYPE", "����");
		CZPlayerConfig::setValue("NOPLAYSHANG", "14 179 255");
		CZPlayerConfig::setValue("NOPLAYZHONG", "114 232 255");
		CZPlayerConfig::setValue("NOPLAYXIA", "14 179 255");
		CZPlayerConfig::setValue("PLAYEDSHANG", "222 54 4");
		CZPlayerConfig::setValue("PLAYEDZHONG", "255 72 16");
		CZPlayerConfig::setValue("PLAYEDXIA", "222 54 4");
		CZPlayerConfig::setValue("YUSHECOLOR", "������");
		CZPlayerConfig::setValue("SHADOW", "255");
		CZPlayerConfig::setValue("CONFIGOPTIONS", "Ctrl+Alt+F1");
		CZPlayerConfig::setValue("SHOWHIDEMAINWIDGET", "Ctrl+Alt+W");
		CZPlayerConfig::setValue("MINMAINWIDGET", "Ctrl+Alt+M");
		CZPlayerConfig::setValue("OPENMUSICFILE", "Ctrl+Alt+O");
		CZPlayerConfig::setValue("SHOWHIDEDOWNLOAD", "Ctrl+Alt+D");
		CZPlayerConfig::setValue("SHOWHIDEMUSICLIST", "Ctrl+Alt+L");
		CZPlayerConfig::setValue("SHOWHIDELRC", "Ctrl+Alt+R");
		CZPlayerConfig::setValue("PLAYPAUSE", "Alt+Space");	
		CZPlayerConfig::setValue("LAST", "Alt+Left");
		CZPlayerConfig::setValue("NEXT", "Alt+Right");
	}
}

//����һ�����ظ���Ŀ¼
void ClientBaseOperating::mkdirDownloadDir()
{
	QDir CZPlayer;
	//��ȡ�����ļ�//����һ���ļ������洢���صĸ���
#ifdef _WIN32_WINNT
	QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
	bool exist = CZPlayer.exists(WINPATH);
	if (!exist)
	{
		CZPlayer.mkdir(WINPATH);
	}
#else
	QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("DOWNLOADDIR_X11").toString();
	bool exist = CZPlayer.exists(X11PATH);
	if (!exist)
	{
		CZPlayer.mkdir(X11PATH);
	}
#endif
}

//����һ���ļ������洢���ص�ר��
void ClientBaseOperating::mkdirAlbumDir()
{
	QDir CZPlayer;
#ifdef _WIN32_WINNT
	QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
	bool exist = CZPlayer.exists(albumImagesName);
	if (!exist)
	{
		CZPlayer.mkdir(albumImagesName);
	}
#else
	QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
	bool exist = CZPlayer.exists(albumImagesName);
	if (!exist)
	{
		CZPlayer.mkdir(albumImagesName);
	}
#endif
}

//����һ�����ظ��Ŀ¼
void ClientBaseOperating::mkdirLrcDir()
{
	QDir CZPlayer;
#ifdef _WIN32_WINNT
	QString lrcDirName = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
	bool exist = CZPlayer.exists(lrcDirName);
	if (!exist)
	{
		CZPlayer.mkdir(lrcDirName);
	}
#else
	QString lrcDirName = QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
	bool exist = CZPlayer.exists(lrcDirName);
	if (!exist)
	{
		CZPlayer.mkdir(lrcDirName);
	}
#endif
}

//��ʼ�����öԻ���
void ClientBaseOperating::initConfigDialog()
{
	ClientBaseOperating::initDownloadConfig();//��ʼ������ҳ
	ClientBaseOperating::initLrcConfig();//��ʼ�����ҳ
	ClientBaseOperating::initHotKeyConfig();//��ʼ��ȫ���ȼ�ҳ
}

//��ʼ������ҳ
void ClientBaseOperating::initDownloadConfig()
{
#ifdef _WIN32_WINNT
	DownloadSettingsPage::setMusicDir(CZPlayerConfig::getValue("MUSICDIR_WIN").toString());
	DownloadSettingsPage::setAlbumDir(CZPlayerConfig::getValue("ALBUMDIR_WIN").toString());
	DownloadSettingsPage::setLrcDir(CZPlayerConfig::getValue("LRCDIR_WIN").toString());
#else
	DownloadSettingsPage::setMusicDir(QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_WIN").toString());
	DownloadSettingsPage::setAlbumDir(QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_WIN").toString());
	DownloadSettingsPage::setLrcDir(CZPlayerConfig::getValue("LRCDIR_X11").toString());
#endif
}

//��ʼ�����ҳ
void ClientBaseOperating::initLrcConfig()
{
	//�������ļ������������
	LrcSettingsPage::setFontCombo(CZPlayerConfig::getValue("FONT").toString());
	LrcSettingsPage::setFontSizeCombo(CZPlayerConfig::getValue("FONTSIZE").toString());
	LrcSettingsPage::setFontTypeCombo(CZPlayerConfig::getValue("FONTTYPE").toString());
	LrcSettingsPage::setNoPlayEdit(CZPlayerConfig::getValue("NOPLAYSHANG").toString());
	LrcSettingsPage::setNoPlayEdit2(CZPlayerConfig::getValue("NOPLAYZHONG").toString());
	LrcSettingsPage::setNoPlayEdit3(CZPlayerConfig::getValue("NOPLAYXIA").toString());
	LrcSettingsPage::setPlayedEdit(CZPlayerConfig::getValue("PLAYEDSHANG").toString());
	LrcSettingsPage::setPlayedEdit2(CZPlayerConfig::getValue("PLAYEDZHONG").toString());
	LrcSettingsPage::setPlayedEdit3(CZPlayerConfig::getValue("PLAYEDXIA").toString());
	LrcSettingsPage::setYuSheColor(CZPlayerConfig::getValue("YUSHECOLOR").toString());
	LrcSettingsPage::setShadowSlider(CZPlayerConfig::getValue("SHADOW").toInt());

	LrcDemo::setFontYS(CZPlayerConfig::getValue("FONT").toString());
	LrcDemo::setFontSize(CZPlayerConfig::getValue("FONTSIZE").toString());
	LrcDemo::setFontType(CZPlayerConfig::getValue("FONTTYPE").toString());
	LrcDemo::setLinearGradient(CZPlayerConfig::getValue("NOPLAYSHANG").toString());
	LrcDemo::setLinearGradient2(CZPlayerConfig::getValue("NOPLAYZHONG").toString());
	LrcDemo::setLinearGradient3(CZPlayerConfig::getValue("NOPLAYXIA").toString());
	LrcDemo::setMaskLinearGradient(CZPlayerConfig::getValue("PLAYEDSHANG").toString());
	LrcDemo::setMaskLinearGradient2(CZPlayerConfig::getValue("PLAYEDZHONG").toString());
	LrcDemo::setMaskLinearGradient3(CZPlayerConfig::getValue("PLAYEDXIA").toString());
}

//��ʼ��ȫ���ȼ�ҳ
void ClientBaseOperating::initHotKeyConfig()
{
	//�������ļ�����ȫ���ȼ�
	HotKeysSettingsPage::setSettingsHotKey(CZPlayerConfig::getValue("CONFIGOPTIONS").toString());
	HotKeysSettingsPage::setMinToTrayHotKey(CZPlayerConfig::getValue("SHOWHIDEMAINWIDGET").toString());
	HotKeysSettingsPage::setOpenMinWidgetHotKey(CZPlayerConfig::getValue("MINMAINWIDGET").toString());
	HotKeysSettingsPage::setOpenMusicFileHotKey(CZPlayerConfig::getValue("OPENMUSICFILE").toString());
	HotKeysSettingsPage::setOpenDownloadWidgetHotKey(CZPlayerConfig::getValue("SHOWHIDEDOWNLOAD").toString());
	HotKeysSettingsPage::setOpenListWidgetHotKey(CZPlayerConfig::getValue("SHOWHIDEMUSICLIST").toString());
	HotKeysSettingsPage::setOpenLrcHotKey(CZPlayerConfig::getValue("SHOWHIDELRC").toString());
	HotKeysSettingsPage::setPauseHotKey(CZPlayerConfig::getValue("PLAYPAUSE").toString());
	HotKeysSettingsPage::setLastHotKey(CZPlayerConfig::getValue("LAST").toString());
	HotKeysSettingsPage::setNextHotKey(CZPlayerConfig::getValue("NEXT").toString());
}

//����������ʱ�����ø������
void ClientBaseOperating::initLrcPropery()
{
	//���ĸ������
	MusicLrc::setFontYS(CZPlayerConfig::getValue("FONT").toString());
	MusicLrc::setFontSize(CZPlayerConfig::getValue("FONTSIZE").toString());
	MusicLrc::setFontType(CZPlayerConfig::getValue("FONTTYPE").toString());
	MusicLrc::setLinearGradient(CZPlayerConfig::getValue("NOPLAYSHANG").toString(),
								CZPlayerConfig::getValue("NOPLAYZHONG").toString(),
								CZPlayerConfig::getValue("NOPLAYXIA").toString());
	MusicLrc::setMaskLinearGradient(CZPlayerConfig::getValue("PLAYEDSHANG").toString(),
									CZPlayerConfig::getValue("PLAYEDZHONG").toString(),
									CZPlayerConfig::getValue("PLAYEDXIA").toString());
	MusicLrc::setLrcShadow(CZPlayerConfig::getValue("SHADOW").toInt());
}

//����������ʱ,����ȫ���ȼ�
void ClientBaseOperating::initHotKeyPropery()
{
	GlobalHotKey::setConfigOptions(CZPlayerConfig::getValue("CONFIGOPTIONS").toString());
	GlobalHotKey::setShowHideMainWidget(CZPlayerConfig::getValue("SHOWHIDEMAINWIDGET").toString());
	GlobalHotKey::setMinMainWidget(CZPlayerConfig::getValue("MINMAINWIDGET").toString());
	GlobalHotKey::setOpenMusicfile(CZPlayerConfig::getValue("OPENMUSICFILE").toString());
	GlobalHotKey::setShowDownload(CZPlayerConfig::getValue("SHOWHIDEDOWNLOAD").toString());
	GlobalHotKey::setShowMusicList(CZPlayerConfig::getValue("SHOWHIDEMUSICLIST").toString());
	GlobalHotKey::setShowLrc(CZPlayerConfig::getValue("SHOWHIDELRC").toString());
	GlobalHotKey::setPlayPause(CZPlayerConfig::getValue("PLAYPAUSE").toString());
	GlobalHotKey::setLast(CZPlayerConfig::getValue("LAST").toString());
	GlobalHotKey::setNext(CZPlayerConfig::getValue("NEXT").toString());
}
