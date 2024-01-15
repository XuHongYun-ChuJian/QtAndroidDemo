#ifndef JAVAUTILS_H
#define JAVAUTILS_H

#include <QObject>
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>

class JavaUtils : public QObject
{
    Q_OBJECT
public:
    explicit JavaUtils(QObject *parent = nullptr);
    ~JavaUtils();

private:
    QJniObject javaClass;

signals:

public slots:
    void fullScreenStickyImmersive();
    void doreboot();
    void doshutdown();
    void getsu();
    void requestPermission();
    void runApp(QString packetname,QString activitypage);

    QString getUSBInfo();
    QString getVolumeInfo();
    QString getUSBPath();

    //deprecation
    void checkStoragePermission();

    void requestExternalStoragePermission();

    void testFunction();

};

#endif // JAVAUTILS_H
