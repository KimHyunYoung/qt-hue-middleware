#ifndef QHUELIGHT_H
#define QHUELIGHT_H

#include <QObject>

class QHueLight : public QObject
{
    Q_OBJECT

public:
    explicit QHueLight(QObject* parent = nullptr);
};

#endif // QHUELIGHT_H
