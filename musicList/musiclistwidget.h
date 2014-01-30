#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H


#include "musiclist.h"
#include "head.h"

class MusicListWidget : public QWidget
{
    Q_OBJECT

public:
    MusicListWidget(MusicList *playList,
                    map<QString, int> *musicInfoMap,
                    QWidget *parent = 0);
    ~MusicListWidget();

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

signals:
    void sig_AddMusic();
	void sig_AddMusicList();

private slots:
    void slot_OnClickedAddMusic();
    void slot_SearchMusic();
    void slot_LineEditFocus();
    void slot_BackMusicList();
	void slot_AddMusic();
	void slot_AddMusicList();

private:
    QWidget *m_parent;
    MusicList *m_playList;
    QTimer *timer;
    map<QString, int> *m_musicInfoMap;

    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *addMusicButton;
    QPushButton *localeButton;
    QPushButton *delMusicButton;
    QPushButton *findButton;
    QPushButton *backButton;

	QAction *addMusicAction;
	QAction *addMusicListAction;
};

#endif // MUSICLISTWIDGET_H
