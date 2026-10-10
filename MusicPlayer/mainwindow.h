#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QList>
#include <QListWidgetItem>
#include <QRandomGenerator>
#include <QMediaMetaData>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handlePlaySlot();//处理播放暂停
    void toggleMusicList();// 切换音乐列表显示/隐藏的槽函数
    void onMusicItemDoubleClicked(QListWidgetItem *item); // 双击音乐项播放
    void onPrevBtnClicked(); // 上一首
    void onNextBtnClicked(); // 下一首
    void onModeBtnClicked(); // 切换播放模式槽函数
    void onMetaDataChanged(); // 处理元数据变化（用于提取封面）
    void onOpenDirBtnClicked();// 自定义扫描文件夹的槽函数

private:
    enum PlayMode {// 播放模式枚举
        OrderMode,  // 顺序播放（默认）
        RandomMode  // 随机播放
    };
    void setButtonStyle(QPushButton *button,const QString & filename);//设置按钮图标
    void initButtons();//初始化按钮
    void setBackGround(const QString &filename);//设置背景
    void loadAppointMusicDir(const QString &filename);
    void enableControlButtons(bool enable); // 启用/禁用控制按钮
    void setDefaultCover();// 设置默认封面（显示default_cover.png）

private:
    Ui::MainWindow *ui;
    QMediaPlayer *m_player;//音乐播放器
    QAudioOutput *audioOutput;
    bool m_isAnimating; // 标记动画是否正在运行（避免重复点击冲突）
    QList<QString> m_musicPaths; // 存储音乐文件的完整路径
    int m_currentIndex; // 当前播放的音乐索引
    PlayMode m_playMode; // 当前播放模式
    QRandomGenerator m_randomGen;//随机数生成器实例
};
#endif // MAINWINDOW_H
