#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QNetworkAccessManager *netManager= nullptr;
    QNetworkReply *netreplay= nullptr;
    QNetworkReply *repoReply= nullptr;
    QByteArray dataBuffer;
    QPixmap *img= nullptr;

private slots:
    void on_pUserName_clicked();
    void readData();
    void finishReading();
    void readDataForrepo();
    void finishGettingRepo();

private:
    Ui::MainWindow *ui;
    void clearallproperty();
    void setUserImage();
};
#endif // MAINWINDOW_H
