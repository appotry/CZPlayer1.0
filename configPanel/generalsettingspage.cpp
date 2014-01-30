#include "stdafx.h"
#include "generalsettingspage.h"

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *startGB = new QGroupBox(tr("����ʱ"));
    QCheckBox *autoPlayCB = new QCheckBox(tr("�Զ����Ÿ���"));
    QCheckBox *autoStartCB = new QCheckBox(tr("����ʱ�Զ�����CZPlayer"));
    QCheckBox *helloCZPlayerCB = new QCheckBox(tr("���š�Hello CZPlayer���ʺ���"));
    QCheckBox *preWidgetCB = new QCheckBox(tr("�ָ��ϴιر�ʱ�Ľ���"));

    QGroupBox *fileConnect = new QGroupBox(tr("�ļ�����"));
    QCheckBox *defaultPlayer = new QCheckBox(tr("��CZPlayer��ΪĬ�ϲ�����"));

    QGridLayout *startGBLayoutG = new QGridLayout;
    startGBLayoutG ->addWidget(autoPlayCB, 0, 0);
    startGBLayoutG ->addWidget(autoStartCB, 0, 1);
    startGBLayoutG ->addWidget(helloCZPlayerCB, 1, 0);
    startGBLayoutG ->addWidget(preWidgetCB, 1, 1);
    startGB ->setLayout(startGBLayoutG);

    QVBoxLayout *fileLayout = new QVBoxLayout;
    fileLayout ->addWidget(defaultPlayer);
    fileConnect ->setLayout(fileLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout ->addWidget(startGB);
    mainLayout ->addWidget(fileConnect);
    mainLayout ->addStretch();
    this ->setLayout(mainLayout);
}

GeneralSettingsPage::~GeneralSettingsPage()
{
}
