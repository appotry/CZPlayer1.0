#include "stdafx.h"
#include "widget.h"
#include "static.h"
#include "czplayerconfig.h"
#include "ClientBaseOperating.h"
#include "DBModule.h"

/*

�������ݿ⹦��
                                                         -> ��������Ϣ¼�����ݿ�(���������·��¼��)
1.���г��� -> �������ļ� -> ��ȫ�������ļ����ص�ý��Դ
                                                         -> ��������Ϣ��ʾ�����ֱ��

                                       -> �������ļ���ʾ�����ֱ��
2.���г��� -> �������ļ������ݿ��ж���
                                       -> ���������ļ�·���������� -> �������ļ����ص�ý��Դ

*/

/*
CTRL_CLOSE_EVENT, CTRL_LOGOFF_EVENT��CTRL_SHUTDOWN_EVENTͨ������������һЩ�������������
Ȼ�����ExitProcess API�����⣬�������¼��г�ʱ���ƣ�CTRL_CLOSE_EVENT��5�룬����������20�롣
�������ʱ��ϵͳ���ᵯ���������̵ĶԻ�������û�ѡ���˽������̣�
�κ���������������������Ӧ���ڳ�ʱʱ������ɹ���
*/
//�¼�����ص�
BOOL HandlerRoutine(DWORD dwCtrlType)  
{  
	switch (dwCtrlType)  
	{  
	case CTRL_C_EVENT:						//Ctrl+C��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "ctrl+c--���򲻻�ر�";
		return TRUE;  
	case CTRL_CLOSE_EVENT:					 //���û��ر�Consoleʱ��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "ctrl close--�����ر�";
		return TRUE;  
	case CTRL_BREAK_EVENT:					//Ctrl+break��ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_BREAK_EVENT";  
		return TRUE;
	case CTRL_LOGOFF_EVENT:					//�û��˳���ע������ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_LOGOFF_EVENT--�û��˳���ע��";  
		return TRUE;
	case CTRL_SHUTDOWN_EVENT:				//ϵͳ�رգ�ϵͳ�ᷢ�ʹ���Ϣ
		qDebug() << "CTRL_SHUTDOWN_EVENT--ϵͳ�ر�";  
		return TRUE;
	default: 
		return FALSE;  
	}  
} 

//���캯��
Widget::Widget()
{
	//��װ�¼�����
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE))
	{
		qDebug() << "��װ�¼�����ʧ�ܣ�";
	}

	//step = 0;
	//baseTimer.start(60, this);

	ClientBaseOperating::touchConfigFile();//����һ�������ļ�
	readyReadDb = 0;//û�д����ݿ��ж�ȡ
	DBModule::initDB();//��ʼ�����ݿ�
	ClientBaseOperating::mkdirDownloadDir();//����һ�����ظ���Ŀ¼
	ClientBaseOperating::mkdirAlbumDir();//����һ���ļ������洢���ص�ר��
	ClientBaseOperating::mkdirLrcDir();//����һ�����ظ��Ŀ¼

    //���ô��ڻ�������
    this ->resize(400, 212);//���ô����С
    this ->setWindowFlags(Qt::FramelessWindowHint);//ȥ������߿�
    this ->setAttribute(Qt::WA_TranslucentBackground);//���ñ���͸��
    this ->setWindowIcon(QIcon(":/images/CZPlayer.png"));//����logo
    this ->setAcceptDrops(true);//�����Ϸ��ļ�
    this ->setWindowTitle(tr("CZPlayer"));

    //    qDebug() << "������Ļ��С��" << QApplication::desktop() ->width() << ", " << QApplication::desktop() ->height();
    //    qDebug() << "��굱ǰλ�ã�" << QCursor::pos();
    //    qDebug() << "��������С��" << this ->frameGeometry();
    //    qDebug() << "��������������Ļ�ľ������꣺" << this ->mapToParent(QPoint(0, 0));
    //    qDebug() << "��������������Ļ�ľ������꣺" << this ->pos();
    //    qDebug() << frameGeometry().topRight();

    //���ý��潥����ʾ
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation ->setDuration(1000);
    animation ->setStartValue(0);
    animation ->setEndValue(1);
    animation ->start();

    //����ý�����
    mediaObject = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(mediaObject, audioOutput);//��Դ�ͽ�����

    //�������Ʋ��Ž��ȵĻ���
    seekSlider = new Phonon::SeekSlider(mediaObject, this);
    seekSlider ->setStyleSheet("QSlider::groove:horizontal:!hover{border-image: url(:/images/progressBar.png);}"
                               "QSlider::groove:horizontal:hover{border-image: url(:/images/progressBar2.png);}"
                               //"QSlider::add-page{border-image: url(:/images/jd.png);}"
                               "QSlider::sub-page{border-image: url(:/images/progressBarSub.png);}");
    //    seekSlider->setStyleSheet("QSlider::groove:horizontal:!hover{border-image: url(:/images/progressBar.png);}"
    //                              "QSlider::groove:horizontal:hover{border-image: url(:/images/progressBar2.png);}");
    //                              "QSlider::sub-page{background:green; border-radius:7px; height:7}");
    //                              "QSlider::add-page{background:pink;}");
    seekSlider ->setObjectName(tr("seekSlider"));

    //�����������ڻ���
    volumeSlider = new Phonon::VolumeSlider(audioOutput, this);
    volumeSlider ->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    volumeSlider ->setStyleSheet("QSlider::groove:horizontal:!hover{border-image: url(:/images/volumeBar.png);}"
                                 "QSlider::groove:horizontal:hover{border-image: url(:/images/volumeBar2.png);}"
                                 //"QSlider::add-page{border-image: url(:/images/jd.png);}"
                                 "QSlider::sub-page{border-image: url(:/images/volumeBarSub.png);}");
    volumeSlider ->setObjectName(tr("volumeSlider"));

    //ר������
    albumImage = new QLabel(this);
    albumImage ->setToolTip(tr("ר������"));
    albumImage ->setPixmap(QPixmap(tr(":/images/albumImage.png")));
    movie = new QMovie(tr(":/images/jiaZai.gif"));
