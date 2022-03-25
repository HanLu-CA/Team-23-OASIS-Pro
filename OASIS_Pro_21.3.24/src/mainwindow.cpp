#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QVector"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    powerStatus = false;

    //QListWidget with session length icons
    ui->sessionLengthWidget->setVisible(false);
    ui->sessionLengthWidget->addItem(new QListWidgetItem(QIcon(":/icons/session group/20m_green.png"), tr("20 min")));
    ui->sessionLengthWidget->addItem(new QListWidgetItem(QIcon(":/icons/session group/45m_yellow.png"), tr("45 min")));
    ui->sessionLengthWidget->addItem(new QListWidgetItem(QIcon(":/icons/session group/userDefine_yellow.png"), tr("user designed")));
    ui->sessionLengthWidget->setCurrentRow(-1);

    //QListWidget with session icons
    ui->sessionsWidget->setVisible(false);
    ui->sessionsWidget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/MET_red.png"), tr("MET")));
    ui->sessionsWidget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/del_yellow.png"), tr("Delta")));
    ui->sessionsWidget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/alpha_yellow.png"), tr("Alpha")));
    ui->sessionsWidget->addItem(new QListWidgetItem(QIcon(":/icons/session freq/beta2_yellow.png"), tr("Beta")));
    ui->sessionsWidget->setCurrentRow(-1);


    ui->greenLight->setVisible(false);
    ui->widget->setVisible(false);
    ui->sessionWidget->setVisible(false);

    //long press event
    longClickTimer.setSingleShot(true);
    connect(ui->button_power, &QPushButton::pressed, [&]{longClickTimer.start(2000);});
    connect(ui->button_power, &QPushButton::released, this, &MainWindow::navigateSessionLength);
    connect(&longClickTimer, &QTimer::timeout, this, &MainWindow::powerOn_Off);

    connect(ui->button_up, &QPushButton::pressed, this, &MainWindow::navigateSessions_Up);
    connect(ui->button_down, &QPushButton::pressed, this, &MainWindow::navigateSessions_Down);

    connect(ui->button_start, &QPushButton::pressed, this, &MainWindow::selectTherapy);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//long press the power button to turn on/off the device
void MainWindow::powerOn_Off()
{
    if(batteryLevel > 0){
        powerStatus = !powerStatus;
    }
    ui->greenLight->setVisible(powerStatus);
    ui->widget->setVisible(powerStatus);
    ui->sessionLengthWidget->setVisible(powerStatus);
    ui->sessionWidget->setVisible(powerStatus);
    ui->sessionsWidget->setVisible(powerStatus);
}

//press the power button to navigate session lengths
void MainWindow::navigateSessionLength()
{
    if(longClickTimer.isActive() && powerStatus){
        longClickTimer.stop();

        int nextIndex = ui->sessionLengthWidget->currentRow() + 1;

        if (nextIndex > ui->sessionLengthWidget->count() - 1) {
            nextIndex = 0;
        }

        ui->sessionLengthWidget->setCurrentRow(nextIndex);
    }
}

//implement INT Up Button
void MainWindow::navigateSessions_Up()
{
    if(powerStatus){

        int nextIndex = ui->sessionsWidget->currentRow() - 1;

        if (nextIndex < 0) {
            nextIndex = ui->sessionsWidget->count() - 1;;
        }

        ui->sessionsWidget->setCurrentRow(nextIndex);
    }
}

//implement INT Down Button
void MainWindow::navigateSessions_Down()
{
    if(powerStatus){
        ui->sessionsWidget->setVisible(powerStatus);

        int nextIndex = ui->sessionsWidget->currentRow() + 1;

        if (nextIndex > ui->sessionsWidget->count() - 1) {
            nextIndex = 0;
        }

        ui->sessionsWidget->setCurrentRow(nextIndex);
    }
}

void MainWindow::selectTherapy()
{
    if(ui->sessionLengthWidget->selectedItems().size() != 0){ //if a session length is selected
        qDebug() << "Selected Session Length:" << ui->sessionLengthWidget->currentItem()->text();
    }
    if(ui->sessionsWidget->selectedItems().size() != 0){ //if a session theraphy is selected
        qDebug() << "Selected Session:" << ui->sessionsWidget->currentItem()->text();
        //startTherapy()
    }

}

