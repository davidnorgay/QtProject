#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <Qsize>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPalette>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QListWidgetItem>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)// 调用父类QMainWindow的构造函数，指定父窗口
    , ui(new Ui::MainWindow)// 初始化UI对象
    , m_isAnimating(false) // 初始化动画状态为“未运行”（避免重复触发动画）
    , m_currentIndex(-1) // 初始没有选中的音乐（无选中音乐，后续播放时更新）
    , m_playMode(OrderMode) // 默认顺序播放
    , m_randomGen(QRandomGenerator::securelySeeded()) //随机种子初始化
{
    ui->setupUi(this);

    setWindowTitle("我的音乐播放器");//设置标题

    m_player=new QMediaPlayer(this);//音乐播放器,用于音频控制
    audioOutput=new QAudioOutput(this);//音频输出对象

    setFixedSize(1280,720);//设置窗口固定大小

    setBackGround(":/image/background2.jpg");//设置背景

    initButtons();//初始化按钮

    enableControlButtons(false); // 禁用控制按钮

    setDefaultCover(); // 初始化默认封面——放在构造函数早期，启动就显示默认图

    //音乐加载后自动提取封面
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataChanged);


    // 初始化音乐列表：默认隐藏在窗口右侧外面
    const int listWidth = 401;    // 列表宽度（从ui文件获取）
    const int listHeight = 571;   // 列表高度（从ui文件获取）
    const int targetY = 10;       // 列表Y坐标（固定）
    const int windowRight = this->width(); // 窗口右侧x坐标（1280）
    // 初始位置：窗口右侧外（x=1280，完全看不见）
    ui->musicList->setGeometry(windowRight, targetY, listWidth, listHeight);
    ui->musicList->setVisible(false); // 确保默认不可见

    // 连接音乐列表双击信号
    connect(ui->musicList, &QListWidget::itemDoubleClicked, this, &MainWindow::onMusicItemDoubleClicked);

    m_player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    QString musicDir="D:/QtProject/MusicPlayer/musicSet/";
    loadAppointMusicDir(musicDir);//加载音乐文件夹

}

void MainWindow::setButtonStyle(QPushButton *button,const QString & filename){//设置按钮图标
    button->setFixedSize(70,70);//设置按钮大小
    button->setIcon(QIcon(filename));
    button->setIconSize(QSize(button->width(),button->height()));//设置图标大小
    button->setStyleSheet("background-color:transparent");//图标背景为透明色
    //connect(ui->playBtn,&QPushButton::clicked,this,&MainWindow::handlePlaySlot);
}

void MainWindow::initButtons(){//初始化按钮
    setButtonStyle(ui->prevBtn,":/Icon/prev.png");
    setButtonStyle(ui->playBtn,":/Icon/play.png");
    setButtonStyle(ui->nextBtn,":/Icon/next.png");
    setButtonStyle(ui->modeBtn,":/Icon/order.png");//默认为顺序播放
    setButtonStyle(ui->listBtn,":/Icon/list.png");
    setButtonStyle(ui->openDirBtn, ":/Icon/file.png");

    connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::handlePlaySlot);
    connect(ui->listBtn, &QPushButton::clicked, this, &MainWindow::toggleMusicList);
    connect(ui->prevBtn, &QPushButton::clicked, this, &MainWindow::onPrevBtnClicked);
    connect(ui->nextBtn, &QPushButton::clicked, this, &MainWindow::onNextBtnClicked);
    connect(ui->modeBtn, &QPushButton::clicked, this, &MainWindow::onModeBtnClicked);
    connect(ui->openDirBtn, &QPushButton::clicked, this, &MainWindow::onOpenDirBtnClicked);

}

void MainWindow::setBackGround(const QString &filename){//初始化背景
    QPixmap pixmap(filename);

    QSize windowSize=this->size();//获取当前窗口大小

    QPixmap scalePixmap=pixmap.scaled(windowSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);//将图片适配当前窗口大小

    QPalette palette = this->palette();//创建QPalette对象并设置背景图片，调色板
    palette.setBrush(QPalette::Window,QBrush(scalePixmap));

    this->setPalette(palette);//将调色板运用到窗口上
}

void MainWindow::handlePlaySlot(){//播放键/暂停切换
    if(m_player->playbackState()==QMediaPlayer::PlayingState){
        m_player->pause();
        ui->playBtn->setIcon(QIcon(":/Icon/play.png"));
    }
    else{
        m_player->play();
        ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
    }
}

