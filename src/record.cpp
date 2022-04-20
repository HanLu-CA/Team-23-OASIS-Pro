#include "record.h"

Record::Record(QString type, int len, QString in, QString time):
    session_type(type),
    session_length(len),
    session_int(in),
    session_time(time)
{}

Record::~Record() {}

QString Record::getSesType() { return session_type; }
int Record::getSesLen() { return session_length; }
QString Record::getSesInt() { return session_int; }
QString Record::getSesTime() { return session_time; }
