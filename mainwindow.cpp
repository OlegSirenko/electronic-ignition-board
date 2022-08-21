#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort; // abstract serial port
    serial->setPortName("ttyUSB0"); // setting port name
    serial->setBaudRate(QSerialPort::Baud9600); //
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    connect(serial, SIGNAL(readyRead()), SLOT(serial_read()));
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}


void MainWindow::on_btn_power_clicked() // this method checks is device is connected.
{
    if (ui->btn_power->text() == "connect"){
        ui->led_1->setStyleSheet("background: 'red'");
        // Serial check connection // HOW???
        ui->btn_mode->setEnabled(true);
        on_connect();
    }
    else{
        // Serial disconnect
        on_disconnect();
    }
}


void MainWindow::on_btn_mode_clicked() // this method change mode of device
{
    if ((ui->lbl_mode->text() == "") or (ui->lbl_mode->text() == "Режим 2")){
        ui->led_2->setStyleSheet("background: 'green'");
        ui->led_3->setStyleSheet("background: 'grey'");
        ui->lbl_mode->setText("Режим 1");
        // Serial write: mode 1 (55 01 16 01 84 55 03)
        // Serial wait for answer: (55 01 20 01 6c 55 03)
    }
    else{
        ui->lbl_mode->setText("Режим 2");
        ui->led_2->setStyleSheet("background: 'grey'");
        ui->led_3->setStyleSheet("background: 'orange'");
        // Serial write: mode 2 (55 01 16 02 d7 55 03)
        // Serial wait for answer: (55 01 20 02 3f 55 03)
    }
}


void MainWindow::on_disconnect()  // method for situation when device disconnected
{
    ui->btn_power->setText("connect");
    ui->lbl_connect->setText("Disconnected");
    ui->btn_mode->setEnabled(false);
    ui->lbl_mode->setText("");
    ui->led_1->setStyleSheet("background: 'grey'");
    ui->led_2->setStyleSheet("background: 'grey'");
    ui->led_3->setStyleSheet("background: 'grey'");
    ui->led_4->setStyleSheet("background: 'grey'");
}


void MainWindow::on_connect()
{
    ui->btn_power->setText("Disconnect");
    ui->lbl_connect->setText("Connected");
    ui->lbl_mode->setText("");
}



void MainWindow::on_btn_start_clicked()
{
    // Serial send message start ignition (55 01 17 01 70 55 03)
    // Serial wait for answer
}


void MainWindow::serial_read(){}


