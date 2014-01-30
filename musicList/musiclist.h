#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "head.h"

class MusicList : public QTableWidget
{
    Q_OBJECT

public:
    MusicList(Phonon::MediaObject *mediaObject,
              QList<Phonon::MediaSource> *sources,
              QWidget *widget = 0);
    ~MusicList();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_PlayListClean();
    void sig_RowSelected(int row, QString title);

private slots:
    void slot_ClearAllMusicList();//��ո���
    void slot_DelCurrentMusic();//ɾ����ǰ����
    void slot_TableClicked(int index);//��ȡѡ����
    void slot_LocateCurrentMusic();//��λ����ǰ����
	void slot_SaveList();//���浱ǰ�б�

private:
    void updateMusicList();//���²����б����

private:
    QPoint dragPosition;
    QWidget *parent;
    QAction *clearMusicList;
    QAction *closeAction;
    QAction *delCurrentMusicAction;
	QAction *saveListAction;
    int row;

    Phonon::MediaObject *m_mediaObject;
    QList<Phonon::MediaSource> *m_sources;
};

#endif // MUSICLIST_H
