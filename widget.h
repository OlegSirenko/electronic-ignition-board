#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_power_clicked(); // clicked on button_connect
    void on_btn_mode_clicked();  // clicked on button_mode
    void on_btn_start_clicked(); // clicked on button_start
    void on_connect();  // method on_connect to device
    void on_disconnect(); // method on disconnect of device
    void serial_read(); // method to read serial port

private:
    Ui::Widget *ui;
    QSerialPort *serial;
    int power = 0;
    int mode = 0;
    bool ignition = false;
};
#endif // WIDGET_H
