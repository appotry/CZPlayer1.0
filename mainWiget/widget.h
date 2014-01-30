#ifndef WIDGET_H
#define WIDGET_H

#include "musiclist.h"
#include "musiclrc.h"
#include "musicdownload.h"
#include "albumthread.h"
#include "lrcthread.h"
#include "HotplugWatcherThread.h"
#include "minmusicwidget.h"
#include "musiclistwidget.h"
#include "configdialog.h"
#include "AboutPanel.h"
#include "globalhotkey.h"
#include "head.h"
//#include <QtDBus/QDBusConnection>

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget();
    ~Widget();

public:
    int getMusicListShow() const;
    void setMusicListShow(int value);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
	//void timerEvent(QTimerEvent *event);

signals:
    void sig_ShowMainWidget();																	//��ʾ������

private slots:
    void slot_UpdateTime(qint64 time);														//����ʱ��͸��
    void slot_OpenMinWidget();
    void slot_OpenFile();																	//�������ļ�
	void slot_AddMusicList();																//��Ӳ����б�
    void slot_OpenMusicList();																//�����ֲ����б�
    void slot_OpenMusicLrc();																//�򿪸��
    void slot_OpenMusicDownload();															//�����ظ�������
    void slot_HideMusicDownload();															//�������ظ�������
    void slot_HideMusicList();																//�������ֲ������б�
    void slot_HideMainWidget();																//����������
    void slot_ShowMusicPanel();																//��ʾ������
    void slot_ConfigCZPlayer();																//����
	void slot_AboutCZPlayer();																//����CZPlayer
    void slot_AboutQt();																	//����Qt
    void slot_SetPaused();																	//��ͣ
    void slot_SetStop();																	//ֹͣ
    void slot_SetPre();																		//��һ��
    void slot_SetNext();																	//��һ��
    void slot_StateChanged(Phonon::State newState, Phonon::State /*oldState*/);				//����״̬�ı�
    void slot_RowChanged(const Phonon::MediaSource &source);								//ѡ�е�ǰ����ý��Դ����
    void slot_TableDoubleClicked(int row, int /*column*/);									//˫��ý���ѡ�в��Ÿ�����
    void slot_ClearSources();																//���Դ
    void slot_DelSelectSource(int row, QString title);										//ɾ��ѡ��Դ�����ݿ���Ӧ����
    void slot_AboutMusicFinish();															//���Ž���ʱ����
    void slot_MetaStateChanged(Phonon::State newState, Phonon::State /*oldState*/);			//ý��Դ�ı�ʱ������µ�Դ
    void slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason);		//ϵͳ���̼����
    void slot_ShowMinSize();																//��С��
    void slot_MinToTray();																	//��С��������
    void slot_ShowLrcLocked();																//��ʾ����Ѿ�������Ϣ
    void slot_ShowLrcUnlocked();															//��ʾ����Ѿ�������Ϣ
    void slot_UnlockLrc();																	//�������
    void slot_CloseAllProgress();															//�˳�����
    void slot_TimeOut();																	//��ʱ��
	void slot_ResolveLrc();																	//�������
	void slot_ShortCutsetDisable();															//����ȫ���ȼ�����/����
    void slot_ConfigOptions();																//ѡ������
    void slot_ShowHideMainWidget();															//��ʾ/����������
    void slot_MinMainWidget();																//����ģʽ/��������
    void slot_OpenMusicfile();																//�������ļ�
    void slot_ShowDownload();																//��ʾ/���������б�
    void slot_ShowMusicList();																//��ʾ/���ظ����б�
    void slot_ShowLrc();																	//��ʾ/����������
    void slot_PlayPause();																	//����/��ͣ
    void slot_Last();																		//��һ��
    void slot_Next();																		//��һ��

	//void slotDeviceAdded(QString str);//����豸�Ĳ���
	//void slotDeviceRemoved(QString str);//����豸�Ĳ���

private:
    void resolveLrc(const QString &sourceFileName);											//�������
    void readFromDb();																		//�����ݿ��ж���������ʾ���������б���
    void updateMap();																		//����map

private:
    //��ʾ������0���رգ�1��������
    int musicListShowFlag;
    int musicDownloadShowFlag;

    QPoint dragPosition;
    QTimer *timer;
	QTimer *shortCutIimer;

    MinMusicWidget *minMusicWidget;							//���㲥�Ž���
    MusicList *playList;									//�����б�
    MusicListWidget *musicListWidget;
    MusicLrc *lrc;											//���
    MusicDownload *musicDownload;							//��������
    ConfigDialog *configDialog;								//���ý���
	AboutPanel *aboutPanel;									//����CZPlayer
    GlobalHotKey *globalHotKey;								//ȫ���ȼ�
	HotplugWatcherThread *hotpugWatcherThread;				//�豸����

    QMap<qint64, QString> lrcMap;
    map<QString, int> musicInfoMap;

    Phonon::MediaObject *mediaObject;						//����ý�����
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *metaInformationResolver;			//Դ��Ϣ����
    Phonon::AudioOutput *metaInformationAudioOutput;
    Phonon::SeekSlider *seekSlider;							//���ſ�����
    Phonon::VolumeSlider *volumeSlider;						//����������
    QList<Phonon::MediaSource> sources;						//���ý����Դ

    QLabel *albumImage;
    QLabel *timeLabel;
    QLabel *InfoLabel;
	QLabel *lrcLabel;

    QAction *closeAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *lastAction;
    QAction *nextAction;
    QAction *minAction;
    QAction *quitAction;
    QAction *unlockLrcAction;
    QAction *openFileAction;
    QAction *openMusicListAction;
    QAction *openMusicDownloadAction;
    QAction *openLrcAction;
    QAction *configAction;
	QAction *aboutAction;
    QAction *aboutQtAction;

    QMenu *trayMenu;										//ϵͳ���̲˵�
    QSystemTrayIcon *trayIcon;								//ϵͳ����ͼ��
    QMovie *movie;

    QPushButton *playButton;
    QPushButton *endButton;
    QPushButton *preButton;
    QPushButton *nextButton;
    QPushButton *openFileButton;
    QPushButton *musicListButton;
    QPushButton *lrcButton;
    QPushButton *showButton;

    QString m_musicName;
    QString m_musicArtist;
    QString m_lrcFileName;
    QString m_title;
    QString m_artist;
    int readyReadDb;

	//QBasicTimer baseTimer;
	//QString lrcText;
	//int step;
};

#endif // WIDGET_H
