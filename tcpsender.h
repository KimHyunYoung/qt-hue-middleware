#ifndef TCPSENDER_H
#define TCPSENDER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QElapsedTimer>
#include <windows.h>
typedef short _stdcall(*inpfuncPtr)(short portaddr);
typedef void _stdcall(*oupfuncPtr)(short portaddr, short datnum);
namespace Ui {
class tcpsender;
}

class tcpsender : public QMainWindow
{
    Q_OBJECT
signals:
    void full();
    void half();
    void off();
    void color();

public:
    bool testfordelay = true;
    bool training = true;
    int outputtime = 0;
    int output = 0;
    int starttime = 0;
    int nowtime = 0;
    int lasttime = 0;
    int lastanswer = 0;
    int positionone = 1;
    int positiontwo = 2;
    int positionthree = 3;
    int positionfour = 4;
    explicit tcpsender(QWidget *parent = 0);
    ~tcpsender();
public slots:
private:

    Ui::tcpsender *ui;
    QTcpServer *server;
    QTcpSocket *client;
    QTcpSocket *client2;
    QTcpServer *servermat;
    QTcpSocket *clientmat;

    QElapsedTimer *timer;
    HINSTANCE hLib;
    inpfuncPtr inp32;
    oupfuncPtr oup32;
    QString portName = "D010";
    short portData;
    int portNumber;
    bool errorFlag;
    int screennumber;
private slots:
    void readyRead();
    void readyReadmat();
    void newConnection();
    void newConnectionmat();
    void stim1();
    void stim2();
    void stim3();
    void stim4();
    void stim11();
    void stim12();
    void stim13();
    void stim64();
    void delaycheck();

};

#endif // TCPSENDER_H
