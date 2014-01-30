#ifndef MINMUSICWIDGET_H
#define MINMUSICWIDGET_H

#include "head.h"

class MinMusicWidget : public QWidget
{
    Q_OBJECT

public:
    MinMusicWidget(Phonon::MediaObject *mediaObject,
                   QList<Phonon::MediaSource> *sources,
                   QWidget *parent = 0);
    ~MinMusicWidget();

    QPushButton *minPlayButton;
    QPushButton *minPauseButton;
    QPushButton *minStopButton;
    QPushButton *minNextButton;
    QPushButton *minLastButton;
    QPushButton *minOpenFileButton;
    QAction *minMusicLrcAction;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_MinPlay();
    void sig_MinPause();
    void sig_MinStop();
    void sig_MinNext();
    void sig_MinLast();
    void sig_OpenFileButtonClicked();
    void sig_MusicLrc();
    void sig_ShowMusicPanel();

private slots:
    void slot_SetPlay();//����
    void slot_SetPaused();//��ͣ
    void slot_SetStop();//ֹͣ
    void slot_SetPre();//��һ��
    void slot_SetNext();//��һ��
    void slot_OpenFile();//�������ļ�
    void slot_OpenMainWidget();//��������
    void slot_OpenMusicLrc();//������
    void slot_HideMinWidget();//����С����

private:
    QWidget *m_parent;
    QPoint dragPosition;

    Phonon::MediaObject *m_mediaObject;//����ý�����
    QList<Phonon::MediaSource> *m_sources;//���ý����Դ
};

#endif // MINMUSICWIDGET_H
