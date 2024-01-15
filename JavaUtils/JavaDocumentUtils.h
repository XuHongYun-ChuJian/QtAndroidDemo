#ifndef JAVADOCUMENTUTILS_H
#define JAVADOCUMENTUTILS_H

#include <QObject>
#include <QtCore/private/qandroidextras_p.h>

class JavaDocumentUtils : public QObject
{
    Q_OBJECT
public:
    explicit JavaDocumentUtils(QObject *parent = nullptr);

signals:

private:
    QJniObject m_javaClass;

};

#endif // JAVADOCUMENTUTILS_H
