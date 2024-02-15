#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QStyle"
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Buttons style
    ui->pushButtonOpen->setIcon(style()->standardIcon(QStyle::SP_DriveDVDIcon));
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->pushButtonLoop->setIcon(style()->standardIcon(QStyle::SP_CommandLink));
    ui->pushButtonShuffle->setText("312");
    //Player init
    m_player = new QMediaPlayer(this);
    m_player->setVolume(70);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());
    //Connect
    connect(m_player,&QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(m_player,&QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    //Playlist init
    m_playlist_model = new QStandardItemModel(this);
    this->ui->tableViewPlaylist->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList()<<"Audio track"<<"File path");
    this->ui->tableViewPlaylist->hideColumn(1);
    this->ui->tableViewPlaylist->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_playlist=new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(this->ui->pushButtonPrev, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::previous);
    connect(this->ui->pushButtonNext, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::next);
    connect(this->m_playlist, &QMediaPlaylist::currentMediaChanged, this, [this]()
    {
        select_item();
    });
}

MainWindow::~MainWindow()
{
    delete m_playlist_model;
    delete m_playlist;
    delete m_player;
    delete ui;
}

void MainWindow::on_position_changed(qint64 position)
{
    QTime q_time=QTime::fromMSecsSinceStartOfDay(position);
    this->ui->labelProgress->setText(QString(q_time.toString("hh:mm:ss")));
    this->ui->horizontalSliderProgress->setSliderPosition(position);
}

void MainWindow::on_duration_changed(qint64 duration)
{
    this->ui->horizontalSliderProgress->setMaximum(duration);
    QTime q_time=QTime::fromMSecsSinceStartOfDay(duration);
    ui->labelDuration->setText(QString("Duration: ").append(q_time.toString("hh:mm:ss")));
}


void MainWindow::on_pushButtonOpen_clicked()
{
//    QString file=QFileDialog::getOpenFileName
//            (
//            this,
//            "Open FIle",
//            NULL,
//            "Audio files(*.mp3 *.flac);;MP-3(*.mp3);;Flac(*.flac)"
//            );
//    this->ui->labelFile->setText("File: "+file);
//    this->setWindowTitle("MediaPlayerPD321 - "+file.split('/').last());
//    this->m_player->setMedia(QUrl::fromLocalFile(file));

    QStringList files=QFileDialog::getOpenFileNames
            (
                this,
                "OpenFile",
                "C:\\Users\\Sania\\Music\\Sabaton - 2023",
                "Audio files(*.mp3 *.flac);;MP-3(*.mp3);;Flac(*.flac)"
            );
    for(QString filesPath:files)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QDir(filesPath).dirName()));
        items.append(new QStandardItem(filesPath));
        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl(filesPath));
    }
}


void MainWindow::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
}


void MainWindow::on_pushButtonPlay_clicked()
{
    m_player->play();
    select_item();
}


void MainWindow::on_pushButtonPause_clicked()
{
    m_player->pause();
}


void MainWindow::on_horizontalSliderProgress_sliderMoved(int position)
{
    this->m_player->setPosition(position);
}


void MainWindow::on_pushButtonMute_clicked()
{
    if (m_player->isMuted())
    {
        m_player->setMuted(false);
        ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
    else
    {
        m_player->setMuted(true);
        ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }
}


void MainWindow::on_pushButtonStop_clicked()
{
    m_player->stop();
}

void MainWindow::select_item()
{
    int row=m_playlist->currentIndex();
    this->ui->tableViewPlaylist->selectRow(row);
}


void MainWindow::on_pushButtonPrev_clicked()
{
    select_item();
}


void MainWindow::on_pushButtonNext_clicked()
{
    select_item();
}


void MainWindow::on_tableViewPlaylist_doubleClicked(const QModelIndex &index)
{
    m_playlist->setCurrentIndex(index.row());
    on_pushButtonPlay_clicked();
}


void MainWindow::on_pushButtonLoop_clicked()
{
    if(m_playlist->playbackMode()==QMediaPlaylist::Loop)
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        ui->pushButtonLoop->setIcon(style()->standardIcon(QStyle::SP_CommandLink));
    }
    else
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        ui->pushButtonLoop->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    }
}


void MainWindow::on_pushButtonShuffle_clicked()
{
    m_playlist->shuffle();
    //m_playlist_model->clear();

}

