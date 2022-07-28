// EZX
#include <ZApplication.h>
#include <ZMainWidget.h>

#include <ezxutilcst.h>

// Qt
#include <qcheckbox.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qwidget.h>

// C
#include <cstdlib>

// POSIX
#include <unistd.h>

// Defines
#define LOGO_FILENAME "aw_banner.jpg"
#define EXEC_FILENAME "raw.ezx"

class Widget : public QWidget {
	Q_OBJECT

	QCheckBox *checkBoxSound;
	QCheckBox *checkBoxStretch;

public:
	Widget(QWidget *parent = 0, const char *name = 0) : QWidget(parent, name) {
		QString fileName = QString("%1/%2").arg(QFileInfo(qApp->argv()[0]).dirPath(true)).arg(LOGO_FILENAME);
		QPixmap banner(200, 100);
		banner.load(fileName);
		QLabel *labelBanner = new QLabel(this);
		labelBanner->setPixmap(banner);

		checkBoxSound = new QCheckBox(tr("Sound"), this);
		checkBoxSound->setChecked(true);
		checkBoxStretch = new QCheckBox(tr("Fullscreen"), this);
		checkBoxStretch->setChecked(true);

		QLabel *labelSign = new QLabel(tr("Limows & EXL, 2022"), this);

		QBoxLayout *layout = new QVBoxLayout(this, 1, 5);
		layout->addWidget(labelBanner, 0, Qt::AlignHCenter);
		layout->addWidget(checkBoxSound);
		layout->addWidget(checkBoxStretch);
		layout->addStretch();
		layout->addWidget(labelSign, 0, Qt::AlignRight);
	}

	bool isSoundEnabled(void) const { return checkBoxSound->isChecked(); }
	bool isFullScreenEnabled(void) const { return checkBoxStretch->isChecked(); }
};

class MainWidget : public ZMainWidget {
	Q_OBJECT

	Widget *widget;

public:
	MainWidget() : ZMainWidget(tr(" Another World Launcher "), false, NULL, NULL, 0) {
		widget = new Widget(this, NULL);
		setContentWidget(widget);

		UTIL_CST *cst = new UTIL_CST(this, tr("Run Game"));
		connect(cst->getMidBtn(), SIGNAL(clicked()), this, SLOT(run()));
		connect(cst->getRightBtn(), SIGNAL(clicked()), qApp, SLOT(quit()));
		cst->getLeftBtn()->setEnabled(false);
		setCSTWidget(cst);
	}

private slots:
	void run(void) {
		if (!widget->isSoundEnabled())
			setenv("RAW_NO_SOUND", "1", 1);
		if (!widget->isFullScreenEnabled())
			setenv("RAW_NO_STRETCH", "1", 1);

		if (!fork()) {
			const char *const argvs[] = { EXEC_FILENAME, NULL };
			execve(EXEC_FILENAME, const_cast<char * const *>(argvs), environ);
		}
		qApp->quit();
	}
};

int main(int argc, char *argv[]) {
	ZApplication application(argc, argv);
	MainWidget mainWidget;
	application.showMainWidget(&mainWidget);
	return application.exec();
}

#include "EzxGameLauncher.moc"