//    albumImage ->setMovie(movie);
//    movie ->start();

    //��Ϣ��ʾ��ǩ
    InfoLabel = new QLabel(this);
    InfoLabel ->setObjectName("InfoLabel");
    InfoLabel ->setText(tr("Welecome to CZPlayer!"));

    //��ʱ�ǩ
	lrcLabel = new QLabel(this);
	lrcLabel ->setObjectName(tr("lrcLabel"));
	lrcLabel ->setText(tr("highway-9"));

    //������ʾʱ���ǩ
    timeLabel = new QLabel(tr("00:00/00:00"), this);
    timeLabel ->setToolTip(tr("��ǰʱ��/��ʱ��"));
    timeLabel ->setObjectName(tr("timeLabel"));
    timeLabel ->setAlignment(Qt::AlignCenter);//���ö����ʽ
    //QSizePolicy��������ˮƽ�ʹ�ֱ�޸Ĵ�С���Ե�һ������
    timeLabel ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//ˮƽ�����ϳߴ����չ����ֱ�����ѹ̶�

    //��ʾ����ҳ��
    showButton = new QPushButton(this);
    showButton ->setObjectName(tr("showButton"));
    showButton ->setToolTip(tr("�������б�"));
    showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/showButton.png);}"
                               "QPushButton:hover{border-image: url(:/images/showButton2.png);}"
                               "QPushButton:pressed{border-image: url(:/images/showButton3.png);}");

    //����
    playButton = new QPushButton(this);
    playButton ->setToolTip(tr("����"));
    playButton ->setObjectName(tr("playButton"));
    playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
                               "QPushButton:hover{border-image: url(:/images/playButton2.png);}"
                               "QPushButton:pressed{border-image: url(:/images/playButton3.png);}");

    //ֹͣ
    endButton = new QPushButton(this);
    endButton ->setToolTip(tr("ֹͣ"));
    endButton ->setObjectName(tr("endButton"));

    //��һ��
    preButton = new QPushButton(this);
    preButton ->setToolTip(tr("��һ��"));
    preButton ->setObjectName(tr("preButton"));

    //��һ��
    nextButton = new QPushButton(this);
    nextButton ->setToolTip(tr("��һ��"));
    nextButton ->setObjectName(tr("nextButton"));

    //���ļ�
    openFileButton = new QPushButton(this);
    openFileButton ->setToolTip(tr("���ļ�"));
    openFileButton ->setObjectName(tr("openFileButton"));

    //�򿪲����б�
    musicListButton = new QPushButton(this);
    musicListButton ->setToolTip(tr("�򿪲����б�"));
    musicListButton ->setObjectName(tr("musicListButton"));

    //������
    lrcButton = new QPushButton(this);
    lrcButton ->setToolTip(tr("������"));
    lrcButton ->setObjectName(tr("lrcButton"));

    //�������˳�
    closeAction = new QAction(this);
    closeAction ->setIcon(QIcon(":/images/quitAction.png"));
    closeAction ->setText(tr("�˳�"));

    //��������С��
    minAction = new QAction(this);
    minAction ->setIcon(QIcon(":/images/minAction.png"));
    minAction ->setText(tr("��С��"));

    //���̽����˳�����
    quitAction = new QAction(this);
    quitAction ->setIcon(QIcon(":/images/quitAction.png"));
    quitAction ->setText(tr("�˳�"));

    //��ͣ(����)
    pauseAction = new QAction(this);
    //    pauseAction ->setIcon(QIcon(":/images/trayPause.png"));
    //    pauseAction ->setText(tr("��ͣ"));

    //ֹͣ
    stopAction = new QAction(this);
    stopAction ->setIcon(QIcon(":/images/trayStop.png"));
    stopAction ->setText(tr("ֹͣ"));

    //��һ��
    lastAction = new QAction(this);
    lastAction ->setIcon(QIcon(":/images/trayLast.png"));
    lastAction ->setText(tr("��һ��"));

    //��һ��
    nextAction = new QAction(this);
    nextAction ->setIcon(QIcon(":/images/trayNext.png"));
    nextAction ->setText(tr("��һ��"));

    //�������
    unlockLrcAction = new QAction(this);
    unlockLrcAction ->setIcon(QIcon(":/images/unlock.png"));
    unlockLrcAction ->setText(tr("�������"));

    //�������ļ�
    openFileAction = new QAction(this);
    openFileAction ->setIcon(QIcon(":/images/folder.png"));
    openFileAction ->setText(tr("�������ļ�"));

    //�򿪲����б�
    openMusicListAction = new QAction(this);
    openMusicListAction ->setIcon(QIcon(":/images/openMusicList.png"));
    openMusicListAction ->setText(tr("�򿪲����б�"));

    //�������б�
    openMusicDownloadAction = new QAction(this);
    openMusicDownloadAction ->setIcon(QIcon(":/images/openMusicDownload.png"));
    openMusicDownloadAction ->setText(tr("�������б�"));

    //��ʾ������
    openLrcAction = new QAction(this);
    openLrcAction ->setIcon(QIcon(":/images/lrc.png"));
    openLrcAction ->setText(tr("��ʾ������"));

    //ѡ��
    configAction = new QAction(this);
    configAction ->setIcon(QIcon(":/images/configButton.png"));
    configAction ->setText(tr("����"));

    //����Qt
    aboutQtAction = new QAction(this);
    aboutQtAction ->setIcon(QIcon(":/images/Qt.png"));
    aboutQtAction ->setText(tr("����Qt"));

	//����
	aboutAction = new QAction(this);
	aboutAction ->setIcon(QIcon(":/images/CZPlayer2.png"));
	aboutAction ->setText(tr("����CZPlayer"));

    //����ϵͳ����ͼ��
    trayIcon = new QSystemTrayIcon(QIcon(":/images/CZPlayer.png"), this);
    trayIcon ->show();
    trayIcon ->showMessage(tr("CZPlayer"), tr("��ӭ��½www.highway-9.com"));
    trayIcon ->setToolTip(tr("CZPlayer"));

    //�����˵���ϵͳ����ͼ���һ����ֵĲ˵�
    trayMenu = new QMenu();
    trayMenu ->addAction(quitAction);
    trayMenu ->addAction(unlockLrcAction);
    trayMenu ->addSeparator();
    trayMenu ->addAction(pauseAction);
    trayMenu ->addAction(stopAction);
    trayMenu ->addSeparator();
    trayMenu ->addAction(lastAction);
    trayMenu ->addAction(nextAction);
    trayIcon ->setContextMenu(trayMenu);//��ϵͳ��������Ӳ˵�
    trayIcon ->show();

    //С���沥����
    minMusicWidget = new MinMusicWidget(mediaObject, &sources, this);
    //�����б�
    playList = new MusicList(mediaObject, &sources, this);
    musicListWidget = new MusicListWidget(playList, &musicInfoMap, this);
    //���
    lrc = new MusicLrc();
    //���ظ��
    musicDownload = new MusicDownload(this);
    //���ý���
    configDialog = new ConfigDialog(this);
	//����CZPlayer
	aboutPanel = new AboutPanel;
    //ȫ���ȼ�����
    globalHotKey = new GlobalHotKey(this);

    //���ø������
	ClientBaseOperating::initLrcPropery();
    //����ȫ���ȼ�
	ClientBaseOperating::initHotKeyPropery();

	//�����豸�����߳�
	hotpugWatcherThread = new HotplugWatcherThread;
	//hotpugWatcherThread ->start();//���߳��ڲ��Ѿ�start

    //����һ����ʱ��
    timer = new QTimer(this);
    timer ->start(30);
	shortCutIimer = new QTimer(this);
	shortCutIimer ->start(500);

    //����ý��Դ��������
    metaInformationResolver = new Phonon::MediaObject(this);
    metaInformationAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(metaInformationResolver, metaInformationAudioOutput);//��Դ�ͽ�����

    //�����ݿ��ж�����Ϣ���������б���
    this ->readFromDb();

    //���������С��λ��
    albumImage->setGeometry(25, 23, 110, 103);
    timeLabel ->setGeometry(295, 23, 82, 18);
    InfoLabel ->setGeometry(153, 69, 240, 39);
	lrcLabel ->setGeometry(153, 108, 240, 39);
    showButton ->setGeometry(365, 55, 17, 17);

    preButton ->setGeometry(33, 164, 25, 25);
    playButton ->setGeometry(83, 164, 25, 25);
    endButton ->setGeometry(133, 164, 25, 25);
    nextButton ->setGeometry(183, 164, 25, 25);

    seekSlider ->setGeometry(22, 137, 361, 14);
    openFileButton ->setGeometry(152, 21, 49, 23);
    musicListButton ->setGeometry(201, 20, 35, 24);
    lrcButton ->setGeometry(236, 21, 44, 23);
    volumeSlider ->setGeometry(253, 170, 128, 15);

    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(slot_MetaStateChanged(Phonon::State,Phonon::State)));
    //��ý������״̬�����ı�ʱ���ͻ��Զ�����stateChanged()�źţ�
    //������źź󣬾Ϳ�������Щ״̬������һЩ�йص�����
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(slot_StateChanged(Phonon::State,Phonon::State)));

    connect(openFileButton, SIGNAL(clicked()), this, SLOT(slot_OpenFile()));									//�������ļ�
    connect(musicListButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicList()));								//�������б����
    connect(lrcButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicLrc()));										//�򿪸��
    connect(showButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicDownload()));								//�򿪸������ؽ���
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slot_TrayIconActivated(QSystemTrayIcon::ActivationReason)));								//����ͼ�걻�������д���
    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(slot_UpdateTime(qint64)));							//����ʱ��,����ý������tick�ź������²���ʱ�����ʾ
    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(slot_RowChanged(Phonon::MediaSource)));													//ѡ�е�ǰ����ý��Դ����
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(slot_AboutMusicFinish()));							//���ֲ��Ž���
    connect(playList, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_TableDoubleClicked(int, int)));		//������˫�������б�
    connect(playList, SIGNAL(sig_PlayListClean()), this, SLOT(slot_ClearSources()));								//���ý��Դ
    connect(playList, SIGNAL(sig_RowSelected(int, QString)), this, SLOT(slot_DelSelectSource(int, QString)));		//ɾ��ѡ�е�Դ
    connect(lrc, SIGNAL(sig_LrcLocked()), this, SLOT(slot_ShowLrcLocked()));										//��ʾ����Ѿ�������Ϣ
    connect(lrc, SIGNAL(sig_LrcUnlocked()), this, SLOT(slot_ShowLrcUnlocked()));									//��ʾ����Ѿ�������Ϣ
    connect(unlockLrcAction, SIGNAL(triggered()), this, SLOT(slot_UnlockLrc()));								//�������
    connect(playButton, SIGNAL(clicked()), this, SLOT(slot_SetPaused()));										//��ͣ
    connect(endButton, SIGNAL(clicked()), this, SLOT(slot_SetStop()));											//ֹͣ
    connect(preButton, SIGNAL(clicked()), this, SLOT(slot_SetPre()));											//��һ��
    connect(nextButton, SIGNAL(clicked()), this, SLOT(slot_SetNext()));											//��һ��
    connect(minAction, SIGNAL(triggered()), this, SLOT(slot_MinToTray()));										//��С��
    connect(closeAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//�˳�����
    connect(quitAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//�˳�����
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(slot_SetPaused()));									//��ͣ(����)
    connect(stopAction, SIGNAL(triggered()), this, SLOT(slot_SetStop()));										//ֹͣ
    connect(lastAction, SIGNAL(triggered()), this, SLOT(slot_SetPre()));										//��һ��
    connect(nextAction, SIGNAL(triggered()), this, SLOT(slot_SetNext()));										//��һ��
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));												//����һ����ʱ��
	connect(shortCutIimer, SIGNAL(timeout()), this, SLOT(slot_ShortCutsetDisable()));							//����ȫ���ȼ�����/����
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(slot_OpenFile()));									//�������ļ�
    connect(openMusicListAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicList()));						//�����ֲ����б�
    connect(openMusicDownloadAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicDownload()));				//�����������б�
    connect(openLrcAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicLrc()));								//��ʾ������
    connect(configAction, SIGNAL(triggered()), this, SLOT(slot_ConfigCZPlayer()));								//����
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slot_AboutCZPlayer()));								//����CZPlayer
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(slot_AboutQt()));									//����Qt
    connect(minMusicWidget, SIGNAL(sig_OpenFileButtonClicked()), this, SLOT(slot_OpenFile()));						//�������ļ�
    connect(minMusicWidget, SIGNAL(sig_MusicLrc()), this, SLOT(slot_OpenMusicLrc()));								//�򿪸��
    connect(minMusicWidget ,SIGNAL(sig_ShowMusicPanel()), this, SLOT(slot_ShowMusicPanel()));						//��ʾ������
    //    connect(minMusicWidget, SIGNAL(minPlay()), this, SLOT(slot_SetPaused()));//����
    //    connect(minMusicWidget, SIGNAL(minPause()), this, SLOT(slot_SetPaused()));//��ͣ
    //    connect(minMusicWidget, SIGNAL(minStop()), this, SLOT(slot_SetStop()));//ֹͣ
    //    connect(minMusicWidget, SIGNAL(minNext()), this, SLOT(slot_SetNext()));//��һ��
    //    connect(minMusicWidget, SIGNAL(minLast()), this, SLOT(slot_SetPre()));//��һ��

    connect(musicListWidget, SIGNAL(sig_AddMusic()), this, SLOT(slot_OpenFile()));									//��Ӹ���
	connect(musicListWidget, SIGNAL(sig_AddMusicList()), this, SLOT(slot_AddMusicList()));							//��Ӳ����б�
    connect(this, SIGNAL(sig_ShowMainWidget()), minMusicWidget, SLOT(slot_OpenMainWidget()));					//���͵����������ʾ������

	//����豸�Ĳ���
	//QDBusConnection::systemBus().connect("org.freedesktop.Hal",
	//	"/org/freedesktop/Hal/Manager",
	//	"org.freedesktop.Hal.Manager",
	//	"DeviceAdded", this, SLOT(slotDeviceAdded(QString)));

	//����豸�Ĳ���
	//QDBusConnection::systemBus().connect("org.freedesktop.Hal",
	//	"/org/freedesktop/Hal/Manager",
	//	"org.freedesktop.Hal.Manager",
	//	"DeviceRemoved", this, SLOT(slotDeviceRemoved(QString)));

}

