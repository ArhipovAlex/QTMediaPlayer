#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QStyle"
#include <QTime>
#include <QMultimedia>
#include <QMediaMetaData>
#include <QMediaTimeInterval>
#include <QMediaTimeRange>
#include <QMessageBox>
#include <QAudioDecoder>
#include <QTimer>
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
    ui->pushButtonDelete->setText("DEL");
    ui->pushButtonClear->setText("CLR");
    //      Player init
    m_player = new QMediaPlayer(this);
    m_temp = new QMediaPlayer(this);
    m_player->setVolume(70);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());
    //      Connect
    connect(m_player,&QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(m_player,&QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    //      Playlist init
    m_playlist_model = new QStandardItemModel(this);
    this->ui->tableViewPlaylist->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList()<<"Audio track"<<"File path"<<"Duration");
    this->ui->tableViewPlaylist->hideColumn(1);
    int c_duration_width=72;
    this->ui->tableViewPlaylist->setColumnWidth(0,this->ui->tableViewPlaylist->width()-c_duration_width*1.5);
    this->ui->tableViewPlaylist->setColumnWidth(2,c_duration_width);
    //this->ui->tableViewPlaylist->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableViewPlaylist->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_playlist=new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(this->ui->pushButtonPrev, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::previous);
    connect(this->ui->pushButtonNext, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::next);
    connect(this->m_playlist, &QMediaPlaylist::currentMediaChanged, this, [this]()
    {
        select_item();
    });

    connect(this->m_playlist, &QMediaPlaylist::currentIndexChanged,
            [this](int position){setTitles();m_player->play();});

    QString filename = DEFAULT_PLAYLIST_LOCATION + "playlist.m3u";
    loadPlaylist(filename);

    timer = new QTimer();
    connect(m_temp,&QMediaPlayer::durationChanged,this,[this](){slotTimerAlarm();});
    timer->start(100);
    QVector<QString> lines = loadPlaylistToArray(filename);
    m_temp->setMedia(QUrl(lines[0]));
    int d=m_temp->duration();

}

MainWindow::~MainWindow()
{
    QString filename = DEFAULT_PLAYLIST_LOCATION + "playlist.m3u";
    //savePlaylist(filename);
    delete m_playlist_model;
    delete m_playlist;
    delete m_player;
    delete m_temp;
    delete ui;
}

void MainWindow::savePlaylist(QString filename)
{
    QString format = filename.split('.').last();
    QUrl url = QUrl::fromLocalFile(filename);
    m_playlist->save(url,format.toStdString().c_str());
}

void MainWindow::loadPlaylist(QString filename)
{
    //QString format = filename.split('.').last();
    //QUrl url = QUrl::fromLocalFile(filename);
    //m_playlist->load(url,format.toStdString().c_str());
    //for(int i=0;i<m_playlist->mediaCount();i++)
    //{
    //    QString url=m_playlist->media(i).canonicalUrl().url();
    //    QList<QStandardItem*> items;
    //    items.append(new QStandardItem(QDir(url).dirName()));
    //    items.append(new QStandardItem(url));
    //    m_playlist_model->appendRow(items);
    //}
    QVector<QString> lines = loadPlaylistToArray(filename);
    for(int i=0;i<lines.size();i++)
    {
        loadFileToPlaylist(lines[i]);

        ///

        ///
    }
}

void MainWindow::loadFileToPlaylist(QString filename)
{
    filename=filename.remove('\n');

    ///////////////

    //QMediaPlayer player;
    //player.setMedia(QUrl(filename));
    //player.media().resources();
    //QString duration;// = QTime::fromMSecsSinceStartOfDay(player.duration()).toString("hh:mm:ss");
    //connect(player,&MediaPlayer::durationChanged,[player, duration]{
    //    duration = QTime::fromMSecsSinceStartOfDay(player.duration()).toString("hh:mm:ss");
    //});
    //QString duration = QTime::fromMSecsSinceStartOfDay(player.duration()).toString("hh:mm:ss");
    ////////////////

    QMediaPlayer* player = new QMediaPlayer();
    m_temp->setMedia(QUrl(filename));

    m_temp->setVolume(50);
    //m_temp->play();

    qint64 d1 = player->duration();
    //qint64 d=m_temp->;
    QVariant data = m_temp->metaData(QMediaMetaData::Duration);
    //QTime duration(duration.toTime());
    QString duration1 = QTime::fromMSecsSinceStartOfDay(m_temp->duration()).toString("hh:mm:ss");

    //QString dura;
    //connect(m_temp, &QMediaPlayer::durationChanged, this, [&](qint64 dur) {
    //    qDebug() << "duration = " << dur;
    //    dura=QTime::fromMSecsSinceStartOfDay(m_temp->duration()).toString("hh:mm:ss");
    //});

    QList<QStandardItem*> items;
    items.append(new QStandardItem(QDir(filename).dirName()));
    items.append(new QStandardItem(filename));

    QMediaPlayer::State status=m_temp->state();
    while (status==QMediaPlayer::LoadingMedia)
    {

    }
    if(status==QMediaPlayer::LoadedMedia)m_temp->play();
    if(status==QMediaPlayer::PlayingState)
    {

        //status=m_temp->state();
        QString duration1 = QTime::fromMSecsSinceStartOfDay(m_temp->duration()).toString("hh:mm:ss");
    items.append(new QStandardItem(duration1));
    }

    //items.append(new QStandardItem(duration.toString("hh:mm:ss")));
    //items.append(new QStandardItem(d));



    m_playlist_model->appendRow(items);
    m_playlist->addMedia(QUrl(filename));

    delete player;
}

void MainWindow::setTitles()
{
    QString title = m_playlist->currentMedia().canonicalUrl().url();
    this->setWindowTitle(title.split('/').last());
    QString name=title.split('/').last()+" "+m_player->metaData(QMediaMetaData::SampleRate).toString()+" kHz, "+m_player->metaData(QMediaMetaData::AudioBitRate).toString()+" kbps";
    this->ui->labelFile->setText(name);
}

QVector<QString> MainWindow::loadPlaylistToArray(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QList<QString> lines;
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        lines.append(line);
    }
    file.close();
    return lines.toVector();
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
                "Audio files(*.mp3 *.flac);;MP-3(*.mp3);;Flac(*.flac);;Playlist(*.m3u *.m3u8 *.CUE)"
            );
    for(QString filesPath:files)
    {
//        QList<QStandardItem*> items;
//        items.append(new QStandardItem(QDir(filesPath).dirName()));
//        items.append(new QStandardItem(filesPath));
//        m_playlist_model->appendRow(items);
//        m_playlist->addMedia(QUrl(filesPath));
        this->loadFileToPlaylist(filesPath);
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
    m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    //m_playlist_model->clear();

}

void MainWindow::onDurationChanged(qint64 d)
{
    QTime t(0,0,0,0);
    t = t.addMSecs(d);
    //this->ui->tableViewPlaylist
}


void MainWindow::on_pushButtonClear_clicked()
{
    m_player->stop();
    m_playlist->clear();
    m_playlist_model->clear();
}


void MainWindow::on_pushButtonDelete_clicked()
{
    m_playlist->removeMedia(this->ui->tableViewPlaylist->currentIndex().row());
    m_playlist_model->removeRow(this->ui->tableViewPlaylist->currentIndex().row());
}

void MainWindow::slotTimerAlarm()
{
    QModelIndex s=m_playlist_model->index(position,1);
    int d=m_temp->duration();
    position++;

}

