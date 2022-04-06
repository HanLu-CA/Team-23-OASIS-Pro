#include "session.h"

Session::Session(QString type, int length, int duration) : ses_length(length), ses_duration(duration), ses_type(type){
    timer = new QTimer(this);
}

Session::~Session() {
    delete timer;
}

void Session::setSesLength(int l) { ses_length = l; }
void Session::setSesDuration(int d) { ses_duration = d; }
void Session::setSesType(QString t) { ses_type = t; }
void Session::reduceSesLength() { ses_length -= 1; }

QString Session::getSesType() { return ses_type; }
int Session::getSesLength() { return ses_length; }
int Session::getSesDuration() { return ses_duration; }
QTimer* Session::getTimer() { return timer; }


