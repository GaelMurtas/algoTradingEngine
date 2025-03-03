Chart::Chart(const TimeFrame & tf,const size_t & size): time(tf), tabExt<Bougie>::TabExt(size){
    //ajouter gestion d'exception et utiliser ce constructeur dans les autres
    //cout << "\n Chart created with size " << size << "\n";
}

Chart::Chart(const Table & ntab, const TimeFrame & tf): tabExt<Bougie>::TabExt(ntab.nLines()){
    if(ntab.nCols() != 4){
        ChartException e(this);
        e.constructionImpossible();
    }
    time = tf;
    for(size_t i=1; i<=ntab.nLines(); i++){
        Bougie B(stod(ntab.getCopie(i,1)),stod(ntab.getCopie(i,2)),stod(ntab.getCopie(i,3)),stod(ntab.getCopie(i,4)));
        get(i) = B;
    }
}

namespace{
    Table chartConstHelp(const string & path){
        Table tmp(path);
        return tmp;
    }
}

Chart::Chart(const std::string & path, const TimeFrame & tf): Chart::Chart(chartConstHelp(path), tf){
}

Table Chart::toTab() const{
    Table T(size(),4); //std::cout << "\n" << taille() << std::endl;
    for (size_t i=0; i < size(); i++){
        T.get(i,1) = to_string(double(getCopie(i).open));
        T.get(i,2) = to_string(getCopie(i).high);
        T.get(i,3) = to_string(getCopie(i).low);
        T.get(i,4) = to_string(getCopie(i).close);
    }
    return T;
}

void Chart::toFile(const string & path) const{
    toTab().CSV(path);
}

TimeFrame Chart::timeFrame() const{
    return time;
}

Tab<double> Chart::returns(){
     Tab<double> rtn(size()-1);
     for(size_t i = 0; i < (size()-1); ++i){
          rtn[i] = get(i).close - get(i+1).close;
     }
     return rtn;
}

double Chart::mean(size_t l){
     double m(0);
     for(size_t i = 0; i < size(); ++i){
          m += get(i).high + get(i).low +get(1).close;
     }
     return m/(3*size());
}

