#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QTimer>
using namespace std;

class Session: public QObject {

    Q_OBJECT

    private:
        int ses_length;
        int ses_duration;
        QString ses_type;
        QTimer *timer;

    public:
        Session(QString, int, int);
        ~Session();

        void setSesLength(int);
        void setSesDuration(int);
        void setSesType(QString);
        void reduceSesLength();

        int getSesLength();
        int getSesDuration();
        QString getSesType();
        QTimer* getTimer();
};

#endif // SESSION_H
