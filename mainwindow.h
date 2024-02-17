#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void savePlaylist(QString filename);
    void loadPlaylist(QString filename);

private slots:
    void on_position_changed(qint64 position);
    void on_duration_changed(qint64 duration);
    void on_pushButtonOpen_clicked();

    void on_horizontalSliderVolume_valueChanged(int value);

    void on_pushButtonPlay_clicked();

    void on_pushButtonPause_clicked();


    void on_horizontalSliderProgress_sliderMoved(int position);

    void on_pushButtonMute_clicked();

    void on_pushButtonStop_clicked();

    void select_item();

    void on_pushButtonPrev_clicked();

    void on_pushButtonNext_clicked();

    void on_tableViewPlaylist_doubleClicked(const QModelIndex &index);

    void on_pushButtonLoop_clicked();

    void on_pushButtonShuffle_clicked();



    void on_pushButtonClear_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* m_player;

    QMediaPlaylist* m_playlist;
    QStandardItemModel* m_playlist_model;

    const QString DEFAULT_PLAYLIST_LOCATION = "D:\\Users\\Sania\\Source\\Qt\\build-MediaPlayer-Desktop_x86_windows_msvc2019_pe_64bit-Debug\\debug\\";
};
#endif // MAINWINDOW_H
