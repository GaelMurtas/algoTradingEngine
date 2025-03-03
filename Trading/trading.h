#ifndef TRADING
#define TRADING

#include <iostream>
#include <string>
#include <fstream>

#include "../Utils/utils.h"

class Bougie
{
 public:
  double open;
  double close;
  double high;
  double low;

  Bougie();
  Bougie(const double &, const double &, const double &, const double &);
  Bougie(const std::string &, const std::string &, const std::string &, const std::string &);
  
  std::string color() const;
  
  void affiche() const;//affichage dans le terminal des informations de la bougie
  
  void check();// vérification de conformité de la bougie

  bool operator ==(const Bougie & b){
       return (open==b.open)*(close==b.close)*(high==b.high)*(low==b.low);
  }
  bool operator !=(const Bougie & b){
       return !(*this==b);
  }
};

//Simple enumeration des unitées basique de temps
enum class TimeUnit{m, h, d, w};


class TimeFrame
{
  private:
  TimeUnit unit;
  size_t nbUnit;
  
  public:
    TimeFrame();
    TimeFrame(const TimeFrame &);
    TimeFrame(const TimeUnit &, const size_t &);
    TimeFrame(const size_t &, const TimeUnit &);

  //durée d'une time frame en fonction d'une autre, 1 minutes par défault
  size_t duree() const;
  size_t duree(const TimeFrame &) const;
  
  //renvoie un string avec le nom de la time frame
  std::string toString() const;
  void affiche() const;
  
  //surcharge d'opérateur
  TimeFrame operator = (const TimeFrame &);
};

class Chart : public tabExt<Bougie>
{
  private : 
    TimeFrame time;
    
  public :
    Chart(const TimeFrame &,const size_t &);
    Chart(const Table &, const TimeFrame &);
    Chart(const std::string &, const TimeFrame &);//construction à partir d'un fichier
    
    //Changement de time frame(A faire Plus tard)
    Chart exportTimeFrame(const TimeFrame &) const;//export dans une time frame différente
    void changeTimeFrame(const TimeFrame &);//changement de time frame de l'object courant

    //fonction d'export
    Table toTab() const;
    void toFile(const std::string &) const;
    
    //accesseurs
    TimeFrame timeFrame() const;
    
    //technical info function
    //tab with returns of the charts (difference beetwen closes)
    Tab<double> returns();
    //valeur moyenne des dernières bougies
    double mean(size_t);
};

#endif
