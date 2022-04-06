#ifndef THERAPY_H
#define THERAPY_H

#include <QObject>
#include <QString>
#include <QTimer>
using namespace std;

class Therapy: public QObject {

    Q_OBJECT

    private:
        int ses_length;
        QString ses_type;
        QTimer *timer;

    public:
        Therapy(QString ses_type, int ses_length);
        ~Therapy();

        void setSesLength(int);
        void setSesType(QString);
        void reduceSesLength();

        int getSesLength();
        QString getSesType();
        QTimer* getTimer();
};

#endif // THERAPY_H
