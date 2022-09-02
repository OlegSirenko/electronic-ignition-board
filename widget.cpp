#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //ui->btn_mode->setGeometry(100, 100, 100, 200);
    serial = new QSerialPort(this); // abstract serial port
    serial->setPortName("ttyUSB0"); // setting port name
    serial->setBaudRate(QSerialPort::Baud9600); // setting boudrate
    serial->setDataBits(QSerialPort::Data8); //
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        QSerialPort port;
        port.setPort(info);
        if (port.open(QIODevice::ReadWrite)){
            qDebug() << "Name: " +info.portName() + " " + info.description() + " " + info.manufacturer();
        }
    }

    connect(serial, SIGNAL(readyRead()), this, SLOT(serial_read())); // independent reading buffer
}

Widget::~Widget()
{
    serial->close(); // close port
    delete serial; // delete object of serial
    delete ui;
}


void Widget::on_btn_power_clicked()
{
    if (power == 0){
        on_connect();
    }
    else {
        on_disconnect(); // Serial disconnect
    }
}


void Widget::on_btn_mode_clicked()
{
    if (ignition){ // ui->btn_mode->text()=="ПУСК"
        if (mode == 1){
            qDebug("PUSK 1");
            QByteArray data = QByteArrayLiteral("\x55\x01\x17\x01\x70\x55\x03");
            serial->write(data); // SERIAL send ignition: mode 1
        }
        else{
            qDebug("PUSK 2");
            QByteArray data = QByteArrayLiteral("\x55\x01\x17\x02\x23\x55\x03");
            serial->write(data); // SERIAL send ignition: mode 2
        }
        return;
    }

    ui->btn_start->setEnabled(true);

    // making cycle switching mode
    if ((mode == 0) || (mode == 2)){ // if mode is undefenite or second mode =>
        ui->led_2->setStyleSheet("background: 'green'");
        ui->led_3->setStyleSheet("background: 'grey'");
        mode = 1;
        ui->btn_mode->setStyleSheet("min-width: 85px;min-height: 90px;border-image: url(:button_mode_1.png);"
                                    "background-color: transparent;"
                                    "background: none;"
                                    "border: none;"
                                    "background-repeat: none;");
    }
    else{ // if mode == 1 =>
        ui->led_2->setStyleSheet("background: 'grey'");
        ui->led_3->setStyleSheet("background: 'orange'");
        mode = 2;
        ui->btn_mode->setStyleSheet("min-width: 85px;min-height: 90px;border-image: url(:button_mode_2.png);"
                                    "background-color: transparent;"
                                    "background: none;"
                                    "border: none;"
                                    "background-repeat: none;");
    }
}


void Widget::on_btn_start_clicked()
{
    ui->btn_start->setEnabled(false);
    ui->btn_start->setStyleSheet("min-width: 60px;\nmin-height: 250px;border-image: url(:switch_unsafe_on.png);"
                                 "background-color: transparent;"
                                 "background: none;"
                                 "border: none;"
                                 "background-repeat: none;");

    ui->btn_mode->setStyleSheet("min-width: 85px;\nmin-height: 90px;\nborder-image: url(:button_start.png);"
                                "background-color: transparent;"
                                "background: none;"
                                "border: none;"
                                "background-repeat: none;");
    ui->btn_mode->setEnabled(false);
    if (mode == 1) {
        QByteArray data = QByteArrayLiteral("\x55\x01\x16\x01\x84\x55\x03");
        serial->write(data); // Serial send: mode_1 (55 01 16 01 84 55 03) ((command to charge copacitor in mode 1)
        qDebug()<<"SEND " + data.toHex();
        ui->textBrowser->append("SEND " + data.toHex());
        // Serial wait for answer: (55 01 20 01 6c 55 03)
    }
    else{
        QByteArray data = QByteArrayLiteral("\x55\x01\x16\x02\xd7\x55\x03");
        serial->write(data);  // Serial send: mode_2 (55 01 16 02 d7 55 03) ((command to charge copacitor in mode 2)
        qDebug()<<"SEND " + data.toHex();
        ui->textBrowser->append("SEND " + data.toHex());
        // Serial wait for answer: (55 01 20 02 3f 55 03)
    }
    ignition = true;
}


