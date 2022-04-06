#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QVector"
#include "QDebug"
#include "QTime"
#include <iostream>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();

    //QListWidget with session length icons
    ui->ses_len_widget->addItem(new QListWidgetItem(QIcon(":/icons/session group/20m_green.png"), tr("20 min")));
    ui->ses_len_widget->addItem(new QListWidgetItem(QIcon(":/icons/session group/45m_yellow.png"), tr("45 min")));
    ui->ses_len_widget->addItem(new QListWidgetItem(QIcon(":/icons/session group/userDefine_yellow.png"), tr("user designed")));
    ui->ses_len_widget->setCurrentRow(-1);

    //QListWidget with session icons
    ui->ses_type_widget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/MET_red.png"), tr("MET")));
    ui->ses_type_widget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/del_yellow.png"), tr("Delta")));
    ui->ses_type_widget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/alpha_yellow.png"), tr("Alpha")));
    ui->ses_type_widget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/beta2_yellow.png"), tr("Beta2")));
    ui->ses_type_widget->setCurrentRow(-1);

    setLabelSize(0,0);
    setConnectSig(false);
    ui->sessionWidget->hide();

    //long press event
    longClickTimer.setSingleShot(true);
    connect(ui->button_start, &QPushButton::pressed,[&]{longClickTimer.start(1000);});
    connect(&longClickTimer, &QTimer::timeout, this, &MainWindow::selectUserSes);

    //dropbox interaction
    connect(ui->connection, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::connectionTest);

    //buttons connection
    connect(ui->button_power, SIGNAL(released()), this, SLOT(powerOn_Off()));
    connect(ui->earclip_left, SIGNAL(pressed()), this, SLOT(leftClipOn_Off()));
    connect(ui->earclip_right, SIGNAL(pressed()), this, SLOT(rightClipOn_Off()));
    connect(ui->button_audio, SIGNAL(pressed()), this, SLOT(audioSigOn_Off()));
    connect(ui->button_ces, SIGNAL(pressed()), this, SLOT(cesSigOn_Off()));
    connect(ui->button_up, SIGNAL(released()), this, SLOT(increaseInt()));
    connect(ui->button_down, SIGNAL(released()), this, SLOT(decreaseInt()));
    connect(ui->button_start, SIGNAL(released()), this, SLOT(selectSession()));
    connect(ui->button_clearLog, SIGNAL(released()), this, SLOT(logClear()));
    connect(ui->button_pause, SIGNAL(released()), this, SLOT(pauseResume()));
    connect(ui->button_charge, SIGNAL(released()), this, SLOT(chargeBattery()));
    connect(ui->button_del, SIGNAL(released()), this, SLOT(deleteSession()));
    connect(ui->button_clearRecord, SIGNAL(released()), this, SLOT(recClear()));

    connect(currentSession->getTimer(), &QTimer::timeout, this, &MainWindow::countdown);
}
MainWindow::~MainWindow() {
    for (int i = 0; i < numSes; i++)
        delete sessionDesigned[i];
    delete [] sessionDesigned;

    for (int i = 0; i < numRec; i++)
        delete recordList[i];
    delete [] recordList;

    delete currentSession;
    delete ui;
}

//initializes local variables
void MainWindow::init() {
    powerStatus = false;
    leftClipStatus = false;
    rightClipStatus = false;
    audioJack = false;
    cesJack = false;
    sessionStatus = false;
    batteryWeak = false;
    currentSession = new Session("n/a", 0, 0);
    batteryCurrent = 100;
    batteryDecayRate = 0.25;
    addingSes = false;
    currentInt = 0;
    tdcs_label[0] = ui->tdcs_025_lit;
    tdcs_label[1] = ui->tdcs_050_lit;
    tdcs_label[2] = ui->tdcs_075_lit;
    tdcs_label[3] = ui->tdcs_100_lit;
    tdcs_label[4] = ui->tdcs_150_lit;
    tdcs_label[5] = ui->tdcs_200_lit;
    tdcs_label[6] = ui->tdcs_p1_lit;
    tdcs_label[7] = ui->tdcs_p2_lit;

    numSes = 0;
    numRec = 0;
    sessionDesigned = new Session*[SES_MAX];
    recordList = new Record*[REC_MAX];
}

