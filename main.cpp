#include "widget.h"
#include <QtGlobal>
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QApplication>
#define DT_FORMAT_YMDHMSZ "yyyy-MM-dd hh:mm:ss zzz"
#define CURRENT_DT QString("[%1]").arg(QDateTime::currentDateTime().toString(DT_FORMAT_YMDHMSZ))
#define CURRENT_TR QString("[%1]").arg(quintptr(QThread::currentThread()))

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *function = context.function ? context.function : "";
    QString typeInfo = "";
    switch (type) {
    case QtDebugMsg:
        typeInfo = "[Debug]";
        break;
    case QtInfoMsg:
        typeInfo = "[Info]";
        break;
    case QtWarningMsg:
        typeInfo = "[Warn]";
        break;
    case QtCriticalMsg:
        typeInfo = "[Crit]";
        break;
    case QtFatalMsg:
        typeInfo = "[Fatal]";
        break;
    }
    QString typeStr = QString("%1").arg(typeInfo , -7 , QChar(' '));
    fprintf(stderr, "%s [%llu] %s %s:%u %s\n", CURRENT_DT.toLocal8Bit().constData(),
            quintptr(QThread::currentThread()),
            typeStr.toLocal8Bit().constData(),
            function ,
            context.line , localMsg.constData());
    QString logMsg = QString("%1 %2 %3:%4 %5").arg(CURRENT_DT.toLocal8Bit().constData()).arg(typeStr.toLocal8Bit().constData())
                         .arg(function).arg(context.line).arg(msg);
    LogManager::getInstance().newLog(logMsg);
}
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qInstallMessageHandler(myMessageOutput);
    setbuf(stderr , NULL);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
