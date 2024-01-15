#include "widget.h"
#include "ui_widget.h"

#include <QPixmap>
#include <QPainter>
#include <QPrinter>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopServices>
#include <QProgressDialog>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QDebug>
#include <QStandardPaths>

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

#define PACKAGE_NAME "package:org.company.package_name"

const char* StoragePath = "/storage";
const char* USBPath = "/storage/usbhost1";
const char* RootPath = "/storage/emulated/0";
const int FilePathRole = Qt::UserRole + 1;

bool requestPermission(const QString& permission)
{
#ifdef Q_OS_ANDROID
    auto result = QtAndroidPrivate::requestPermission(permission);
    result.waitForFinished();
    auto permissionResult = result.result();
    switch (permissionResult) {
    case QtAndroidPrivate::PermissionResult::Authorized:
        qDebug()<< permission << "permission Authorized";
        break;
    case QtAndroidPrivate::PermissionResult::Denied:
        qDebug()<< permission << "permission Denied";
        break;
    case QtAndroidPrivate::PermissionResult::Undetermined:
        qDebug()<< permission << "permission Undetermined";
        break;
    default:
        break;
    }
    return permissionResult == QtAndroidPrivate::PermissionResult::Authorized;
#endif
}

bool checkPermisson(const QString& permission)
{
#ifdef Q_OS_ANDROID
    auto result = QtAndroidPrivate::checkPermission(permission);
    result.waitForFinished();
    auto permissionResult = result.result();
    switch (permissionResult) {
    case QtAndroidPrivate::PermissionResult::Authorized:
        qDebug()<< permission << "permission Authorized";
        break;
    case QtAndroidPrivate::PermissionResult::Denied:
        qDebug()<< permission << "permission Denied";
        break;
    case QtAndroidPrivate::PermissionResult::Undetermined:
        qDebug()<< permission << "permission Undetermined";
        break;
    default:
        break;
    }
#endif
    return permissionResult == QtAndroidPrivate::PermissionResult::Authorized;
}

bool requestExternalStoragePermission()
{
#ifdef Q_OS_ANDROID
    bool value = QJniObject::callStaticMethod<jboolean>("android/os/Environment", "isExternalStorageManager");
    if(value == false){
        QJniObject filepermit = QJniObject::getStaticObjectField(
            "android/provider/Settings",
            "ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION",
            "Ljava/lang/String;");
        //you must changing the YOURPKGNAME at below
        QJniObject pkgName = QJniObject::fromString("package:org.qtproject.example");
        QJniObject parsedUri = QJniObject::callStaticObjectMethod(
            "android/net/Uri",
            "parse","(Ljava/lang/String;)Landroid/net/Uri;",
            pkgName.object<jstring>());
        QJniObject intent("android/content/Intent",
                          "(Ljava/lang/String;Landroid/net/Uri;)V",
                          filepermit.object<jstring>(),parsedUri.object());
        QtAndroidPrivate::startActivity(intent, 0);
    }
#endif
    return true;
}

