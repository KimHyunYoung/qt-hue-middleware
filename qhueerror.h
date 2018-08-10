#ifndef QHUEERROR_H
#define QHUEERROR_H

#include <QtCore/QString>


class QHueError
{
public:
        enum ErrorType {
                LinkButtonNotPressed = 101
        };

        QHueError::ErrorType error;
        QString address;
        QString text;
};

#endif // QHUEERROR_H