void Widget::on_connect(){
    if(!serial->open(QIODevice::ReadWrite)){ // CHECK CONNECTION
        ui->textBrowser->append((tr("error %1").arg(serial->error()))); // if error => log error
        return; // stop function
    }
    else{
        qDebug()<<"Connected";
        power = 1;
        ui->led_1->setStyleSheet("background: 'red'");
        ui->btn_mode->setEnabled(true);
        ui->btn_power->setStyleSheet("border-image: url(:switch_power_on.png); min-width: 100px; min-height: 170px; "
                                     "background-color: transparent;");
    }
}

void Widget::on_disconnect(){
    serial->close(); // Serial CLOSE CONNECTION
    // Return labels and buttons in start position

    ui->btn_power->setStyleSheet("border-image: url(:switch_power_off.png); min-width: 100px; min-height: 170px; "
                                 "background-color: transparent;");
    ui->btn_mode->setStyleSheet("border-image: url(:button_start.png); min-width: 85px; min-height: 90px; "
                                 "background-color: transparent;");
    ui->btn_start->setStyleSheet("border-image: url(:switch_safe.png); min-width: 87px; min-height: 270px; "
                                 "background-color: transparent;");

    ui->btn_mode->setEnabled(false);
    ui->btn_start->setEnabled(false);

    power = 0;
    mode = 0; // return int mode to undefenite
    ignition=false; //
    // turn off LEDs
    ui->led_1->setStyleSheet("background: 'grey'");
    ui->led_2->setStyleSheet("background: 'grey'");
    ui->led_3->setStyleSheet("background: 'grey'");
    ui->led_4->setStyleSheet("background: 'grey'");
    Widget::close();
}


void Widget::serial_read(){
    QByteArray buf;
    buf = serial->readAll();
    QByteArray answer = buf;
    //ui->lbl_answer->setText(answer);
    bool mode_selected = (answer == QByteArrayLiteral("\x55\x01\x20\x01\x6c\x55\x03")
                              || answer == QByteArrayLiteral("\x55\x01\x20\x02\x3f\x55\x03"));
    bool capacitor_charged = (answer == QByteArrayLiteral("\x55\x01\x21\x01\x98\x55\x03")
                                  || answer == QByteArrayLiteral("\x55\x01\x21\x02\xcb\x55\x03"));
    if (mode_selected){ // answer to mode chosen
        ui->textBrowser->append(QString("ВЫБРАН %1-й РЕЖИМ "+ answer.toHex()).arg(mode));
        ui->textBrowser->append(QString("Идет зарядка конденсатора"));
        //return;
    }
    if (capacitor_charged){
        ui->led_4->setStyleSheet("background:'red'");
        ui->btn_mode->setEnabled(true);
        ui->btn_mode->setStyleSheet("min-width: 85px;min-height: 85px;border-image: url(:button_mode_1.png);"
                                    "background-color: transparent;"
                                    "background: none;"
                                    "border: none;"
                                    "background-repeat: none;");
        //ui->btn_start->setStyleSheet("background: 'grey'");
        ui->textBrowser->append(QString("КОНДЕНСАТОР ЗАРЯЖЕН ДЛЯ %1-го РЕЖИМА "+ answer.toHex()).arg(mode));
        //return;
    }

    if (answer == QByteArrayLiteral("\x55\x01\x22\x01\xb5\x55\x03")){
        ui->textBrowser->append("ПОДЖИГ В 1-м РЕЖИМЕ ИНИЦИАЛИЗИРОВАН! "+ answer.toHex());
    }
    if (answer == QByteArrayLiteral("\x55\x01\x22\x02\xe6\x55\x03")){
        ui->textBrowser->append("ПОДЖИГ ВО 2-м РЕЖИМЕ ИНИЦИАЛИЗИРОВАН! "+ answer.toHex());
    }
    qDebug()<< "ANSWERED +\n" + buf.toHex();
    buf.clear();
}