void MainWindow::loadAppointMusicDir(const QString &filename){//加载指定文件夹
    QDir dir(filename);
    if(dir.exists() == false){//文件夹不存在
        QMessageBox::warning(this,"文件夹","文件夹打开失败");
        return ;
    }

    QFileInfoList filelist = dir.entryInfoList(QDir::Files);

    // 清空现有列表
    ui->musicList->clear();
    m_musicPaths.clear();

    for(auto element : filelist){ // 遍历文件夹中的MP3文件
        if(element.suffix() == "mp3"){
            ui->musicList->addItem(element.baseName());//添加到音乐列表
            m_musicPaths.append(element.filePath());//保存完整路径
        }
    }
}

void MainWindow::toggleMusicList() {//音乐列表动画加载
    if (m_isAnimating) { //如果动画正在运行，忽略本次点击（避免冲突）
        return;
    }

    const int listWidth = 401;//固定列表的基础参数（从ui文件获取）
    const int listHeight = 571;
    const int targetY = 10;
    const int windowRight = this->width(); // 窗口右侧x=1280
    const int showX = 870;                // 列表显示时的x坐标（从ui文件获取）
    const int hideX = windowRight;        // 列表隐藏时的x坐标（窗口右侧外）

    //创建动画对象（控制geometry属性：x变，宽高不变）
    QPropertyAnimation *listAnim = new QPropertyAnimation(ui->musicList, "geometry", this);
    listAnim->setDuration(300); // 动画持续时间（300ms，可调整快慢）
    listAnim->setEasingCurve(QEasingCurve::InOutQuad);    // 动画曲线：开始和结束平缓（避免生硬）

    //标记动画开始
    m_isAnimating = true;
    //判断当前状态：隐藏→显示（拉进来），显示→隐藏（拉出去）
    if (!ui->musicList->isVisible()) {
        //隐藏→显示（从右侧拉进窗口）
        ui->musicList->setVisible(true); // 先显示列表（否则动画看不见）
        // 起始位置：窗口右侧外（hideX, targetY, ...）
        QRect startRect(hideX, targetY, listWidth, listHeight);
        // 结束位置：目标显示位置（showX, targetY, ...）
        QRect endRect(showX, targetY, listWidth, listHeight);
        listAnim->setStartValue(startRect);
        listAnim->setEndValue(endRect);
    } else {
        //显示→隐藏（从窗口拉回右侧外）
        // 起始位置：当前显示位置（showX, targetY, ...）
        QRect startRect(showX, targetY, listWidth, listHeight);
        // 结束位置：窗口右侧外（hideX, targetY, ...）
        QRect endRect(hideX, targetY, listWidth, listHeight);
        listAnim->setStartValue(startRect);
        listAnim->setEndValue(endRect);

        // 动画结束后，设置列表为不可见（避免在窗口外占用事件）
        connect(listAnim, &QPropertyAnimation::finished, this, [=]() {
            ui->musicList->setVisible(false);
        });
    }

    //动画结束后：重置状态+删除动画对象（避免内存泄漏）
    connect(listAnim, &QPropertyAnimation::finished, this, [=]() {
        m_isAnimating = false;   // 重置动画状态
        listAnim->deleteLater(); // 自动删除动画对象
    });
    //启动动画
    listAnim->start();
}

void MainWindow::onPrevBtnClicked() {//上一首按钮被点击
    if (m_musicPaths.isEmpty()) return;// 如果没有加载任何音乐

    int listSize = m_musicPaths.size();
    if (listSize == 1) { // 列表只有1首时，直接播放当前歌曲
        m_player->setSource(QUrl::fromLocalFile(m_musicPaths[0]));
        m_player->play();
        ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
        ui->musicList->setCurrentRow(0);
        return;
    }

    if (m_playMode == OrderMode) {// 顺序模式：上一曲（不能小于0）
        if (m_currentIndex <= 0) return;
        m_currentIndex--;
    } else {// 随机模式：生成不等于当前索引的随机数
        int randomIndex;
        do {
            randomIndex = m_randomGen.bounded(listSize);//随机数
        } while (randomIndex == m_currentIndex); // 避免和当前歌曲重复
        m_currentIndex = randomIndex;
    }

    // 播放逻辑
    m_player->setSource(QUrl::fromLocalFile(m_musicPaths[m_currentIndex]));
    m_player->play();
    ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
    ui->musicList->setCurrentRow(m_currentIndex);
}

void MainWindow::onNextBtnClicked() {//下一首按钮被点击
    if (m_musicPaths.isEmpty()) return;
    int listSize = m_musicPaths.size();

    // 列表只有1首时，直接播放当前歌曲
    if (listSize == 1) {
        m_player->setSource(QUrl::fromLocalFile(m_musicPaths[0]));
        m_player->play();
        ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
        ui->musicList->setCurrentRow(0);
        return;
    }

    if (m_playMode == OrderMode) {
        // 顺序模式：下一曲（不能大于列表最后索引）
        if (m_currentIndex >= listSize - 1) return;
        m_currentIndex++;
    } else {
        // 随机模式：生成不等于当前索引的随机数
        int randomIndex;
        do {
            randomIndex = m_randomGen.bounded(listSize); // 随机数
        } while (randomIndex == m_currentIndex); // 避免和当前歌曲重复
        m_currentIndex = randomIndex;
    }

    // 原有播放逻辑不变
    m_player->setSource(QUrl::fromLocalFile(m_musicPaths[m_currentIndex]));
    m_player->play();
    ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
    ui->musicList->setCurrentRow(m_currentIndex);
}

