#include <ZApplication.h>
#include <ZMainWidget.h>

#include <qwidget.h>

#include <ezxutilcst.h>

#if 0
#include <ezxres.h>

#include <qfileinfo.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qtextcodec.h>
#include <qtimer.h>
#endif

class Widget : public QWidget {
	Q_OBJECT

public:
	Widget(QWidget *parent = 0, const char *name = 0) : QWidget(parent, name) { }
	~Widget() { }
};

class MainWidget : public ZMainWidget {
	Q_OBJECT

	Widget *widget;

public:
	MainWidget() : ZMainWidget(" Another World Launcher ", false, NULL, NULL, 0) {
		widget = new Widget(this, NULL);
		setContentWidget(widget);

		UTIL_CST *cst = new UTIL_CST(this, "Run Game");
		connect(cst->getMidBtn(), SIGNAL(clicked()), widget, SLOT(run()));
		connect(cst->getRightBtn(), SIGNAL(clicked()), qApp, SLOT(quit()));
		cst->getLeftBtn()->setEnabled(false);
		setCSTWidget(cst);
	}

private slots:
	void run(void) {

	}
};

int main(int argc, char *argv[]) {
	ZApplication application(argc, argv);
	MainWidget mainWidget;
	application.showMainWidget(&mainWidget);
	return application.exec();
}

#include "EzxGameLauncher.moc"
