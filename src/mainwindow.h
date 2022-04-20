#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WIDTH 41
#define HEIGHT 16
#define SES_MAX 8
#define REC_MAX 64

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QtWidgets>

#include "session.h"
#include "record.h"

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
    QTimer blinkTimer;
    QTimer powerSaveTimer;
    QTimer powerButtonTimer;

    double batteryCurrent;
    double batteryDecayRate;
    int currentInt;
    int startSessionTime;
    int endSessionTime;
    int numSes;
    int numRec;

    bool powerStatus;
    bool leftClipStatus;
    bool rightClipStatus;
    bool cesJack;
    bool audioJack;
    bool sessionStatus;
    bool batteryWeak;
    bool addingSes;

    Session *currentSession;
    Session **sessionDesigned;
    Record **recordList;
    QLabel* tdcs_label[8];
    QLabel* connectionlev_label[8];

    //helpers
    void init();
    void powerSave();
    void setEnable(bool);
    void setVisible_();
    void delayFor(int);
    void countdown();
    void startSession();
    void addUserDesignedSes(QString, int);
    bool checkDuplicate(QString, int);

    void setConnectSig(bool);
    void setLabelSize(int, int);
    void setConlevLabelSize(int, int);
    void batteryIconUpdate();
    void batteryDecay();
    void addRecord();
    void resumeSession();
    void pauseSession();

private slots:
    //toggle
    void powerOn_Off();
    void selectUserSes();
    void leftClipOn_Off();
    void rightClipOn_Off();
    void audioSigOn_Off();
    void cesSigOn_Off();
    void endSession();

    //dropbox function
    void connectionTest();

    //button functions
    void selectSession();
    void pauseResume();
    void increaseInt();
    void decreaseInt();
    void chargeBattery();
    void deleteSession();
    void logClear();
    void recClear();
};
#endif // MAINWINDOW_H
