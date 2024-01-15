#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#ifdef Q_OS_ANDROID
#include "JavaUtils/javautils.h"
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class LogManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(LogManager)
    LogManager(){}
public:
    ~LogManager(){}

    static LogManager& getInstance()
    {
        static LogManager instance;
        return instance;
    }

signals:
    void newLog(const QString& msg);
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void addLog(const QString& log);

private slots:
    void on_btnSelect_clicked();

    void on_btnConvert_clicked();

    void on_btnSelectDir_clicked();

    void on_btnQuit_clicked();

    void on_btnAssignDir_clicked();

    void on_btnDelete_clicked();

    void on_btnRequestPermisson_clicked();

    void on_btnSaveLog_clicked();

    void on_btnPressionFileAll_clicked();

    void on_btnCopyFile_clicked();

    void on_btnCheckUSB_clicked();

private:
    void initDirCombobox();
    void initPermissonComboBox();
    void setDataFileListWidget(const QString& dirPath);
private:
    Ui::Widget *ui;

    QStringList m_filePaths;

#ifdef Q_OS_ANDROID
private:
    JavaUtils m_javaUtils;
#endif
};
#endif // WIDGET_H