void MainWindow::enableControlButtons(bool enable) {//启用按钮
    ui->playBtn->setEnabled(enable);
    ui->prevBtn->setEnabled(enable);
    ui->nextBtn->setEnabled(enable);
}

void MainWindow::onMusicItemDoubleClicked(QListWidgetItem *item) {//获取列表被双击的歌曲
    // 获取双击项的索引
    int index = ui->musicList->row(item);
    if (index >= 0 && index < m_musicPaths.size()) {
        m_currentIndex = index;
        // 设置播放源
        m_player->setSource(QUrl::fromLocalFile(m_musicPaths[index]));
        m_player->play();
        ui->playBtn->setIcon(QIcon(":/Icon/stop.png"));
        // 启用控制按钮（如果未启用）
        enableControlButtons(true);
    }
}

void MainWindow::onModeBtnClicked() {//切换播放模式（顺序↔随机）
    if (m_playMode == OrderMode) {
        // 切换到随机播放
        m_playMode = RandomMode;
        ui->modeBtn->setIcon(QIcon(":/Icon/random.png"));
    } else {
        // 切换到顺序播放
        m_playMode = OrderMode;
        ui->modeBtn->setIcon(QIcon(":/Icon/order.png"));
    }
}

void MainWindow::setDefaultCover() {// 设置默认封面（加载Icon文件夹下的default_cover.png）
    QPixmap defaultCover(":/Icon/default_cover.png");
    if (!defaultCover.isNull()) {
        // 缩放封面到coverLabel大小
        ui->coverLabel->setPixmap(defaultCover.scaled(
            ui->coverLabel->size(),    // 适配封面标签大小
            Qt::KeepAspectRatio,       // 保持宽高比（不拉伸变形）
            Qt::SmoothTransformation  // 平滑缩放（画质更好）
            ));
        ui->coverLabel->setText(""); // 清空“封面”文字
    } else { // 万一默认封面加载失败，显示提示文字
        ui->coverLabel->setText("无默认封面");
    }
}

void MainWindow::onMetaDataChanged() {//提取音乐内置封面

    QMediaMetaData metaData = m_player->metaData();//获取当前音乐的元数据（包含封面信息）

    QVariant coverData = metaData.value(QMediaMetaData::ThumbnailImage);//提取封面数据（Qt6中音乐封面唯一对应枚举：ThumbnailImage）

    if (coverData.canConvert<QImage>()) {// 尝试将封面数据转换为QImage
        QImage coverImage = coverData.value<QImage>();
        if (!coverImage.isNull()) {
            // 转换为QPixmap并缩放显示
            QPixmap coverPixmap = QPixmap::fromImage(coverImage);
            ui->coverLabel->setPixmap(coverPixmap.scaled(
                ui->coverLabel->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                ));
            ui->coverLabel->setText(""); // 清空文字
            return; // 提取成功，直接返回
        }
    }
    // 若提取不到封面（或封面为空），显示默认封面
    setDefaultCover();
}

void MainWindow::onOpenDirBtnClicked() {//自定义扫描文件夹——让用户选择本地文件夹，默认不选则保持原文件夹
    // 弹出文件夹选择对话框（初始路径为用户主目录）
    QString selectedDir = QFileDialog::getExistingDirectory(
        this,                          // 父窗口（当前主窗口）
        tr("选择音乐文件夹"),            // 对话框标题
        QDir::homePath(),              // 初始打开路径（用户主目录，如C:\Users\你的用户名）
        QFileDialog::ShowDirsOnly |    // 只显示文件夹，不显示文件
            QFileDialog::DontResolveSymlinks // 不解析符号链接（避免异常路径）
        );

    // 判断用户是否选择了文件夹：选了则加载新文件夹，没选则保持原文件夹（不做任何操作）
    if (!selectedDir.isEmpty()) {
        loadAppointMusicDir(selectedDir); // 加载用户选择的文件夹

        // 若新文件夹有音乐，自动启用控制按钮
        if (!m_musicPaths.isEmpty() && !ui->playBtn->isEnabled()) {
            enableControlButtons(true);
        }
    }
    // 用户点击“取消”时，selectedDir为空，不执行任何操作，保持原文件夹的音乐列表
}

MainWindow::~MainWindow()
{
    delete ui;
}