//��������
Widget::~Widget()
{
	if (musicDownload) {delete musicDownload; musicDownload = 0;}
	if (playList) {delete playList; playList = 0;}
	if (musicListWidget) {delete musicListWidget; musicListWidget = 0;}
	if (lrc) {delete lrc; lrc = 0;}
	if (minMusicWidget) {delete minMusicWidget; minMusicWidget = 0;}

	//terminate:��ֹ�߳�
	if (hotpugWatcherThread ->isRunning()) {hotpugWatcherThread ->terminate(); delete hotpugWatcherThread; hotpugWatcherThread = 0;}
}

//�����ݿ��ж��������ļ���Ϣ�������ֲ����б�
void Widget::readFromDb()
{
	QStringList list;
	DBModule::readFromDb(list);
	if (list.isEmpty())
	{
		return;
	}

    sources.clear();
    //��ȡ��ǰý��Դ�б�Ĵ�С
    int index = sources.size();

    foreach (QString str, list)
    {
        Phonon::MediaSource source(str);//��ÿһ�����ּ��س�һ��Դ
        sources.append(source);//��ÿһ��Դ��ӵ�ý����Դ�б�
    }

    if (!sources.isEmpty())
    {
        //���ý��Դ�б�Ϊ�գ����¼���ĵ�һ��ý��Դ��Ϊ��ǰý��Դ
        metaInformationResolver ->setCurrentSource(sources.at(index));
    }

    mediaObject ->setCurrentSource(sources.at(0));
    mediaObject ->play();
    readyReadDb = 1;//�Ѿ������ݿ��ж���
}

//����һ����ʱ��
void Widget::slot_TimeOut()
{
    if (lrc ->text().isEmpty())
    {
        lrc ->stopLrcMask();
    }
    if (lrcLabel ->text().isEmpty())
    {
    }

    //��������б�Ϊ�գ��򲥷Ű�ť������
    if (playList ->rowCount() == 0)
    {
        playButton ->setEnabled(false);
        endButton  ->setEnabled(false);
        preButton ->setEnabled(false);
        nextButton ->setEnabled(false);

        minMusicWidget ->minPlayButton ->setEnabled(false);
        minMusicWidget ->minPauseButton ->setEnabled(false);
        minMusicWidget ->minStopButton ->setEnabled(false);
        minMusicWidget ->minNextButton ->setEnabled(false);
        minMusicWidget ->minLastButton ->setEnabled(false);

        pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
        pauseAction ->setText(tr("����"));
        pauseAction ->setEnabled(false);
        stopAction ->setEnabled(false);
        lastAction ->setEnabled(false);
        nextAction ->setEnabled(false);
    }
    else
    {
        playButton ->setEnabled(true);
        endButton  ->setEnabled(true);
        preButton ->setEnabled(true);
        nextButton ->setEnabled(true);

        minMusicWidget ->minPlayButton ->setEnabled(true);
        minMusicWidget ->minPauseButton ->setEnabled(true);
        minMusicWidget ->minStopButton ->setEnabled(true);
        minMusicWidget ->minNextButton ->setEnabled(true);
        minMusicWidget ->minLastButton ->setEnabled(true);

        pauseAction ->setEnabled(true);
        stopAction ->setEnabled(true);
        lastAction ->setEnabled(true);
        nextAction ->setEnabled(true);
    }
}

