#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QNetworkAccessManager *netManager= nullptr;
//    QNetworkReply *netreplay= nullptr;
//    QNetworkReply *repoReply= nullptr;
//    QByteArray dataBuffer;
//    QPixmap *img= nullptr;
    netManager=new QNetworkAccessManager();
    //netreplay=new QNetworkReply;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearallproperty()
{
    ui->laAccountType->clear();
    ui->laFollowers->clear();
    ui->laFollowing->clear();
    ui->laName->clear();
    ui->laRepoCount->clear();
    ui->laUserName->clear();
    ui->textBio->clear();
    ui->textRepoList->clear();
   // ui->textUserName->clear();
    dataBuffer.clear();
}
void MainWindow::on_pUserName_clicked()
{
    auto username=QInputDialog::getText(this,"Github Username","Enter Your Github UserName");
    if(!username.isEmpty())
    {
        clearallproperty();
        QNetworkRequest req{QUrl(QString("https://api.github.com/users/%1").arg(username))};
        QNetworkRequest repoReq{QUrl(QString("https://api.github.com/users/%1/repos").arg(username))};
        netreplay=netManager->get(req);
        connect(netreplay,&QNetworkReply::readyRead,this,&MainWindow::readData);
        connect(netreplay,&QNetworkReply::finished,this,&MainWindow::finishReading);
        repoReply=netManager->get(repoReq);
        connect(repoReply,&QNetworkReply::readyRead,this,&MainWindow::readDataForrepo);
        connect(repoReply,&QNetworkReply::finished,this,&MainWindow::finishGettingRepo);
    }
}

void MainWindow::readData()
{
    dataBuffer.append(netreplay->readAll());
}

void MainWindow::finishReading()
{
    if(netreplay->error() != QNetworkReply::NoError){
            qDebug() << "Error : " << netreplay->errorString();
            QMessageBox::warning(this,"Error",QString("Request[Error] : %1").arg(netreplay->errorString()));
        }else{

            //CONVERT THE DATA FROM A JSON DOC TO A JSON OBJECT
            QJsonObject userJsonInfo = QJsonDocument::fromJson(dataBuffer).object();


            //SET USERNAME
            QString login = userJsonInfo.value("login").toString();
            ui->laUserName->setText(login);

            // SET DISPLAY NAME
            QString name = userJsonInfo.value("name").toString();
            ui->laName->setText(name);

            //SET BIO
            auto bio = userJsonInfo.value("bio").toString();
            ui->textBio->setText(bio);

            //SET FOLLOWER AND FOLLOWING COUNT
            auto follower = userJsonInfo.value("followers").toInt();
            auto following = userJsonInfo.value("following").toInt();
            ui->laFollowers->setText(QString::number(follower));
            ui->laFollowing->setText(QString::number(following));

            //SET ACCOUNT TYPE
            QString type = userJsonInfo.value("type").toString();
            ui->laAccountType->setText(type);

            //SET PICTURE
            auto picLink = userJsonInfo.value("avatar_url").toString();
            QNetworkRequest link{QUrl(picLink)};
            netreplay = netManager->get(link);
            connect(netreplay,&QNetworkReply::finished,this,&MainWindow::setUserImage);
            dataBuffer.clear();
        }
}
void MainWindow::setUserImage()
{
    img=new QPixmap();
    img->loadFromData(netreplay->readAll());
    QPixmap temp = img->scaled(ui->laImage->size());
    ui->laImage->setPixmap(temp);
}
void MainWindow::readDataForrepo()
{
dataBuffer.append(repoReply->readAll());
}

void MainWindow::finishGettingRepo()
{
    if(repoReply->error() != QNetworkReply::NoError){
            qDebug() << "Error Getting List of Repo: " << netreplay->errorString();
            QMessageBox::warning(this,"Error",QString("Request[Error] : %1").arg(netreplay->errorString()));
        }else{
            QJsonArray repoInfo = QJsonDocument::fromJson(dataBuffer).array();
            //ui->textRepoList->setc(repoInfo.size());
            qDebug()<<"repoInfo.size()"<<repoInfo.size();
            for(int i{0}; i < repoInfo.size(); ++i){
                auto repo = repoInfo.at(i).toObject();
                QString repoName = repo.value("name").toString();
                //ui->textRepoList->addaddt(repoName);
                ui->textRepoList->append(repoName);
            }
        }
}
