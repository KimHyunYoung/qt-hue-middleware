#ifndef QHUEBRIDGEMANAGER_H
#define QHUEBRIDGEMANAGER_H

#include <QObject>

#include "qhuebridge.h"

class QHueBridgeManagerPrivate;


class QHueBridgeManager : public QObject
{
        Q_OBJECT
        Q_DECLARE_PRIVATE(QHueBridgeManager)
        Q_DISABLE_COPY(QHueBridgeManager)

public:
        enum DetectionMethod {
                NoAutoDetection,
                UPnP, //!< \todo Currently not supported.
                NUPnP,
                IPScan //!< \todo Currently not supported.
        };

        explicit QHueBridgeManager(
                        QHueBridgeManager::DetectionMethod method =
                                QHueBridgeManager::NUPnP,
                        QObject* parent = nullptr);

        QHueBridgeManager::DetectionMethod detectionMethod() const;

public slots:
        void detect();

        void setDetectionMethod(QHueBridgeManager::DetectionMethod m);

signals:
        void detected(QHueBridge* bridge);

private:
        QHueBridgeManagerPrivate* d_ptr;
};

#endif // QHUEBRIDGEMANAGER_H
