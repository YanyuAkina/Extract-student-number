#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSettings* settings;
    int setV;
    std::vector<int> numGroup;
    int limit;
    bool loop = false;

   void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow *ui;

    void randomReset();
    void randomGet();
    void randomChange(int);
};

#endif // MAINWINDOW_H
