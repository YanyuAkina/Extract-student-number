#include "Mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QInputDialog>
#include <QDialog>

#include <vector>
#define DEFAULT 44

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings("./config.ini",QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    ui->tag->setText(settings->value("Title","恭喜这位同学被选中了！").toString());
    randomChange(settings->value("range",DEFAULT).toInt());
    randomReset();

    std::string str = "人数:" + std::to_string(setV);
    auto status = new QLabel(QString::fromStdString(str),this);
    status->setFont(QFont("SimSun",15));
    ui->statusBar->addWidget(status);

    connect(ui->numSetting,&QAction::triggered,[=](){ //人数设置
        if (loop){
            QMessageBox::warning(this,"警告","请先暂停再尝试更改!");
            return;
        }
        int num = QInputDialog::getInt(this,"提示","输入总人数",44,1,9999);
        if (setV!=num){
            randomChange(num);
            randomReset();
            std::string str = "人数:"+std::to_string(setV);
            status->setText(QString::fromStdString(str));
            QMessageBox::information(this,"成功","设置完毕！");
        }
    });
    connect(ui->SPBtn,&QPushButton::clicked,[this](){ //开始暂停按钮实现
        randomGet();
    });
    connect(ui->resetBtn,&QPushButton::clicked,[=](){ //重置按钮实现
        if (loop){
            QMessageBox::warning(this,"警告","请先暂停再重置!");
            return;
        }
        randomReset();
    });
    connect(ui->about,&QAction::triggered,[this](){ //关于
        auto dia = new QDialog(this);
        dia->resize(600,200);
        dia->setWindowTitle("关于");
        auto intro = new QLabel(dia);
        intro->resize(600,200);
        intro->setFont(QFont("SimSun",12));
        intro->setText("开发人员:\n*烟雨あきな\n开源代码:\n*https://github.com/YanyuAkina/Extract-student-number\n*采用GNU GPL v3协议\n邮箱:\n*yanyuakina@outlook.com");
        dia->exec();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::randomReset()
{
    limit = setV;
    srand(static_cast<unsigned int>(std::time(nullptr)));
    numGroup.clear();
    for(int i=1;i<=setV;i++){
        numGroup.push_back(i);
    }
    ui->num->setText("??");
}

void MainWindow::randomGet()
{
    if (numGroup.size()==0){
        QMessageBox::warning(this,"警告","所有学号抽取完毕!请点击重置");
        return;
    }
    loop =! loop;
    if (loop){
        std::vector<int>::iterator it = numGroup.begin()+rand()%limit;
        ui->SPBtn->setText("暂停");
        while (loop){
            QCoreApplication::processEvents();
            ui->num->setText(QString::number(*it));
            it++;
            if (it == numGroup.end()){
                it = numGroup.begin();
            }
        }
        ui->num->setText(QString::number(*it));
        numGroup.erase(it);
        limit--;
    }
    ui->SPBtn->setText("开始");
}

void MainWindow::randomChange(int v)
{
    setV = v;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (loop)
        loop=false;
    settings->setValue("range",setV);
    e->accept();
}