//ѡ������
void Widget::slot_ConfigOptions()
{
    if (configDialog ->isHidden())
    {
        this ->slot_ConfigCZPlayer();
    }
}

//��ʾ/����������
void Widget::slot_ShowHideMainWidget()
{
    this ->slot_MinToTray();
    if (this ->isHidden())
    {
        this ->slot_TrayIconActivated(QSystemTrayIcon::Trigger);
    }
}

//����ģʽ/��������
void Widget::slot_MinMainWidget()
{
    this ->slot_OpenMinWidget();
    if (this ->isHidden())
    {
        emit sig_ShowMainWidget();//��ʾ������
    }
}

//�������ļ�
void Widget::slot_OpenMusicfile()
{
    this ->slot_OpenFile();
}

//��ʾ/���������б�
void Widget::slot_ShowDownload()
{
    this ->slot_OpenMusicDownload();
}

//��ʾ/���ظ����б�
void Widget::slot_ShowMusicList()
{
    this ->slot_OpenMusicList();
}

//��ʾ/����������
void Widget::slot_ShowLrc()
{
    this ->slot_OpenMusicLrc();
}

//����/��ͣ
void Widget::slot_PlayPause()
{
    this ->slot_SetPaused();
}

//��һ��
void Widget::slot_Last()
{
    this ->slot_SetPre();
}

//��һ��
void Widget::slot_Next()
{
    this ->slot_SetNext();
}

//���ļ�
void Widget::slot_OpenFile()
{
    //����ͬʱ�򿪶����Ƶ�ļ�
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("�������ļ�"),
                                                     WINPATH, tr("music Files(*.mp3 *.wav *.wma *ogg)"));
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("�������ļ�"),
                                                     X11PATH, tr("music Files(*.mp3 *.wav *.wma *ogg)"));
#endif

    if (list.isEmpty())
    {
        return;
    }
    //��ȡ��ǰý��Դ�б�Ĵ�С
    int index = sources.size();

    foreach (QString str, list)
    {
        Phonon::MediaSource source(str);//��ÿһ�����ּ��س�һ��Դ
        sources.append(source);//��ÿһ��Դ��ӵ�ý����Դ�б�
    }

    if (!sources.isEmpty())
    {
        //���ý��Դ�б�Ϊ�գ����¼���ĵ�һ��ý��Դ��Ϊ��ǰý��Դ
        metaInformationResolver ->setCurrentSource(sources.at(index));
    }

    int static count = 0;
    ++count;
    if (count == 1 && readyReadDb == 0)//��һ�δ��ļ�����û�д����ݿ��ж�ȡ
    {
        mediaObject ->setCurrentSource(sources.at(0));
        mediaObject ->play();
    }
}

//��Ӳ����б�
void Widget::slot_AddMusicList()
{
	//ֻ�ܴ�һ�������б��ļ�
#ifdef _WIN32_WINNT
	QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("���벥���б�"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#else
	QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("���벥���б�"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#endif

	if (fileName != "")
	{
		QString strSuffixName = fileName.right(3);//�õ���׺��
		//��ȡ��ǰý��Դ�б�Ĵ�С
		int index = sources.size();
		char buf[256];
		string sBuf;
		int i = 0;
		int n = 1;
		ifstream file(fileName.toStdString());
		while (!file.eof())
		{
			file.getline(buf, 256, '\n');
			sBuf = buf;

			if (i == 0)		//��ȡ��ͷ
			{
				if (sBuf != "#EXTM3U" && sBuf != "[playlist]")
				{
					QMessageBox::information(NULL, tr("��Ϣ"), tr("������Ч��m3u��pls�ļ���"), QMessageBox::Yes);
					return;
				}
			}
			else
			{
				if ((strSuffixName == "m3u" || strSuffixName == "M3U") && i % 2 == 0)
				{
					Phonon::MediaSource source(QString::fromStdString(sBuf));//��ÿһ�����ּ��س�һ��Դ
					sources.append(source);//��ÿһ��Դ��ӵ�ý����Դ�б�
				}
				else if ((strSuffixName == "pls" || strSuffixName == "PLS") && i == 1 + (n - 1) * 3)
				{
					if (!QString::fromStdString(sBuf).contains("NumberOfEntries"))
					{
						++n;
						if (QString::fromStdString(sBuf).contains("="))
						{
							QStringList list = QString::fromStdString(sBuf).split("=");
							QString filePath = list.at(1);
							Phonon::MediaSource source(filePath);//��ÿһ�����ּ��س�һ��Դ
							sources.append(source);//��ÿһ��Դ��ӵ�ý����Դ�б�
						}
					}
				}
			}
			++i;
		}
		file.close();

		if (!sources.isEmpty())
		{
			//���ý��Դ�б�Ϊ�գ����¼���ĵ�һ��ý��Դ��Ϊ��ǰý��Դ
			metaInformationResolver ->setCurrentSource(sources.at(index));
		}

		int static count = 0;
		++count;
		if (count == 1 && readyReadDb == 0)//��һ�δ��ļ�����û�д����ݿ��ж�ȡ
		{
			mediaObject ->setCurrentSource(sources.at(0));
			mediaObject ->play();
		}
	}
}

//����С����
void Widget::slot_OpenMinWidget()
{
    if (minMusicWidget ->isHidden())
    {
        QPropertyAnimation *minWidgetdAnimation = new QPropertyAnimation(minMusicWidget, "windowOpacity");
        minWidgetdAnimation ->setDuration(500);
        minWidgetdAnimation ->setStartValue(0);
        minWidgetdAnimation ->setEndValue(1);
        minWidgetdAnimation ->start();
        minMusicWidget ->show();

        QPropertyAnimation *mainWidgetdAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetdAnimation ->setDuration(500);
        mainWidgetdAnimation ->setStartValue(1);
        mainWidgetdAnimation ->setEndValue(0);
        mainWidgetdAnimation ->start();
        connect(mainWidgetdAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));

        QPropertyAnimation *musicDownloadAnimation = new QPropertyAnimation(musicDownload, "windowOpacity");
        musicDownloadAnimation ->setDuration(500);
        musicDownloadAnimation ->setStartValue(1);
        musicDownloadAnimation ->setEndValue(0);
        musicDownloadAnimation ->start();
        connect(musicDownloadAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicDownload()));

        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
}

//�򿪲����б�
void Widget::slot_OpenMusicList()
{
    if (musicListWidget ->isHidden())
    {
        musicListWidget ->move(frameGeometry().bottomLeft());//��ʾ����������·�
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(0);
        musicListAnimation ->setEndValue(1);
        musicListAnimation ->start();
        musicListWidget ->show();
        openMusicListAction ->setText(tr("�رղ����б�"));
        musicListShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
        openMusicListAction ->setText(tr("�򿪲����б�"));
        musicListShowFlag = 0;
    }
}

//�򿪸��
void Widget::slot_OpenMusicLrc()
{
    if (lrc ->isHidden())
    {
        //lrc ->show();
        lrc ->showNormal();
        openLrcAction ->setText(tr("�ر�������"));
        minMusicWidget ->minMusicLrcAction ->setText(tr("�ر�������"));
    }
    else
    {
        lrc ->hide();
        openLrcAction ->setText(tr("��ʾ������"));
        minMusicWidget ->minMusicLrcAction ->setText(tr("��ʾ������"));
    }
}

//�����ظ�������
void Widget::slot_OpenMusicDownload()
{
    if (musicDownload ->isHidden())
    {
        musicDownload ->move(frameGeometry().topRight());//��ʾ����������ҷ�
        QPropertyAnimation *downloadAnimation = new QPropertyAnimation(musicDownload, "windowOpacity");
        downloadAnimation ->setDuration(500);
        downloadAnimation ->setStartValue(0);
        downloadAnimation ->setEndValue(1);
        downloadAnimation ->start();
        musicDownload ->show();
        openMusicDownloadAction ->setText(tr("�ر������б�"));
        showButton ->setToolTip(tr("�ر������б�"));
        showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/hideButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/hideButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/hideButton3.png);}");
        musicDownloadShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *downloadAnimation = new QPropertyAnimation(musicDownload, "windowOpacity");
        downloadAnimation ->setDuration(500);
        downloadAnimation ->setStartValue(1);
        downloadAnimation ->setEndValue(0);
        downloadAnimation ->start();
        connect(downloadAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicDownload()));
        openMusicDownloadAction ->setText(tr("�������б�"));
        showButton ->setToolTip(tr("�������б�"));
        showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/showButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/showButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/showButton3.png);}");
        musicDownloadShowFlag = 0;
    }
}

