#include "StdAfx.h"
#include "AboutPage.h"


AboutPage::AboutPage(void)
{
	strLabel = new QLabel;
	strLabel ->setObjectName("strLabel");
	strLabel ->setText(tr("����Qt�Ķ�ý�岥������CZPlayer��"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout ->addWidget(strLabel);
	mainLayout ->addStretch();
	this ->setLayout(mainLayout);
}


AboutPage::~AboutPage(void)
{
}
