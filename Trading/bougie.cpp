using namespace std;

Bougie::Bougie(){
open=0;
close=0;
high=0;
low=0;
};

Bougie::Bougie(const  double & o, const double & h, const double & l, const double & c){
    open=o;
    close=c;
    high=h;
    low=l;
    //vérification de validité de la bougie
    bool b = (low<=open)&&(low<=close)&&(low<=high)&&(high>=low)&&(high>=open);
    if(!b){
        Bougie_Incorrecte e(* this);
        e.Message();
        throw e;}
};

Bougie::Bougie(const  string & o, const string & c, const string & h, const string & l): Bougie::Bougie(stod(o), stod(c), stod(h), stod(l))
{};

void Bougie::check(){
    bool b = (low<=open)&&(low<=close)&&(low<=high)&&(high>=low)&&(high>=open);
    if(!b){
        Bougie_Incorrecte e(* this);
        e.Message();
        throw e;}
}

string Bougie::color() const{
  if(open<close){return "green";}
  if(open>close){return "red";}
  else return "grey";
}

void Bougie::affiche() const{
    cout << " Open " << open << ", Close " << close << ", High " << high << ", Low " << low << ", Color " << color() <<"\n";}