//�������ظ�������
void Widget::slot_HideMusicDownload()
{
    musicDownload ->hide();
}

//�������ֲ������б�
void Widget::slot_HideMusicList()
{
    musicListWidget ->hide();
}

//����������
void Widget::slot_HideMainWidget()
{
    this ->hide();
}

//��ʾ������
void Widget::slot_ShowMusicPanel()
{
    if (musicDownloadShowFlag == 1)
    {
        this ->slot_OpenMusicDownload();
    }
    if (musicListShowFlag == 1)
    {
        this ->slot_OpenMusicList();
    }
}

//����
void Widget::slot_ConfigCZPlayer()
{
    ConfigDialog::contentsWidget ->setCurrentRow(0);//ѡ�е�һҳ
	ClientBaseOperating::initConfigDialog();//��ʼ�����öԻ���
    configDialog ->exec();//ģ̬��ʾ�Ի���
}

//����Qt
void Widget::slot_AboutQt()
{
    QMessageBox::aboutQt(this, "����Qt");
}

//����CZPlayer
void Widget::slot_AboutCZPlayer()
{
	AboutPanel::tabWidget ->setCurrentIndex(0);
	aboutPanel ->exec();//ģ̬��ʾ�Ի���
}

//����LRC��ʣ���stateChanged()������Phonon::PlayingState��(������ǰ���Ÿ�ĸ��)
//��slot_AboutMusicFinish()�����е����˸ú���(��ʼ������һ�׸�ĸ��)
void Widget::resolveLrc(const QString &sourceFileName)
{
    lrcMap.clear();
    if (sourceFileName.isEmpty())
    {
        return;
    }

    QString lrcFileInfo = QFileInfo(mediaObject ->currentSource().fileName()).baseName();
#ifdef _WIN32_WINNT
    QString lrcDirName = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
#else
    QString lrcDirName = QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
#endif
    QString lrcFileName = lrcDirName + "/" + lrcFileInfo + ".lrc";
    m_lrcFileName = lrcFileName;

    //����ר��ͼƬ
    QString fileNameInfo = QFileInfo(mediaObject ->currentSource().fileName()).baseName();
    if (!fileNameInfo.contains(" - "))//û�а������ַ���(��ʽΪ���������)
    {
        QString name = fileNameInfo;//������
        int index = sources.indexOf(mediaObject ->currentSource());
        QTableWidgetItem *item = playList ->item(index, 2);
        QString artist = item ->text();//����

#ifdef _WIN32_WINNT
    QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
    QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

        QString albumFileName = albumImagesName + "/" + artist + " - " + name + ".jpg";
        QFile fileAlbum(albumFileName);
        if (fileAlbum.open(QIODevice::ReadOnly))//���ļ��ɹ�
        {
			//�������س���û��������
			if (fileAlbum.size() <  4500)
			{
				//����ר��ͼƬ
				AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
				albumThread ->start();//����һ���߳�
			}
			else
			{
				albumImage ->setPixmap(QPixmap(albumFileName));
			}
        }
        else
        {
            //����ר��ͼƬ
            AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
            albumThread ->start();//����һ���߳�
        }
    }
    else//�������ַ�������ʽΪ��������� - �ƼҾԣ�
    {
        //��ר��ͼƬ
        QStringList ablumList = fileNameInfo.split(" - ");
        QString name = ablumList.at(1);//������
        QString artist = ablumList.at(0);//����

#ifdef _WIN32_WINNT
    QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
    QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

        QString albumFileName = albumImagesName + "/" + artist + " - " + name + ".jpg";
        QFile fileAlbum(albumFileName);
        if (fileAlbum.open(QIODevice::ReadOnly))//���ļ��ɹ�
        {
            //�������س���û��������
            if (fileAlbum.size() <  4500)
            {
                //����ר��ͼƬ
                AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
                albumThread ->start();//����һ���߳�
            }
            else
            {
                albumImage ->setPixmap(QPixmap(albumFileName));
            }
        }
        else
        {
            //����ר��ͼƬ
            AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
            albumThread ->start();//����һ���߳�
        }
    }

    //�򿪸��
    QFile file(lrcFileName);
    if (!file.open(QIODevice::ReadOnly))//���ظ����ļ�
    {
        lrc ->setText(QFileInfo(mediaObject ->currentSource().fileName()).baseName() +
                      tr("----�������ظ���ļ�!"));

        QString fileInfo = QFileInfo(mediaObject ->currentSource().fileName()).baseName();
        if (!fileInfo.contains(" - "))//û�а������ַ���
        {
            QString musicName = fileNameInfo;//������
            int index = sources.indexOf(mediaObject ->currentSource());
            QTableWidgetItem *item = playList ->item(index, 1);
            QString musicArtist = item ->text();//����

			//���ظ��
			LrcThread *lrcThread = new LrcThread(musicName, musicArtist, m_lrcFileName, lrc);
			lrcThread ->start();//����һ���߳�
			connect(lrcThread, SIGNAL(sig_ResolveLrc()), this, SLOT(slot_ResolveLrc()));
            return;
        }
        else
        {
            QStringList list = fileInfo.split(" - ");
            QString musicName = list.at(1);//������
            QString musicArtist = list.at(0);//����

			//���ظ��
			LrcThread *lrcThread = new LrcThread(musicName, musicArtist, m_lrcFileName, lrc);
			lrcThread ->start();//����һ���߳�
			connect(lrcThread, SIGNAL(sig_ResolveLrc()), this, SLOT(slot_ResolveLrc()));
            return;
        }
    }

    QString allText = QString(file.readAll());
    file.close();

    //����ʰ��з�Ϊ����б�
    QStringList lrcLines = allText.split("\n");

    //�����ʱ���ǩ�ĸ�ʽ[00:05.54]
    //������ʽ\d{2}��ʾƥ����������
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");

    foreach (QString oneLine, lrcLines)
    {
        QString temp = oneLine;
        temp.replace(rx, "");//�ÿ��ַ����滻������ʽ����ƥ��ĵط�,�����ͻ���˸���ı�

        // Ȼ�����λ�ȡ��ǰ���е�����ʱ���ǩ�����ֱ������ı�����QMap��
        //indexIn()Ϊ���ص�һ��ƥ���λ�ã��������Ϊ-1�����ʾû��ƥ��ɹ�
        //���������pos����Ӧ�ö�Ӧ���Ǹ���ļ�

        int pos = rx.indexIn(oneLine, 0);
        while (pos != -1)//��ʾƥ��ɹ�
        {
            QString cap = rx.cap(0);//���ص�0�����ʽƥ�������

            //��ʱ���ǩת��Ϊʱ����ֵ���Ժ���Ϊ��λ
            QRegExp regexp;

            //��÷���
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();

            //�����
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();

            //��ú���
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int milliseSecond = regexp.cap(0).toInt();

            //�����ʱ��
            qint64 totalTime = minute * 60000 + second * 1000 + milliseSecond * 10;

            //��ÿһ�е�ʱ��͸�ʲ��뵽lrcMap��
            lrcMap.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneLine, pos);//ƥ��ȫ��
        }
    }

    //���lrcMapΪ��
    if (lrcMap.isEmpty())
    {
        lrc ->setText(QFileInfo(mediaObject ->currentSource().fileName()).baseName()
                      + tr("----����ļ����ݴ���!"));
        return;
    }
}