//changing icons accordingly when power is on or off
void MainWindow::setVisible_() {
    if (!powerStatus) {
        ui->misc_powersig->setStyleSheet("border-image:url(:/icons/misc/powerSig_default.png)");
        ui->ces_jack->setStyleSheet("border-image:url(:/icons/misc/CES_output_default.png)");
        ui->misc_audio->setStyleSheet("border-image:url(:/icons/misc/audio_input_default.png)");
        ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_default.png)");
        ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_default.png)");
        ui->tdcs_tag->setStyleSheet("border-image:url(:/icons/tDCS/tDCS_default.png)");
        setLabelSize(0, 0);
        setConnectSig(powerStatus);
        currentSession->getTimer()->stop();
    }
    else {
        ui->misc_powersig->setStyleSheet("border-image: url(:/icons/misc/powerSig_green.png)");
        ui->tdcs_tag->setStyleSheet("border-image:url(:/icons/tDCS/tDCS_lit.png)");
        tdcs_label[currentInt]->setFixedWidth(WIDTH);
        tdcs_label[currentInt]->setFixedHeight(HEIGHT);
    }

    if (audioJack && powerStatus)
        ui->misc_audio->setStyleSheet("border-image: url(:/icons/misc/audio_input_yellow.png)");
    if (cesJack && powerStatus) {
        ui->ces_jack->setStyleSheet("border-image:url(:/icons/misc/CES_output_yellow.png)");
        if (leftClipStatus && powerStatus)
            ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_yellow.png)");
        if (rightClipStatus && powerStatus)
            ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_green.png)");
    }
    ui->connectivity_1->setVisible(powerStatus);
    ui->sessionWidget->setVisible(powerStatus);
}

// quick change of buttons state
void MainWindow::setEnable(bool boolean) {
    ui->button_start->setEnabled(boolean);
    ui->button_pause->setEnabled(boolean);
    ui->ses_fast->setEnabled(boolean);
    ui->connection->setEnabled(boolean);
    ui->button_up->setEnabled(boolean);
    ui->button_down->setEnabled(boolean);
    ui->ses_len_widget->setEnabled(boolean);
    ui->ses_type_widget->setEnabled(boolean);
    ui->ses_record->setEnabled(boolean);
}

//long press the power button to turn on/off the device
void MainWindow::powerOn_Off() {
    delayFor(2);
    if(batteryCurrent > 0 || powerStatus){
        //may change codes here when battery is implemented
        powerStatus = !powerStatus;
        if (powerStatus) {
            ui->logDisplay->append("Power On!");
            setEnable(true);
            ui->ses_fast->setEnabled(true);
        }
        else {
            ui->logDisplay->append("Power Off!");
            setEnable(false);
            currentSession->setSesLength(0);
            ui->time_remain->setText(0);
            sessionStatus = false;
        }
        setVisible_();
    }
    else {
        ui->logDisplay->append("Battery inefficient");
    }
}

// change visible of all intensity labels
void MainWindow::setLabelSize(int width, int height) {
    for (int i = 0; i < 8; i++) {
        tdcs_label[i]->setFixedWidth(width);
        tdcs_label[i]->setFixedHeight(height);
    }
}

//handle logical behaviour when interact with left earclip
void MainWindow::leftClipOn_Off() {
    leftClipStatus = !leftClipStatus;
    if (leftClipStatus) {
        ui->logDisplay->append("Left ear clip connected!");
        if (cesJack && powerStatus)
            ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_yellow.png)");
    }
    else {
        ui->logDisplay->append("Left ear clip disconnected!");
        ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_default.png)");
    }
}

//handle logical behaviour when interact with right earclip
void MainWindow::rightClipOn_Off() {
    rightClipStatus = !rightClipStatus;
    if (rightClipStatus) {
        ui->logDisplay->append("Right ear clip connected!");
        if (cesJack && powerStatus)
            ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_green.png)");
    }
    else {
        ui->logDisplay->append("Right ear clip disconnected!");
        ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_default.png)");
    }
}

//handle logical behaviour when interact with audio jack
void MainWindow::audioSigOn_Off() {
    audioJack = !audioJack;
    if (audioJack) {
        ui->logDisplay->append("Audio jack is connected");
        if (powerStatus)
            ui->misc_audio->setStyleSheet("border-image:url(:/icons/misc/audio_input_yellow.png)");
        return;
    }
    ui->logDisplay->append("Audio jack is disconnected!");
    ui->misc_audio->setStyleSheet("border-image:url(:/icons/misc/audio_input_default.png)");
}

//handle logical behaviour when interact with CES input
void MainWindow::cesSigOn_Off() {
    cesJack = !cesJack;
    if (cesJack) {
        ui->logDisplay->append("CES jack is connected");
        if (powerStatus) {
            ui->ces_jack->setStyleSheet("border-image:url(:/icons/misc/CES_output_yellow.png)");
            if (rightClipStatus)
                ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_green.png)");
            if (leftClipStatus)

                ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_yellow.png)");
        }
        return;
    }
    ui->logDisplay->append("CES jack is disconnected!");
    ui->ces_jack->setStyleSheet("border-image:url(:/icons/misc/CES_output_default.png)");
    ui->ces_right->setStyleSheet("border-image:url(:/icons/CES mode/rightCES_default.png)");
    ui->ces_left->setStyleSheet("border-image:url(:/icons/CES mode/leftCES_default.png)");
}

