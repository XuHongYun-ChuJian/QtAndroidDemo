#include "JavaDocumentUtils.h"

#define JFS_CLASSNAME "org/qtproject/example/utils/JavaDocumentUtils"
#define JFS_SIGNTURE_INSTANCE "()Lorg/qtproject/example/utils/JavaDocumentUtils;"


JavaDocumentUtils::JavaDocumentUtils(QObject *parent)
    : QObject{parent}
{
    QJniEnvironment env;
    QJniObject context = QtAndroidPrivate::activity();
    // 调用静态方法
    QJniObject result = QJniObject::callStaticObjectMethod(
        JFS_CLASSNAME, // Java类的全名
        "getExtSdCardPaths", // 方法名
        "(Landroid/content/Context;)[Ljava/lang/String;", // 方法签名
        context.object<jobject>() // 参数
        );

    if (result.isValid()) {
        jobjectArray array = result.object<jobjectArray>();
        jsize len = env->GetArrayLength(array);
        for (jsize i = 0; i < len; ++i) {
            jstring str = (jstring) (env->GetObjectArrayElement(array, i));
            const char *rawString = env->GetStringUTFChars(str, 0);
            qDebug()<< rawString;
            // 在这里使用rawString
            env->ReleaseStringUTFChars(str, rawString);
        }
    }
}
