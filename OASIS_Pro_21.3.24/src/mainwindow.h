#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer longClickTimer;

    int  batteryLevel = 100;
    bool powerStatus;

private slots:
    void powerOn_Off();
    void navigateSessionLength();
    void navigateSessions_Up();
    void navigateSessions_Down();
    void selectTherapy();

};
#endif // MAINWINDOW_H