bool pictureToPDF(const QStringList& picturePaths, const QString& pdfPath)
{
    QProgressDialog progress("转换中", "取消", 0, picturePaths.count(), nullptr);
    progress.setCancelButton(nullptr);
    progress.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    progress.setWindowModality(Qt::WindowModal);
    // 创建一个QPrinter对象，设置输出格式为PDF，分辨率为高清，纸张大小为A4
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    // 设置PDF的保存路径和文件名
    printer.setOutputFileName(pdfPath);

    QPainter painter(&printer);
    int rows = 5;
    int cols = 5;
    QSize pdfSize = printer.pageRect(QPrinter::Unit::DevicePixel).size().toSize();
    QSize pictureSize(pdfSize.width() / cols, pdfSize.height() / rows);

    int pictureCount = picturePaths.count();
    int row = 0, col = 0;
    int x = 0, y = 0;
    int pageNo = 0;
    for(int i = 0; i < pictureCount; ++i)
    {
        progress.setValue(i);
        if (row >= rows)
        {
            printer.newPage();
            col = 0;
            row = 0;
            x = 0;
            y = 0;
            pageNo++;
        }
        auto picturePath = picturePaths.at(i);
        if (QImage(picturePath).isNull())
            continue;

        // 加载图片，缩放到合适的大小，绘制到QPrinter上
        QPixmap pixmap(picturePath);

        pixmap = pixmap.scaled(pictureSize,
                               Qt::AspectRatioMode::KeepAspectRatio,
                               Qt::FastTransformation);

        QColor color = QColor::fromRgb(rand() % 256, rand() % 256, rand() % 256);
        painter.setBrush(color);
        QRect rect(x, y, pictureSize.width(), pictureSize.height());
        QFont font = painter.font();
        font.setPixelSize(300);
        painter.setFont(font);
//        painter.drawRect(rect);
        painter.drawPixmap(rect, pixmap);
        painter.drawText(rect, Qt::AlignCenter, QString("%1 * %2:%3").arg(row).arg(col).arg(row * col * (pageNo + 1)));

        col++;
        if (col >= cols)
        {
            col = 0;
            row++;
        }
        x = col * pictureSize.width();
        y = row * pictureSize.height();
    }
    painter.end();

    progress.setValue(pictureCount);
    return true;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&LogManager::getInstance(), &LogManager::newLog, this, &Widget::addLog);

    requestPermission("android.permission.READ_EXTERNAL_STORAGE");
    requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    requestPermission("android.permission.INTERNET");

//    requestExternalStoragePermission();

    ui->textBrowser->append(QString("应用程序运行路径：%1").arg(qApp->applicationDirPath()));
    ui->textBrowser->append(QString("应用程序名称：%1").arg(qApp->applicationName()));

    addLog("QDir::currentPath() = " + QDir::currentPath());

    initDirCombobox();
    initPermissonComboBox();

//    qDebug()<< m_javaUtils.getUSBInfo();

//    m_javaUtils.checkStoragePermission();
//    m_javaUtils.testFunction();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addLog(const QString &log)
{
    ui->textBrowser->append(log);
}


void Widget::on_btnSelect_clicked()
{
    m_filePaths = QFileDialog::getOpenFileNames(this,
                                                "选择图像文件",
                                                "D://",
                                                "Images (*.png *.jpg *.bmp)");
}


void Widget::on_btnConvert_clicked()
{
    QString pdfPath = QFileDialog::getSaveFileName(nullptr, "保存PDF文件", "", "PDF Files (*.pdf)");
    if (pictureToPDF(m_filePaths, pdfPath))
    {
        QDesktopServices::openUrl(QUrl(pdfPath));
    }
    else
    {
        QMessageBox::information(this, "提示", "pdf文件打开失败");
    }
    ui->textBrowser->append("创建的Pdf文件：" + pdfPath);
}


void Widget::on_btnSelectDir_clicked()
{
    QString dirPath = "C://";
#ifdef Q_OS_ANDROID
    dirPath = QFileDialog::getExistingDirectory(this, "选择路径");
#endif
    ui->textBrowser->append("当前选择的文件路径：" + dirPath);
    setDataFileListWidget(dirPath);
}


void Widget::on_btnQuit_clicked()
{
    auto ret = QMessageBox::information(this, "提示", "是否退出?" , QMessageBox::Ok | QMessageBox::No);
    if (ret == QMessageBox::No)
        return;
    else
        qApp->quit();
}


void Widget::on_btnAssignDir_clicked()
{
    QString dirSuffix = "/storage/emulated/0";
    QString currentDir = ui->comboBox->currentText();
    QString dirPath = dirSuffix + "/" + currentDir;
    setDataFileListWidget(dirPath);
}

void Widget::initDirCombobox()
{
    ui->comboBox->clear();
    QDir dir(RootPath);
    QFileInfoList fileInfos = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for(auto const& fileInfo : fileInfos)
    {
        ui->comboBox->addItem(fileInfo.fileName());
    }
}

