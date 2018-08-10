#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtWidgets/QApplication>

#include <QtConcurrent/QtConcurrentRun>

#include "qhuebridgemanager.h"


int main(int argc, char** argv)
{
        QApplication a(argc, argv);
        bool isConnected = false;
        Q_UNUSED(isConnected);


        QHueBridgeManager manager;

        isConnected = QObject::connect(&manager,
                                       &QHueBridgeManager::detected,
                                       [](QHueBridge* bridge) {
                qDebug() << "Detected" << bridge->id();

                bool isConnected = false;
                Q_UNUSED(isConnected);

                isConnected = QObject::connect(bridge,
                                               &QHueBridge::error,
                                               [](const QHueError& error) {
                        qCritical() << "Error" << error.error << error.text;
                });
                Q_ASSERT(isConnected == true);

                isConnected = QObject::connect(bridge,
                                               &QHueBridge::userCreated,
                                               [](const QString& userName) {
                        qDebug() << "Created user" << userName;
                });
                Q_ASSERT(isConnected == true);

                isConnected = QObject::connect(bridge,
                                               &QHueBridge::configurationUpdated,
                                               [](const QHueBridge::Configuration& config) {
                        qDebug() << config.version;
                });
                //QObject::connect(bridge,SIGNAL() bridge, SLOT(setLight(1,false,0)));
                //bridge->setLight(1,true,50);
                bridge->updateLights();
                bridge->requestConfiguration();
        });
        Q_ASSERT(isConnected == true);

        manager.detect();

        return a.exec();
}