//���ý������
void Widget::slot_ResolveLrc()
{
	this ->resolveLrc(mediaObject ->currentSource().fileName());//������ǰ���ظ��
}

//����ʱ��
void Widget::slot_UpdateTime(qint64 time)
{
    qint64 totalTimeValue = mediaObject ->totalTime();//���û�ȡ����Ƶ�ļ�����ʱ����������λΪ����
    //��3�������ֱ������ʱ,��,��,60000����Ϊ1����,
    //���Է��ӵڶ����������ȳ�60000,��3��������ֱ�ӳ�1s
    QTime totalTime(0, (totalTimeValue / 60000) % 60, (totalTimeValue / 1000) % 60);
    QTime currentTime(0, (time / 60000) % 60, (time / 1000) % 60);
    QString str = currentTime.toString("mm:ss") + "/" + totalTime.toString("mm:ss");
    timeLabel ->setText(str);

    //��ȡ��ǰʱ���Ӧ�ĸ��
    if (!lrcMap.isEmpty())
    {
        //��ȡ��ǰʱ���ڸ���е�ǰ������ʱ���
        qint64 previousTime = 0;
        qint64 laterTime = 0;

        //keys()��������lrcMap�б�
        foreach (qint64 value, lrcMap.keys())
        {
            if (time >= value)
            {
                previousTime = value;
            }
            else
            {
                laterTime = value;
                break;
            }
        }

        // �ﵽ���һ��,��laterTime����Ϊ������ʱ���ֵ
        if (laterTime == 0)
        {
            laterTime = totalTimeValue;
        }

        //��ȡ��ǰʱ������Ӧ�ĸ������
        QString currentLrc = lrcMap.value(previousTime);

        // ������µ�һ�и�ʣ���ô���¿�ʼ��ʾ�������
        if (currentLrc != lrc ->text())
        {
            lrc ->setText(currentLrc);
			lrcLabel ->setText(currentLrc);
            qint64 intervalTime = laterTime - previousTime;//ʱ����
            lrc ->startLrcMask(intervalTime);//�����������
        }
    }
    else
    {
        lrc ->setText(QFileInfo(mediaObject ->currentSource().fileName()).baseName() +
                      tr("----δ�ҵ�����ļ�!"));
		lrcLabel ->setText(tr("highway-9"));
    }
}

//��ͣ����
void Widget::slot_SetPaused()
{
    if (mediaObject ->state() == Phonon::PlayingState)
    {
        mediaObject ->pause();
    }
    else
    {
        mediaObject ->play();
    }
}

//ֹͣ����
void Widget::slot_SetStop()
{
    if (mediaObject ->state() == Phonon::PlayingState || mediaObject ->state() == Phonon::PausedState)
    {
        mediaObject ->stop();
    }
}

//������һ��
void Widget::slot_SetPre()
{
    //QMovie *movie = new QMovie(tr(":/images/jiaZai.gif"));
    albumImage ->setPixmap(QPixmap(tr("")));
    albumImage ->setMovie(movie);
    movie ->start();
    int index = sources.indexOf(mediaObject ->currentSource());
    if (index != 0)//�����ǰ�ײ�Ϊ��һ�ף��򲥷���һ��
    {
        mediaObject ->setCurrentSource(sources.at(index - 1));
        mediaObject ->play();
    }
}

//������һ��
void Widget::slot_SetNext()
{
    //QMovie *movie = new QMovie(tr(":/images/jiaZai.gif"));
    albumImage ->setPixmap(QPixmap(tr("")));
    albumImage ->setMovie(movie);
    movie ->start();
    int index = sources.indexOf(mediaObject ->currentSource()) + 1;
    if (sources.size() > index)//�����ǰ�ײ�Ϊ���һ�ף��򲥷���һ��
    {
        mediaObject ->setCurrentSource(sources.at(index));
        mediaObject ->play();
    }
}

//�òۺ����ǵ�ý��Դ�����ı�ʱ������currentSourceChanged()�źţ��Ӷ�ִ�иòۺ���
//�ú�����ɵĹ�����ѡ�����ı��ý��Դ����һ��
void Widget::slot_RowChanged(const Phonon::MediaSource &source)
{
    albumImage ->setPixmap(QPixmap(tr("")));
    albumImage ->setMovie(movie);
    movie ->start();
    int index = sources.indexOf(source);
    playList ->selectRow(index);
}

//������˫�����ֲ����б�ѡ����
void Widget::slot_TableDoubleClicked(int row, int /*column*/)
{
    //bool wasPlaying = mediaObject ->state() == Phonon::PlayingState;

    mediaObject ->stop();//ֹͣ��ǰ�����Ĳ���
    mediaObject ->clearQueue();//������Ŷ���

    //���ѡ�е��д���ý��Դ����У��򷵻�
    if (row >= sources.size())
    {
        return;
    }

    mediaObject ->setCurrentSource(sources.at(row));
    mediaObject ->play();

}

//���ý��Դ����ɾ�����ݿ�ȫ�����ݺ�map
void Widget::slot_ClearSources()
{
    sources.clear();
    mediaObject ->stop();
    mediaObject ->clear();

	if (!DBModule::clearDB())//������ݿ�
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("������ݿ�ʧ�ܣ�"), QMessageBox::Yes);
	}

    musicInfoMap.clear();
    albumImage ->setPixmap(QPixmap(tr(":/images/albumImage.png")));
}

//ɾ��ѡ�е�Դ����ɾ�����ݿ���Ӧ������
void Widget::slot_DelSelectSource(int row, QString title)
{
    //���ɾ���ĵ�ǰ��Ϊ���ڲ��ŵ���һ�У���ֹͣ�����ֵĲ���
    if (row == sources.indexOf(mediaObject ->currentSource()))
    {
        mediaObject ->stop();
    }
    sources.removeAt(row);

	if (!DBModule::delLine(title))//ɾ��һ��
	{
		QMessageBox::information(NULL, tr("��Ϣ"), tr("ɾ���ø���ʧ�ܣ�"), QMessageBox::Yes);
		return;
	}

    //����map
    this ->updateMap();
}

//����map
void Widget::updateMap()
{
    musicInfoMap.clear();
    for (int index = 0; index < playList ->rowCount(); ++index)
    {
        QTableWidgetItem *musicNameItem = playList ->item(index, 1);
        QString musicName = musicNameItem ->text();//������
        QTableWidgetItem *artistItem = playList ->item(index, 2);
        QString artist = artistItem ->text();//����
        QString str = musicName + "-" + artist;
        musicInfoMap.insert(make_pair(str, index));
    }
}

//��ý�岥�ſ����ʱ���ᷢ��aboutToFininsh()�ź�
void Widget::slot_AboutMusicFinish()
{
    int index = sources.indexOf(mediaObject ->currentSource()) + 1;
    if (sources.size() > index)
    {
        mediaObject ->setCurrentSource(sources.at(index));
        mediaObject ->play();
        lrc ->stopLrcMask();//ֹͣ�������
        resolveLrc(sources.at(index).fileName());//������һ�׸�ĸ��
    }
    else
    {
        mediaObject ->enqueue(sources.at(0));
        mediaObject ->play();
        lrc ->stopLrcMask();//ֹͣ�������
        resolveLrc(sources.at(0).fileName());//������һ�׸�ĸ��
        QString fileName = QFileInfo(sources.at(0).fileName()).baseName();
        InfoLabel ->setText(fileName);
    }
}

