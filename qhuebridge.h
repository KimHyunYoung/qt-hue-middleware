#ifndef QHUEBRIDGE_H
#define QHUEBRIDGE_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>

#include "qhueerror.h"

class QHueBridgePrivate;


class QHueBridge : public QObject
{
        Q_OBJECT
        Q_DECLARE_PRIVATE(QHueBridge)
        Q_DISABLE_COPY(QHueBridge)

public:
        struct PortalState {
                QString communicationState;
                bool incomingEnabled;
                bool outgoingEnabled;
                bool isSignedOn;
        };

        struct WhitelistEntry {
                QString id;
                QDateTime createDateTime;
                QDateTime lastUseDateTime;
                QString name;
        };

        struct Configuration {
                QString name;

                QDateTime bridgeDateTime;
                QDateTime localDateTime;
                QString timezone;

                QString hardwareAddress;
                QString ipAddress;
                QString gatewayIpAddress;
                QString subnetIpAddress;
                QString proxyAddress;
                quint16 proxyPort;

                bool linkButtonPressed;

                QString portalConnection;
                bool portalAccountLinked;
                QHueBridge::PortalState portalState;

                QString apiVersion;
                QString version;

                QList<QHueBridge::WhitelistEntry> whitelist;

                quint8 zigbeeChannel;
                //! \todo swupdate currently not implemented.
        };

        explicit QHueBridge(QObject* parent = nullptr);

        void setId(const QString& id);
        QString id() const;

        void setIpAddress(const QString& ipAddress);
        QString ipAddress() const;

        void setHardwareAddress(const QString& hardwareAddress);
        QString hardwareAddress() const;

        void setName(const QString& name);
        QString name() const;

        void setUserName(const QString& user);
        QString userName() const;

        void setDeviceName(const QString& device);
        QString deviceName() const;

public slots:
        void createUser(const QString& applicationName,
                        const QString& deviceName);
        void requestConfiguration();

        void updateLights();

        void setLight(int id, bool onoff, quint8 brightness, int colo);
        void fullemit();
        void halfemit();
        void offemit();
        void coloremit();

signals:
        void error(const QHueError& error);
        void userCreated(const QString& userName);
        void configurationUpdated(const QHueBridge::Configuration& config);

private:
        QHueBridgePrivate* d_ptr;
};

#endif // QHUEBRIDGE_H
