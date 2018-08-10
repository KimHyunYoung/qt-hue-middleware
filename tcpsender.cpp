#include "tcpsender.h"
#include "ui_tcpsender.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtWidgets/QApplication>

#include <QtConcurrent/QtConcurrentRun>

#include "qhuebridgemanager.h"
static inline QByteArray IntToArray(qint32 source);

QByteArray IntToArray(qint32 source) {
   QByteArray temp;
   QDataStream data(&temp, QIODevice::ReadWrite);
   data << source;
   return temp;
}
class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};
tcpsender::tcpsender(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tcpsender)
{
    ui->setupUi(this);
    timer = new QElapsedTimer(); // 시간 측정기
    timer->start(); //시작
    server = new QTcpServer(this); // 서버 시작
    client = new QTcpSocket();
    client2 = new QTcpSocket(); //클라이언트 2개, 하나는 수신 하나는 발신
    servermat = new QTcpServer(this); //매트랩용 클라이언트
    if(!server->listen(QHostAddress::Any,1667)){ // 자극용 서버
        qDebug() << "TCPserver could not start";
    }
    else{
       qDebug() << "server start";
    }
    if(!servermat->listen(QHostAddress::Any,1668)){ // 매트랩용 클라이언트
        qDebug() << "TCPserver could not start";
    }
    else{
       qDebug() << "server start";
    }
    connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(servermat,SIGNAL(newConnection()), this, SLOT(newConnectionmat())); // 활성화
    hLib = LoadLibraryA("inpout32.dll"); //라이브러리 로드
    if(hLib == NULL)
    {
        errorFlag = true;
        return;
    }

    inp32 = (inpfuncPtr) GetProcAddress(hLib, "Inp32");

    if(inp32 == NULL)
    {
        errorFlag = true;
        return;
    }

    oup32 = (oupfuncPtr) GetProcAddress(hLib, "Out32");

    if(oup32 == NULL)
    {
        errorFlag = true;
    }
    bool ok;
    portNumber = portName.toInt(&ok,16); // 포트 번호는 h파일에
}