//����һЩ״̬����
void Widget::slot_StateChanged(Phonon::State newState, Phonon::State /*oldState*/)
{
    QString fileName;
    QString fileName2;
    switch (newState)
    {
    //����״̬Ϊ����״̬ʱ
    //    case Phonon::ErrorState:
    //        if (mediaObject ->errorType() == Phonon::FatalError)
    //        {
    //            QMessageBox::warning(this, tr("��������!"), mediaObject ->errorString());
    //        }
    //        else
    //        {
    //            QMessageBox::warning(this, tr("����!"), mediaObject ->errorString());
    //        }
    //        break;

    //����״̬Ϊ����״̬ʱ������һЩ�ؼ���״̬
    case Phonon::PlayingState:
        playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/stopButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/stopButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/stopButton3.png);}");
        playButton ->setToolTip(tr("��ͣ"));
        pauseAction ->setIcon(QIcon(":/images/trayPause.png"));
        pauseAction ->setText(tr("��ͣ"));
        fileName = QFileInfo(mediaObject ->currentSource().fileName()).baseName();
        InfoLabel ->setText(fileName);
        trayIcon ->setToolTip(tr("���ڲ���:%1").arg(fileName));
        resolveLrc(mediaObject ->currentSource().fileName());//�������ڲ��ŵĸ����ĸ��
        break;

        //��״̬Ϊ��ͣ״̬
    case Phonon::PausedState:
        playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/playButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/playButton3.png);}");
        playButton ->setToolTip(tr("����"));
        pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
        pauseAction ->setText(tr("����"));
        fileName2 = QFileInfo(mediaObject ->currentSource().fileName()).baseName();
        fileName2 += "\n��ͣ";
        InfoLabel ->setText(fileName2);
        trayIcon ->setToolTip(tr("CZPlayer"));
        lrc ->stopLrcMask();
        lrc ->setText(QFileInfo(mediaObject ->currentSource().fileName()).baseName());
		lrcLabel ->setText(tr("highway-9"));
        break;

        //��״̬Ϊֹͣ����
    case Phonon::StoppedState:
        playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/playButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/playButton3.png);}");
        playButton ->setToolTip(tr("����"));
        pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
        pauseAction ->setText(tr("����"));
        timeLabel ->setText(tr("00:00/00:00"));
        InfoLabel ->setText(tr("Welecome to CZPlayer!"));
        trayIcon ->setToolTip(tr("CZPlayer"));
        lrc ->stopLrcMask();
        lrc ->setText(tr("CZPlayer"));
		lrcLabel ->setText(tr("highway-9"));
        break;

    case Phonon::BufferingState:
        break;

    default:
        break;
    }
}

//ý��Դ�ı�ʱ������µ�Դ
void Widget::slot_MetaStateChanged(Phonon::State newState, Phonon::State /*oldState*/)
{
    //����״̬�����ý��Դ�б��г�ȥ����ӵ�ý��Դ
    if (newState == Phonon::ErrorState)
    {
        QMessageBox::warning(this, tr("���ļ�ʱ����!"), metaInformationResolver ->errorString());
        //takeLast()Ϊɾ�����һ�в����䷵��
        while (!sources.isEmpty() && !(sources.takeLast() == metaInformationResolver ->currentSource()))
        {

        }//ֻ�������һ��
        return;
    }

    //����Ȳ�����ֹͣ״̬Ҳ��������ͣ״̬����ֱ�ӷ���
    if (newState != Phonon::StoppedState && newState != Phonon::PausedState)
    {
        return;
    }

    //���ý��Դ���ʹ�����ֱ�ӷ���
    if (metaInformationResolver ->currentSource().type() == Phonon::MediaSource::Invalid)
    {
        return;
    }

    QMap<QString, QString> metadata = metaInformationResolver ->metaData();//��ȡý��Դ�е�Դ����

    //��ȡ�ļ�������Ϣ
    QString title;
    QString filePath = metaInformationResolver ->currentSource().fileName();
    if (!QFileInfo(filePath).baseName().contains(" - "))//û�а������ַ���
    {
        title = metadata.value("TITLE");
        if (title == "")
        {
            title = "unKown";
        }
    }
    else
    {
        QStringList str = QFileInfo(filePath).baseName().split(" - ");
        title = str.at(1);
    }
    QTableWidgetItem *titleItem = new QTableWidgetItem(title);
    titleItem ->setTextAlignment(Qt::AlignCenter);

    //��ȡ��������Ϣ
    QString artist = "unKown";
    if (!QFileInfo(filePath).baseName().contains(" - "))//û�а������ַ���
    {
        artist = metadata.value("ARTIST");
        if (artist == "")
        {
            artist = "unKown";
        }
    }
    else
    {
        QStringList str = QFileInfo(filePath).baseName().split(" - ");
        artist = str.at(0);
    }
    QTableWidgetItem *artistItem = new QTableWidgetItem(artist);
    artistItem ->setTextAlignment(Qt::AlignCenter);

    //Album��ȡר����Ϣ
    //QTableWidgetItem *albumItem = new QTableWidgetItem(metadata.value("DATE"));

    //��ȡ��ʱ����Ϣ
    qint64 totalTime = metaInformationResolver ->totalTime();
    QTime time(0, (totalTime / 60000) % 60, (totalTime / 1000) % 60);
    QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));
    timeItem ->setTextAlignment(Qt::AlignCenter);

    //���
    QTableWidgetItem *rowItem = new QTableWidgetItem;
    rowItem ->setTextAlignment(Qt::AlignCenter);

    //���벥���б�
    int currentRows = playList ->rowCount();//�����б��е�����
    playList ->insertRow(currentRows);//�Ӳ����б��еĵ�ǰ�в���
    rowItem ->setText(QString::number(currentRows + 1));
    playList ->setItem(currentRows, 0, rowItem);
    playList ->setItem(currentRows, 1, titleItem);
    playList ->setItem(currentRows, 2, artistItem);
    //playList ->setItem(currentRows, 2, albumitem);
    playList ->setItem(currentRows, 3, timeItem);

    //����ʱ���м��
    if (currentRows + 1 <= 12)
    {
        playList ->horizontalHeader() ->resizeSection(3, 80);
    }
    else
    {
        playList ->horizontalHeader() ->resizeSection(3, 65);
    }

    //��������Ϣ����vector
    QString musicInfoStr = title + "-" + artist;
    musicInfoMap.insert(make_pair(musicInfoStr, currentRows));

	//�������ݿ�
	DBModule::insertLine(title, artist, time.toString("mm:ss"), filePath);

    //sourcesΪ�򿪵�������Ƶ�ļ��б�playListΪ���ֲ����б������
    int index = sources.indexOf(metaInformationResolver ->currentSource()) + 1;
    if (sources.size() > index)//û�н�����
    {
        metaInformationResolver ->setCurrentSource(sources.at(index));
    }
    else
    {
        //û�б�ѡ�е���
        if (playList ->selectedItems().isEmpty())
        {
            //�������û�в��Ÿ��������õ�һ��ý��ԴΪý�����ĵ�ǰý��Դ
            //��Ϊ�������ڲ��Ÿ���ʱ����˲����б�Ȼ����������µ��б�
            if (mediaObject ->state() != Phonon::PlayingState
                    && mediaObject ->state() != Phonon::PausedState)
            {
                mediaObject ->setCurrentSource(sources.at(0));
                mediaObject ->play();
            }
            else
            {
                //������ڲ��Ÿ�������ѡ�в����б�ĵ�һ����Ŀ��������ͼ��״̬
                playList ->selectRow(0);
            }
        }
    }
}

//��С��������
void Widget::slot_MinToTray()
{
    if (musicListWidget ->isVisible())
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
    if (musicDownload ->isVisible())
    {
        QPropertyAnimation *musicDownloadAnimation = new QPropertyAnimation(musicDownload, "windowOpacity");
        musicDownloadAnimation ->setDuration(500);
        musicDownloadAnimation ->setStartValue(1);
        musicDownloadAnimation ->setEndValue(0);
        musicDownloadAnimation ->start();
        connect(musicDownloadAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicDownload()));
    }
    if (this ->isVisible())
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation ->setDuration(500);
        mainWidgetAnimation ->setStartValue(1);
        mainWidgetAnimation ->setEndValue(0);
        mainWidgetAnimation ->start();
        connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));
        trayIcon ->showMessage(tr("CZPlayer"), tr("���������»ص�������"));
    }
}