//quick change of visiblity of connection labels
void MainWindow::setConnectSig(bool sig) {
    ui->connectivity_1->setVisible(sig);
    ui->connectivity_2->setVisible(sig);
    ui->connectivity_3->setVisible(sig);
}

//handle user's interaction with connection dropbox
void MainWindow::connectionTest(){
    setConnectSig(false);
    if (ui->connection->value() == 1)
        ui->connectivity_1->show();
    else if (ui->connection->value() == 2)
        ui->connectivity_2->show();
    else
        ui->connectivity_3->show();
    batteryDecay();
}

//press Up button to increase intensity
void MainWindow::increaseInt() {
    if (currentInt == 7) return;
    setLabelSize(0,0);
    currentInt++;
    tdcs_label[currentInt]->setFixedWidth(WIDTH);
    tdcs_label[currentInt]->setFixedHeight(HEIGHT);
    batteryDecay();
}

//press Down button to decrease intensity
void MainWindow::decreaseInt() {
    if (!currentInt) return;
    setLabelSize(0,0);
    currentInt--;
    tdcs_label[currentInt]->setFixedWidth(WIDTH);
    tdcs_label[currentInt]->setFixedHeight(HEIGHT);
    batteryDecay();
}

//add newly created session onto user designed session list
void MainWindow::addUserDesignedSes(QString type, int len) {
    sessionDesigned[numSes++] = new Session(type, len, len);
    ui->ses_list->addItem(new QListWidgetItem("Session: " + type + ", length: " + QString::number(len) + "min"));
}

//check if newly created session is a duplicate
bool MainWindow::checkDuplicate(QString type, int len) {
    for (int i = 0; i < numSes; i++)
        if (sessionDesigned[i]->getSesLength() == len && sessionDesigned[i]->getSesType() == type) return true;
    return false;
}

//handle delete button
void MainWindow::deleteSession() {
    if (numSes == 0) return;
    int ind = ui->ses_list->currentRow();
    for (int i = ind; i < numSes-1; i++) {
        sessionDesigned[i]->setSesLength(sessionDesigned[i+1]->getSesLength());
        sessionDesigned[i]->setSesType(sessionDesigned[i+1]->getSesType());
    }
    delete sessionDesigned[--numSes];
    delete ui->ses_list->takeItem(ind);
}

//delay the entire system for n seconds
void MainWindow::delayFor(int n) {
    QTime delay = QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < delay)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//handle start button when long hold
void MainWindow::selectUserSes() {
    if (!numSes) {
        ui->logDisplay->append("No user designed session available");
        return;
    }
    int ind = ui->ses_list->currentRow();
    currentSession->setSesLength(sessionDesigned[ind]->getSesLength());
    currentSession->setSesType(sessionDesigned[ind]->getSesType());
    currentSession->setSesDuration(sessionDesigned[ind]->getSesLength());
    startSession();
}

//handle start button when normal clicked
void MainWindow::selectSession() {
    if(!longClickTimer.isActive()) return;

    longClickTimer.stop();
    if (ui->ses_len_widget->selectedItems().size() == 0) {
        ui->logDisplay->append("pick a session length");
        return;
    }
    if (ui->ses_len_widget->currentItem()->text() ==("user designed") || addingSes){
        if (numSes == SES_MAX) {
            QMessageBox::information(this, "List full", "User designed sessions list is full, delete existed to add more");
            return;
        }
        addingSes = !addingSes;
        if (addingSes) {
            QMessageBox::information(this, "User designed session", "select a session type and length, then press the button again to add.");
            ui->logDisplay->append("Creating new session...");
        }
        else {
            if (ui->ses_type_widget->selectedItems().size() == 0 || ui->ses_len_widget->currentItem()->text() ==("user designed")) {
                ui->logDisplay->append("No session created.");
                return;
            }
            QString ses_type = ui->ses_type_widget->currentItem()->text();
            int ses_len = ui->ses_len_widget->currentItem()->text().left(2).toInt();
            if (checkDuplicate(ses_type, ses_len)) {
                ui->logDisplay->append("This session is already existed, try a different one.");
                addingSes = true;
                return;
            }
            ui->logDisplay->append("Created session type " + ses_type + " with length " + QString::number(ses_len) + "min");
            addUserDesignedSes(ses_type, ses_len);
        }
        return;
    }
    if (ui->ses_type_widget->selectedItems().size() == 0) {
        ui->logDisplay->append("pick a session type");
        return;
    }
    currentSession->setSesLength(ui->ses_len_widget->currentItem()->text().left(2).toInt());
    currentSession->setSesDuration(ui->ses_len_widget->currentItem()->text().left(2).toInt());
    currentSession->setSesType(ui->ses_type_widget->currentItem()->text());
    startSession();
}

