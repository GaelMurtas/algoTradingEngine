TimeFrame::TimeFrame(const TimeUnit & t, const size_t & s):unit(t), nbUnit(s){
    if (s < 1){
        TimeFrameException::constructionImpossible(t,s);
    }
}

TimeFrame::TimeFrame(const size_t & s, const TimeUnit & t): TimeFrame::TimeFrame(t,s){}

TimeFrame::TimeFrame(const TimeFrame & tf){
    unit = tf.unit;
    nbUnit = tf.nbUnit;
}

TimeFrame::TimeFrame(): TimeFrame::TimeFrame(TimeUnit::m,1){}

size_t TimeFrame::duree() const{
    size_t t = 1;
    switch (unit) {
        case TimeUnit::h : t = 60;
                           break;
        case TimeUnit::d : t = 60*24;
                           break;
        case TimeUnit::w : t = 60*24*7;
                           break;
    }
    return nbUnit*t;
}

size_t TimeFrame::duree(const TimeFrame & tf) const{
    if (duree() % tf.duree() != 0){
        TimeFrameException e(*this);
        e.dureeIndivisibles(tf);}
    return duree()/tf.duree();
}

std::string TimeFrame::toString() const{
    std::string s = " m";
    switch (unit) {
        case TimeUnit::h : s = " h";
                     break;
        case TimeUnit::d : s = " d";
                     break;
        case TimeUnit::w : s = " w";
            }
    return std::to_string(nbUnit) + s;
}

void TimeFrame::affiche() const{
    std::cout << "\t" << toString() <<"\n";
}


TimeFrame  TimeFrame::operator = (const TimeFrame & tf){
    TimeFrame tmp(tf);
    return tf;
}