tcpsender::~tcpsender()
{
    delete ui;
}
void tcpsender::newConnection(){ //자극물 쪽의 연결
      client = server->nextPendingConnection();
      qDebug() << "connect with TCPclient";
     connect(client, SIGNAL(readyRead()),this, SLOT(readyRead()));
     client2->connectToHost("192.168.0.6", 23); //원래 정해진 주소로 연결. 원래는 이 연결된 주소로 쌍방연결을 해야함. 찾는거 할줄 모름 ㅎ...
}
void tcpsender::newConnectionmat(){//매트랩과의 연결
    clientmat = servermat->nextPendingConnection();
    qDebug() << "connect with TCPclientmat";
    connect(clientmat, SIGNAL(readyRead()),this, SLOT(readyReadmat())); //매트랩 수신부 연결

}
void tcpsender::readyReadmat(){
       QByteArray byte = clientmat->readAll(); // 모두 읽기
       QString DataAsString(byte); //데이터를 스트링으로
       int result=DataAsString.toInt(); // 스트링을 정수형으로

       if(result == 0){
           switch(positionone){ // 기록된 위치에 따라서 결과 실행
           case 1:
                emit full();
               break;
           case 2:
                emit half();
               break;
           case 3:
                emit off();
               break;
           case 4:
                emit color();
               break;
           }
           client2->write(byte);
       }
       else if(result == 1){
           switch(positiontwo){
           case 1:
                emit full();
               break;
           case 2:
                emit half();
               break;
           case 3:
                emit off();
               break;
           case 4:
                emit color();
               break;
           }
           client2->write(byte);
       }
       else if(result == 2){
           switch(positionthree){
           case 1:
                emit full();
               break;
           case 2:
                emit half();
               break;
           case 3:
                emit off();
               break;
           case 4:
                emit color();
               break;
           }
           client2->write(byte);
       }
       else if(result == 3){
           switch(positionfour){
           case 1:
                emit full();
               break;
           case 2:
                emit half();
               break;
           case 3:
                emit off();
               break;
           case 4:
                emit color();
               break;
           }
           client2->write(byte);
       }
       else if(result == 6){ // 6번은 재시작
           qDebug() << byte;
           client2->write(byte);
       }
}
void tcpsender::readyRead(){
       QByteArray byte = client->readAll();
       QString DataAsString(byte);
       QStringList datalist = DataAsString.split('#');
       int size = datalist.size();
       client->flush();
       qDebug() << DataAsString;
       for(int i = 1; i < (size/2) + 1; i++){
           //qDebug() << datalist[i*2-1];
           if(testfordelay){
               if(datalist[i*2- 1] == "x"){
                   starttime = timer->elapsed();
                   lasttime = 0;
                   outputtime = datalist[i*2-1].toInt();
                   output = 11;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "y"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 12;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "z"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 13;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "a"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 1;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "b"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 2;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "c"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 3;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "d"){
                   outputtime = datalist[i*2-2].toInt();
                   output = 4;
                   delaycheck();
               }
               else if(datalist[i*2- 1] == "M"){
                   starttime = timer->elapsed();
                   lasttime = datalist[i*2-2].toInt();
                   training = false;
                   qDebug() << " test start.";
               }
               else if(datalist[i*2- 1] == "*a"){
                   screennumber = 1;
               }
               else if(datalist[i*2- 1] == "*b"){
                   screennumber = 2;
               }
               else if(datalist[i*2- 1] == "*c"){
                   screennumber = 3;
               }
               else if(datalist[i*2- 1] == "A"){
                   switch(positionone){
                   case 1:
                        emit full();
                       qDebug() << "fullon";
                       break;
                   case 2:
                        emit half();
                       qDebug() << "halfon";
                       break;
                   case 3:
                        emit off();
                       qDebug() << "offlight";
                       break;
                   case 4:
                        emit color();
                       qDebug() << "changecolor";
                       break;
                   }
               }
               else if(datalist[i*2- 1] == "B"){
                   switch(positiontwo){
                   case 1:
                        emit full();
                       qDebug() << "fullon";
                       break;
                   case 2:
                        emit half();
                       qDebug() << "halfon";
                       break;
                   case 3:
                        emit off();
                       qDebug() << "offlight";
                       break;
                   case 4:
                        emit color();
                       qDebug() << "changecolor";
                       break;
                   }
               }
               else if(datalist[i*2- 1] == "C"){
                   switch(positionthree){
                   case 1:
                        emit full();
                       qDebug() << "fullon";
                       break;
                   case 2:
                        emit half();
                       qDebug() << "halfon";
                       break;
                   case 3:
                        emit off();
                       qDebug() << "offlight";
                       break;
                   case 4:
                        emit color();
                       qDebug() << "changecolor";
                       break;
                   }
               }
               else if(datalist[i*2- 1] == "D"){
                   switch(positionfour){
                   case 1:
                        emit full();
                       qDebug() << "fullon";
                       break;
                   case 2:
                        emit half();
                       qDebug() << "halfon";
                       break;
                   case 3:
                        emit off();
                       qDebug() << "offlight";
                       break;
                   case 4:
                        emit color();
                       qDebug() << "changecolor";
                       break;
                   }
               }
               else if(datalist[i*2- 1] == "&1"){
                   positionone = datalist[i*2-2].toInt();
                   qDebug() << "positionone = " << positionone;
               }
               else if(datalist[i*2- 1] == "&2"){
                   positiontwo = datalist[i*2-2].toInt();
                   qDebug() << "positiontwo = " << positiontwo;
               }
               else if(datalist[i*2- 1] == "&3"){
                   positionthree = datalist[i*2-2].toInt();
                   qDebug() << "positionthree = " << positionthree;
               }
               else if(datalist[i*2- 1] == "&4"){
                   positionfour = datalist[i*2-2].toInt();
                   qDebug() << "positionfour = " << positionfour;
               }
           }
       }

}
void tcpsender::delaycheck(){
   //qDebug() << "time : " << outputtime << ", output = " << output;
    if(lastanswer != output ){
        nowtime = timer->elapsed()-starttime;
        lasttime += outputtime;
        //qDebug() << "nowtime = " << nowtime << ", lasttime = " << lasttime;
       // qDebug() << " time to delay" <<700 + lasttime - nowtime;
        switch(output){
        case 1:
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim1()));
            break;
        case 2:
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim2()));
            break;
        case 3:
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim3()));
            break;
        case 4:
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim4()));
            break;
        case 11:
            stim64();
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim11()));
            break;
        case 12:
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim12()));
            break;
        case 13:
            qDebug() << "this is 13 V" << output;
            QTimer::singleShot(700 + lasttime - nowtime ,this,SLOT(stim13()));
            break;
        }
        //qDebug() << "send ->" << output;
        lastanswer = output;
    }
}

void tcpsender::stim1()
{
    (oup32)(portNumber,1);
    Sleeper::msleep(3);
}
void tcpsender::stim2()
{
    (oup32)(portNumber,2);
    Sleeper::msleep(3);
}
void tcpsender::stim3()
{
    (oup32)(portNumber,3);
    Sleeper::msleep(3);
}
void tcpsender::stim4()
{
    (oup32)(portNumber,4);
    Sleeper::msleep(3);
}
void tcpsender::stim11()
{
    (oup32)(portNumber,11);
    Sleeper::msleep(3);
}
void tcpsender::stim12()
{
    (oup32)(portNumber,12);
    Sleeper::msleep(3);
}
void tcpsender::stim13()
{
    (oup32)(portNumber,13);
    Sleeper::msleep(3);
}
void tcpsender::stim64(){
    (oup32)(portNumber,64);
    Sleeper::msleep(3);
}