//�ر����г���
void Widget::slot_CloseAllProgress()
{
    //    mediaObject ->stop();
    //    sources.clear();

    QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
    mainWidgetAnimation ->setDuration(500);
    mainWidgetAnimation ->setStartValue(1);
    mainWidgetAnimation ->setEndValue(0);
    mainWidgetAnimation ->start();
    connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
    musicListAnimation ->setDuration(500);
    musicListAnimation ->setStartValue(1);
    musicListAnimation ->setEndValue(0);
    musicListAnimation ->start();
    connect(musicListAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *musicDownloadAnimation = new QPropertyAnimation(musicDownload, "windowOpacity");
    musicDownloadAnimation ->setDuration(500);
    musicDownloadAnimation ->setStartValue(1);
    musicDownloadAnimation ->setEndValue(0);
    musicDownloadAnimation ->start();
    connect(musicDownloadAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *lrcAnimation = new QPropertyAnimation(lrc, "windowOpacity");
    lrcAnimation ->setDuration(500);
    lrcAnimation ->setStartValue(1);
    lrcAnimation ->setEndValue(0);
    lrcAnimation ->start();
    connect(lrcAnimation, SIGNAL(finished()), this, SLOT(close()));

    minMusicWidget ->close();
    //    QPropertyAnimation *minWidgetAnimation = new QPropertyAnimation(minMusicWidget, "windowOpacity");
    //    minWidgetAnimation ->setDuration(500);
    //    minWidgetAnimation ->setStartValue(1);
    //    minWidgetAnimation ->setEndValue(0);
    //    minWidgetAnimation ->start();
    //    connect(minWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));
}

//ϵͳ����ͼ�걻����
void Widget::slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation ->setDuration(500);
        mainWidgetAnimation ->setStartValue(0);
        mainWidgetAnimation ->setEndValue(1);
        mainWidgetAnimation ->start();
        this ->show();//��ʾ������
        this ->slot_ShowMusicPanel();
    }
}

//��С��
void Widget::slot_ShowMinSize()
{
    this ->showMinimized();
    musicListWidget ->showMinimized();
    musicDownload ->showMinimized();
}

//��ʾ����Ѿ�������Ϣ
void Widget::slot_ShowLrcLocked()
{
    trayIcon ->showMessage(tr("��ʾ"), tr("����������Ѿ�����,���������ͼ���Ҽ����Խ���"));
}

//��ʾ����Ѿ�������Ϣ
void Widget::slot_ShowLrcUnlocked()
{
    trayIcon ->showMessage(tr("��ʾ"), tr("����������Ѿ�����"));
}

//�������
void Widget::slot_UnlockLrc()
{
    lrc ->unlockLrc();//�������
}

////��λ����ǰ����
//void Widget::locateCurrentMusic()
//{
//    int index = sources.indexOf(mediaObject ->currentSource());
//    playList ->selectRow(index);
//}

void Widget::slot_ShortCutsetDisable()//����ȫ���ȼ�����/����
{
	if (minMusicWidget ->isHidden())
	{
		//���������ȼ�
		GlobalHotKey::showHideMainWidgetDisabled(false);
		GlobalHotKey::showDownloadDisabled(false);
		GlobalHotKey::showMusicListDisabled(false);
	}
	else
	{
		//���������ȼ�
		GlobalHotKey::showHideMainWidgetDisabled(true);
		GlobalHotKey::showDownloadDisabled(true);
		GlobalHotKey::showMusicListDisabled(true);
	}
}

//����������Ҽ�����¼�
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(closeAction);
    menu.addAction(minAction);
    menu.addAction(QIcon(":/images/minMainWidget.png"), tr("����С����"), this, SLOT(slot_OpenMinWidget()));
    menu.addSeparator();
    menu.addAction(openFileAction);
    menu.addAction(openMusicListAction);
    menu.addAction(openMusicDownloadAction);
    menu.addAction(openLrcAction);
    menu.addSeparator();
    menu.addAction(configAction);
	menu.addAction(aboutAction);
    menu.addAction(aboutQtAction);
    menu.exec(event ->globalPos());//globalPos()Ϊ��ǰ����λ������
    //menu.exec(QCursor::pos());
    event ->accept();
}

//�Ͻ��¼�
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event ->mimeData() ->urls();
    foreach (QUrl url, urls)
    {
        QString filePath = url.toLocalFile();
        QStringList fileList = filePath.split('.');
        QString kzName = fileList.at(1);
        if (kzName == "mp3" || kzName == "MP3" || kzName == "wav" || kzName == "WAV"
                || kzName == "wma" || kzName == "WMA" || kzName == "ogg" || kzName == "OGG")
        {
            event ->accept();
        }
        else
        {
            event ->ignore();
        }
    }
}

//�����¼�
void Widget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event ->mimeData() ->urls();
    if (urls.isEmpty())
    {
        return;
    }

    //��ȡ��ǰý��Դ�б�Ĵ�С
    int index = sources.size();

    foreach (QUrl url, urls)
    {
        QString filePath = url.toLocalFile();
        Phonon::MediaSource source(filePath);//��ÿһ�����ּ��س�һ��Դ
        sources.append(source);//��ÿһ��Դ��ӵ�ý����Դ�б�
    }

    if (!sources.isEmpty())
    {
        //���ý��Դ�б�Ϊ�գ����¼���ĵ�һ��ý��Դ��Ϊ��ǰý��Դ
        metaInformationResolver ->setCurrentSource(sources.at(index));
    }

    int static count = 0;
    ++count;
    if (count == 1 && readyReadDb == 0)//��һ�δ��ļ�����û�д����ݿ��ж�ȡ
    {
        mediaObject ->setCurrentSource(sources.at(0));
        mediaObject ->play();
    }
    event ->accept();
}

//��дpaintEvent,��ӱ���ͼƬ
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/images/mainBg3.png");

    //��ͨ��pix�ķ������ͼƬ�Ĺ��˵�͸���Ĳ��ֵõ���ͼƬ����ΪloginPanel�Ĳ�����߿�
    this ->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 212, backgroundImage);

	////���ұ�
	//static const int sineTable[16] = { 0, 38, 71, 92, 100, 92, 71, 38,	0, -38, -71, -92, -100, -92, -71, -38};

	//QFontMetrics metrics(font());	//����ָ��
	//int x = (430 - metrics.width(lrcText)) / 2;
	//int y = (250 + metrics.ascent() - metrics.descent()) / 2;//���� - ����
	//QColor color;

	//for (int i = 0; i < lrcText.size(); ++i)
	//{
	//	int index = (step + i) % 16;
	//	color.setHsv((15 - index) * 16, 255, 191);
	//	painter.setPen(color);
	//	painter.drawText(x, y - ((sineTable[index] * metrics.height()) / 400), QString(lrcText[i]));
	//	x += metrics.width(lrcText[i]);
	//}

    event ->accept();
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event ->button() == Qt::LeftButton)
    {
        dragPosition = event ->globalPos() - frameGeometry().topLeft();
        event ->accept();
    }
}

//��дmousePressEvent��mouseMoveEventʵ�ִ�����ƶ�
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event ->buttons() == Qt::LeftButton)
    {
        this ->move(event ->globalPos() - dragPosition);//�ƶ�����
        musicListWidget ->move(frameGeometry().bottomLeft());//�ƶ������ʱ�����ֲ����б�����ƶ�
        musicDownload ->move(frameGeometry().topRight());//�ƶ������ʱ���������ؽ�������ƶ�
        event ->accept();
    }
}

//ʱ���¼�
//void Widget::timerEvent( QTimerEvent *event )
//{
//	if (event ->timerId() == baseTimer.timerId())
//	{
//		++step;
//		update();
//	}
//	else
//	{
//		QWidget::timerEvent(event);
//	}
//}

//����豸�Ĳ���
//void Widget::slotDeviceAdded( QString str )
//{
//	QDBusInterface device("org.freedesktop.Hal", str, "org.freedesktop.Hal.Device", QDBusConnection::systemBus());
//}

//����豸�Ĳ���
//void Widget::slotDeviceRemoved( QString str )
//{
//
//}
