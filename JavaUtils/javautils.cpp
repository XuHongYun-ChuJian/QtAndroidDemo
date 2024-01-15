#include "javautils.h"
#include <QCoreApplication>
#include <QJniObject>
#include <jni.h>

#define JFS_CLASSNAME "org/qtproject/example/utils/JavaUtils"
#define JFS_SIGNTURE_INSTANCE "()Lorg/qtproject/example/utils/JavaUtils;"

void realFullScreenStickyImmersiveMethod() {
    QJniObject javaCustomFunction = QJniObject::callStaticObjectMethod(JFS_CLASSNAME, "instance", JFS_SIGNTURE_INSTANCE);
    javaCustomFunction.callMethod<void>("fullScreenStickyImmersive", "()V");
}

JavaUtils::JavaUtils(QObject *parent) : QObject(parent)
{
    javaClass = QJniObject::callStaticObjectMethod(JFS_CLASSNAME, "instance", JFS_SIGNTURE_INSTANCE);
    javaClass.callMethod<void>("setContext", "(Landroid/content/Context;)V", QtAndroidPrivate::activity());
}

JavaUtils::~JavaUtils()
{

}


/********************************************************************************************
    全屏
********************************************************************************************/
void JavaUtils::fullScreenStickyImmersive()
{
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]() {
        realFullScreenStickyImmersiveMethod();
    });
}


/********************************************************************************************
    重启
********************************************************************************************/
void JavaUtils::doreboot()
{
    javaClass.callMethod<void>("doreboot");
}


/********************************************************************************************
    关机
********************************************************************************************/
void JavaUtils::doshutdown()
{
    javaClass.callMethod<void>("doshutdown");
}


/********************************************************************************************
    获取权限
********************************************************************************************/
void JavaUtils::getsu()
{
    javaClass.callMethod<void>("getsu");
    requestPermission();
}

//申请权限
void JavaUtils::requestPermission()
{
    // 请求读取外部存储权限
    QtAndroidPrivate::requestPermission("android.permission.READ_EXTERNAL_STORAGE");
//    QtAndroidPrivate::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
//    QtAndroidPrivate::requestPermission("android.permission.INTERNET");
}



/********************************************************************************************
    run app
********************************************************************************************/
void JavaUtils::runApp(QString packetname,QString activitypage)
{
    QJniObject java_packetname= QJniObject::fromString(packetname);
    QJniObject java_activitypage = QJniObject::fromString(activitypage);
    javaClass.callMethod<void>("runApp",java_packetname.object<jstring>(),java_activitypage.object<jstring>());
}

QString JavaUtils::getUSBInfo()
{
    QJniObject result = javaClass.callObjectMethod("getUSBInfo", "()Ljava/lang/String;");
    if (result.isValid()) {
        // 将返回的字符串转换为 QString
        return result.toString();
    }
    return {};
}

QString JavaUtils::getVolumeInfo()
{
    QJniObject result = javaClass.callObjectMethod("getVolumeInfo", "()Ljava/lang/String;");
    if (result.isValid())
    {
        return result.toString();
    }
    return {};
}

QString JavaUtils::getUSBPath()
{
    QJniObject result = javaClass.callObjectMethod("getUSBPath", "()Ljava/lang/String;");
    if (result.isValid())
    {
        return result.toString();
    }
    return {};
}

void JavaUtils::checkStoragePermission()
{
    javaClass.callMethod<void>("checkStoragePermission");
}

void JavaUtils::requestExternalStoragePermission()
{
    javaClass.callMethod<void>("requestExternalStoragePermission");
}

void JavaUtils::testFunction()
{
    // 调用Java方法，返回String[]
    QJniObject resultArray = javaClass.callObjectMethod("getStringArray", "()[Ljava/lang/String;");

    // 检查是否有错误
    if (resultArray.isValid()) {
        // 处理返回的String[]
        jint length = resultArray.getField<jint>("length");
        for (int i = 0; i < length; ++i) {
            QJniObject element = resultArray.callObjectMethod("get", "(I)Ljava/lang/Object;", i);
            QString stringValue = element.toString();
            qDebug() << "Element at index" << i << ":" << stringValue;
        }
    } else {
        qDebug() << "Error calling Java method";
    }
}




