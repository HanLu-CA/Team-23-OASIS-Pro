#ifndef RECORD_H
#define RECORD_H

#include <QString>

class Record {

private:
    QString session_type;
    int session_length;
    QString session_int;
    QString session_time;

public:
    Record(QString,int,QString,QString);
    ~Record();

    QString getSesType();
    int getSesLen();
    QString getSesInt();
    QString getSesTime();
};

#endif // RECORD_H