//start Session according to user's input
void MainWindow::startSession() {
    sessionStatus = !sessionStatus;
    ui->logDisplay->append("Selected session: " + currentSession->getSesType());
    ui->logDisplay->append("Selected length: " + QString::number(currentSession->getSesLength()) + "min");
    ui->button_start->setEnabled(false);
    ui->ses_fast->setEnabled(false);
    ui->ses_record->setEnabled(false);
    delayFor(1);
    ui->logDisplay->append("Session in progress...");
    if (sessionStatus){
        if (ui->ses_fast->isChecked())
            currentSession->getTimer()->start(100);
        else
            currentSession->getTimer()->start(1000);
    }
}

//update timer and battery every second
void MainWindow::countdown() {
    int sec = currentSession->getSesLength() % 60;
    int min = (currentSession->getSesLength() - sec)/60;
    QTime time(0,min,sec);
    QString time_text = time.toString("ss");
    ui->time_remain->setText(time_text + " min(s)");
    currentSession->reduceSesLength();
    QString battery_text = QString::number(batteryCurrent);
    ui->battery_state->setText(battery_text + "%");
    batteryCurrent -= batteryDecayRate;
    batteryIconUpdate();
    // when session times out
    if (currentSession->getSesLength() == -1) {
        ui->logDisplay->append("Session finished.");
        ui->button_start->setEnabled(true);
        ui->ses_fast->setEnabled(true);
        ui->ses_record->setEnabled(true);
        currentSession->getTimer()->stop();
        sessionStatus = false;
        addRecord();
    }
}

//adding record to history list
void MainWindow::addRecord() {
    if (!ui->ses_record->isChecked()) return;
    const char *intensity[8] = {"0.25", "0.50", "0.75", "1.00", "1.50", "2.00", "+1   ", "+2  "};    
    QString type = currentSession->getSesType();
    QString duration = QString::number(currentSession->getSesDuration());
    QString in = QString::fromStdString(intensity[currentInt]);
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString qstr = QString("%1%2min    %3   %4").arg(type, -9).arg(duration).arg(in).arg(time);
    ui->recordDisplay->append(qstr);
    recordList[numRec++] = new Record(currentSession->getSesType(), currentSession->getSesLength(), in, time);
}

//handle pause/resume button
void MainWindow::pauseResume() {
    if (currentSession->getSesLength() <= 0) {
        ui->logDisplay->append("Start a Session first");
        return;
    }
    if (sessionStatus) {
        currentSession->getTimer()->stop();
        ui->logDisplay->append("Session pause");
    }
    else {
        if (ui->ses_fast->isChecked())
            currentSession->getTimer()->start(100);
        else
            currentSession->getTimer()->start(1000);
        ui->logDisplay->append("Session resume");
    }
    sessionStatus =! sessionStatus;
}

//update battery icon and display battery state
void MainWindow::batteryIconUpdate() {
    if (50 < batteryCurrent && batteryCurrent <= 74) {
        ui->battery_state->setStyleSheet("border-image: url(:/overlay/bat_yellow.png)");
    }
    else if (25 < batteryCurrent && batteryCurrent <= 49) {
        ui->battery_state->setStyleSheet("border-image: url(:/overlay/bat_brown.png)");
    }
    else if (0 < batteryCurrent && batteryCurrent <= 24){
        ui->battery_state->setStyleSheet("border-image: url(:/overlay/bat_red.png)");
        if (!batteryWeak) {
            ui->logDisplay->append("Battery less than 25%");
            QMessageBox::information(this, "Low Battery", "25% battery remaining.");
            batteryWeak = true;
        }
    }
    else if (batteryCurrent <= -1){
        ui->battery_state->setStyleSheet("border-image: url(:/overlay/bat_emp.png)");
        ui->logDisplay->append("Battery is fully depleted");

        ui->button_charge->setEnabled(true);
        powerOn_Off();
    }
}

//adjust battery decay rate according to connection, intensity
void MainWindow::batteryDecay() {
    double int_decay = (currentInt+1)*0.1;
    double con_decay = ui->connection->value()*0.1;
    batteryDecayRate = 0.25 + int_decay - con_decay;
}

//fully charge battery
void MainWindow::chargeBattery() {
    batteryCurrent = 100;
    ui->button_charge->setEnabled(false);
    ui->battery_state->setStyleSheet("border-image: url(:/overlay/bat_green.png)");
    ui->battery_state->setText("100%");
    ui->logDisplay->append("Battery fully charged");
}

//clear record list
void MainWindow::recClear() {
    ui->recordDisplay->clear();
    for (int i = 0; i < numRec; i++)
        delete recordList[i];
    numRec = 0;
}

//clear log
void MainWindow::logClear(){
    ui->logDisplay->clear();
}
