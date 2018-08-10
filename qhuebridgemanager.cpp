#include "qhuebridgemanager.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QList>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


class QHueBridgeManagerPrivate : public QObject
{
        Q_OBJECT
        Q_DECLARE_PUBLIC(QHueBridgeManager)
        Q_DISABLE_COPY(QHueBridgeManagerPrivate)

public:
        QNetworkAccessManager* qnam;

        QHueBridgeManager::DetectionMethod detectionMethod;
        QList<QHueBridge*> bridges;

        explicit QHueBridgeManagerPrivate(QHueBridgeManager* parent);
        virtual ~QHueBridgeManagerPrivate();

public slots:
        void handleSslErrors(QNetworkReply* reply,
                             const QList<QSslError>& errors);
        void handleReply(QNetworkReply* reply);

private:
        QHueBridgeManager* q_ptr;
};

#include "qhuebridgemanager.moc"


QHueBridgeManagerPrivate::QHueBridgeManagerPrivate(
                QHueBridgeManager* parent) :
        QObject(parent),
        detectionMethod(QHueBridgeManager::NoAutoDetection),
        q_ptr(parent)
{
        bool isConnected = false;
        Q_UNUSED(isConnected);


        qnam = new(std::nothrow) QNetworkAccessManager(this);
        Q_CHECK_PTR(qnam);

        isConnected = QObject::connect(
                              qnam,
                              SIGNAL(sslErrors(QNetworkReply*,
                                               QList<QSslError>)),
                              this,
                              SLOT(handleSslErrors(QNetworkReply*,
                                                   QList<QSslError>)));
        Q_ASSERT(isConnected == true);

        isConnected = QObject::connect(qnam,
                                       SIGNAL(finished(QNetworkReply*)),
                                       this,
                                       SLOT(handleReply(QNetworkReply*)));
        Q_ASSERT(isConnected == true);
}

QHueBridgeManagerPrivate::~QHueBridgeManagerPrivate()
{
        qDeleteAll(bridges);
}

void QHueBridgeManagerPrivate::handleSslErrors(QNetworkReply* reply,
                                               const QList<QSslError>& errors)
{
        qWarning() << "Ignoring SSL errors.";
        reply->ignoreSslErrors(errors);
}

void QHueBridgeManagerPrivate::handleReply(QNetworkReply* reply)
{
        Q_Q(QHueBridgeManager);

        if (detectionMethod == QHueBridgeManager::NUPnP) {
                QJsonParseError jsonError;
                QJsonDocument document = QJsonDocument::fromJson(
                                                 reply->readAll(),
                                                 &jsonError);
                if (jsonError.error != QJsonParseError::NoError) {
                        qCritical() << jsonError.errorString();
                        return;
                }

                QJsonArray bridges = document.array();
                foreach (QJsonValue bridgeValue, bridges) {
                        QJsonObject bridgeData = bridgeValue.toObject();

                        QHueBridge* bridge =
                                        new(std::nothrow) QHueBridge(this);
                        Q_CHECK_PTR(bridge);

                        bridge->setId(bridgeData.value("id").toString());
                        bridge->setName(bridgeData.value("name").toString());
                        bridge->setIpAddress(bridgeData.value(
                                                     "internalipaddress")
                                             .toString());
                        bridge->setHardwareAddress(bridgeData.value(
                                                           "macaddress")
                                                   .toString());

                        this->bridges.append(bridge);

                        emit q->detected(bridge);
                }
        }
        //! \todo Currently no other auto-detection method is supported.
        else {
                qCritical() << "Currently only N-UPnP is supported!";
                return;
        }
}


QHueBridgeManager::QHueBridgeManager(QHueBridgeManager::DetectionMethod m,
                                     QObject* parent) :
        QObject(parent),
        d_ptr(new QHueBridgeManagerPrivate(this))
{
        setDetectionMethod(m);
}

void QHueBridgeManager::detect()
{
        Q_D(QHueBridgeManager);

        if (d->detectionMethod == QHueBridgeManager::NUPnP)
                d->qnam->get(QNetworkRequest(
                                     QUrl("https://www.meethue.com/"
                                          "api/nupnp")));
        //! \todo Currently no other auto-detection method is supported.
        else {
                qCritical() << "Currently only N-UPnP is supported!";
                return;
        }

}

QHueBridgeManager::DetectionMethod QHueBridgeManager::detectionMethod() const
{
        Q_D(const QHueBridgeManager);
        return d->detectionMethod;
}

void QHueBridgeManager::setDetectionMethod(
                QHueBridgeManager::DetectionMethod m)
{
        Q_D(QHueBridgeManager);
        d->detectionMethod = m;
}
