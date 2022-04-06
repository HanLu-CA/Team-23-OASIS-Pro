#include "therapy.h"

Therapy::Therapy(QString type, int length) {
    ses_type = type;
    ses_length = length;
    timer = new QTimer(this);
}

Therapy::~Therapy() {
    delete timer;
}

void Therapy::setSesLength(int l) { ses_length = l; }
void Therapy::setSesType(QString t) { ses_type = t; }
void Therapy::reduceSesLength() { ses_length -= 1; }

QString Therapy::getSesType() { return ses_type; }
int Therapy::getSesLength() { return ses_length; }
QTimer* Therapy::getTimer() { return timer; }