void Widget::initPermissonComboBox()
{
    QStringList permissons;
    permissons << "android.permission.CAMERA" << "android.permission.MANAGE_EXTERNAL_STORAGE"
               << "android.permission.WRITE_EXTERNAL_STORAGE" << "android.permission.READ_EXTERNAL_STORAGE";
    ui->comboBoxPermisson->addItems(permissons);
}

void Widget::setDataFileListWidget(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
    {
        addLog(QString("%1目录打开失败").arg(dirPath));
        return;
    }
    addLog(QString("%1目录打开成功").arg(dirPath));
    ui->listWidget->clear();
    QFileInfoList fileInfos = dir.entryInfoList(QDir::Files /*| QDir::Dirs*/ | QDir::NoDotAndDotDot, QDir::Name);
    foreach(const auto& fileInfo, fileInfos)
    {
        auto item = new QListWidgetItem;
        item->setText(fileInfo.fileName());
        item->setData(FilePathRole, fileInfo.filePath());
        ui->listWidget->addItem(item);
    }
}

void Widget::on_btnDelete_clicked()
{
    auto item = ui->listWidget->currentItem();
    if (!item)
        return;

    auto ret = QMessageBox::information(this, "提示", "确认删除选中的文件吗?" , QMessageBox::Ok | QMessageBox::No);
    if (ret == QMessageBox::No)
        return;

    QString filePath = item->data(FilePathRole).toString();
    addLog("当前删除的文件路径：" + filePath);
    QFile file(filePath);
    if (file.exists())
    {
        bool ok = file.remove();
        addLog(QString("%1删除%2").arg(item->text()).arg(ok ? "成功":"失败"));
    }
    else
    {
        addLog(item->text() + "不存在");
    }
}


void Widget::on_btnRequestPermisson_clicked()
{
    QString permisson = ui->comboBoxPermisson->currentText();

    if (!checkPermisson(permisson))
        requestPermission(permisson);
}


void Widget::on_btnSaveLog_clicked()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd.log");
    QString fileDir = QString("%1/log").arg(RootPath);
    QString filePath = fileDir + "/" + fileName;

    QDir dir(fileDir);
    if (!dir.exists())
    {
        if (dir.mkdir(fileDir))
        {
            addLog(fileDir + "创建成功");
        }
        else
        {
            addLog(fileDir + "创建失败");
            return;
        }
    }

    addLog(filePath);
    QFile file(filePath);
    if (!file.open(QFile::ReadWrite | QFile::Text | QFile::Truncate))
    {
        addLog(QString("%1打开失败%2").arg(filePath, file.errorString()));
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    in << ui->textBrowser->toPlainText();
    file.close();
    addLog(QString("%1保存成功").arg(filePath));
}


void Widget::on_btnPressionFileAll_clicked()
{
    m_javaUtils.requestExternalStoragePermission();
}


void Widget::on_btnCopyFile_clicked()
{
    auto item = ui->listWidget->currentItem();
    if (!item)
        return;

    QString oldName = item->data(FilePathRole).toString();
    QString fileName = item->text();

    QString desPath = m_javaUtils.getUSBPath() + "/temp";
    QString newFileName = desPath + "/" + fileName;
    QDir dir(desPath);
    if (!dir.exists())
    {
        if (dir.mkdir(desPath))
        {
            addLog(desPath + " dir make success.");
        }
        else
        {
            addLog(desPath + " dir make failed.");
            return;
        }
    }

    if (QFile::exists(newFileName))
    {
        addLog(newFileName + " already exists.");
        return;
    }

    if (QFile::copy(oldName, newFileName))
    {
        QString msg = QString("%1 Copy Success.").arg(oldName);
        addLog(msg);
        msg = QString("目的路径：" + desPath);
        addLog(msg);
    }
    else
    {
        QString msg = QString("%1 Copy Failed.").arg(oldName);
        addLog(msg);
    }
}


void Widget::on_btnCheckUSB_clicked()
{
//    QDir dir(ui->lineEdit->text());
//    addLog(QString("%1USB路径%2 ").arg(ui->lineEdit->text()).arg(dir.exists() ? "存在" : "不存在"));

    addLog(m_javaUtils.getUSBInfo());
    addLog(m_javaUtils.getVolumeInfo());
}

