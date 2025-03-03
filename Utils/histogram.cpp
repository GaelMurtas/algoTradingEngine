using namespace histogram;

Classe::Classe(){
}

Classe::Classe(size_t initQuantity, double initInf, double initSup, size_t totalElements):
     quantity(initQuantity), inf(initInf), sup(initSup), center((initInf+initSup)/2), frequency((double)initQuantity/(double)totalElements), spread(initSup - initInf){
}

bool Classe::operator ==(const Classe & c) const{
     return (quantity == c.quantity)&&(inf == c.inf)&&(sup == c.sup);
}

void Classe::print() const{
     cout << "----------\n" << "histogram::Classe : \n" << " inf : " << inf << " sup : " << sup << " quantity : " << quantity << " frequancy : " << frequency << " center : " << center << " spread : " << spread << endl << "----------\n";
}

Histogram::Histogram(Tab<double> inputData) : totalElements(inputData.size()), Tab<histogram::Classe>::Tab(nbClassesFonction(inputData.size())){
    Tab<double> separators(size()+1);
    double minValue = inputData.min();
    double maxValue = inputData.max();
    double spread = (maxValue-minValue)/size();
    for (size_t i = 0; i < size(); i++){
        double classeMin = minValue + i*spread;
        double classeMax = classeMin + spread;
        size_t quantityCount = 0;
        for(auto val : inputData){
            if ((val >= classeMin) && (val < classeMax)){
                quantityCount++;
            }
        }
        if(i == size()){
            quantityCount++;
        }
        (*this)[i] = Classe(quantityCount, classeMin, classeMax, totalElements);
        //(*this)[i].print();
    }
}

Table Histogram::toTable(){
     Table histoTable(4, size()+1);
          histoTable.get(1, 1) = "Borne inférieure";
          histoTable.get(2, 1) = "Borne supérieure";
          histoTable.get(3, 1) = "quantité";
          histoTable.get(4, 1) = "fréquence";
     for(size_t i=0; i < size(); i++){
          histoTable.get(1, i+2) = to_string((*this)[i].inf);
          histoTable.get(2, i+2) = to_string((*this)[i].sup);
          histoTable.get(3, i+2) = to_string((*this)[i].quantity);
          histoTable.get(4, i+2) = to_string((*this)[i].frequency);
     }
     return histoTable;
}

const histogram::Classe Histogram::getClasse(double x) const{
     for(constIterator it = begin(); it != end(); ++it){
          if((it->inf <= x) && (it->sup >= x)){
               return *it;
          }
     }
     ValueOutOfBound<UTILS_EXCEPTION, double>::check(this, source_location::current(), x, getConst(0).inf, getConst1(size()).sup);
     return Classe();//ce cas n'arrive jamais en pratique car il est empéché par la gestion d'exception
}


double Histogram::probaInterval(double borneInf, double borneSup) const{
     SuspectInputs<UTILS_EXCEPTION, double, double>::check(source_location::current(), borneInf, borneSup, [](double borneInf, double borneSup){return borneInf < borneSup;});
     borneInf = std::max(borneInf, getConst(0).inf);
     borneSup = std::min(borneSup, getConst1(size()).sup);
     if(borneSup <= borneInf) return 0;
     //maintenant on est sur que tout est en ordre pour commencer le calcul
     constIterator it1(*this, search(getClasse(borneInf)).second);
     constIterator it2(*this, search(getClasse(borneSup)).second);
     if(it1 == it2){
          return it1->frequency * (borneSup-borneInf)/ it1->spread;
          }
     double prob = it1->frequency * (it1->sup - borneInf)/ it1->spread;
     prob += it2->frequency * (borneSup - it2->inf)/ it2->spread;
     while((++it1)<it2){
          prob += it1->frequency;
     }
     return prob;
}

void Histogram::print(){
     cout << "----------" << endl << "Histogram :\n";
     toTable().affiche();
}

void Histogram::CSV(const std::string & path){
     toTable().CSV(path);
}

std::function<size_t(size_t)> Histogram::switchFonctionNbClasse(size_t i){
     tabException::indexCheck(NULL, source_location::current(), 3, i);
     vector<function<size_t(size_t)>> nbClassesFonctions;
     //définition des fonctions possibles
     //basé sur la racine créé, à utilisé si l'on souhaite avoir beaucoup de classe lorsqu'il y a un grand nombre d'éléments
     nbClassesFonctions.push_back([](size_t n) -> size_t{return sqrtl(n)/2+1;});
     //basé sur la racine cubique, à utiliser si l'on veut un grand nombre de classes alors qu'il y a peu d'éléments
     nbClassesFonctions.push_back([](size_t n) -> size_t{return 3*cbrtl(n)+1;});
     //basé sur le logarithme, a utiliser si l'on souhaite avoir le moins de classes possibles
     nbClassesFonctions.push_back([](size_t n) -> size_t{return logl(n)+1;});
     //mise en place de la fonction voulue
     nbClassesFonction = nbClassesFonctions[i];
     return nbClassesFonctions[i];
}

std::function<double(const Histogram &, const Histogram &)> Histogram::switchFonctionCompair(size_t i){
     tabException::indexCheck(NULL, source_location::current(), 2, i);
     vector<function<double(const Histogram & h1, const Histogram & h2)>> compairFonctions;
     //définition des fonctions possibles
     //fonction basé sur les moindre carré
     compairFonctions.push_back([](const Histogram & h1, const Histogram & h2) -> double{
          double score(0);
          for(auto it = h1.begin(); it != h1.end(); ++it){
               double proba = h2.probaInterval(it->inf, it->sup);
               score += pow(it->frequency - proba, 2);
               //cout << score <<" ";
               }
          for(auto it = h2.begin(); it != h2.end(); ++it){
               double proba = h1.probaInterval(it->inf, it->sup);
               score += pow(it->frequency - proba, 2);
               //cout << score <<" ";
               }
          return score;
          });
     //fonction basé sur un algorithme perso, pas d'estimation de proba, tout est basé sur les données réelles, en revenche il peut nous données une différence nulle pour des histo différents mais qui se ressemeble trop
     compairFonctions.push_back([](const Histogram & h1, const Histogram & h2) -> double{
          //A FAIRE : cet algorithme pourrais être plus rapide car il n'est pas vraiment nécessaire de vérifier toutes les classes à la deuxième loop
          double score = 0;
          for(auto it = h1.begin(); it != h1.end(); ++it){
               Classe i1 = *it;
               double somme = 0;
               for(Classe i2 : h2){
                    //une classe fait partie des classes qui en recouvre une autre si la condition suivante est vérifier
                    bool condition1 = (it.index() == 0) && (i2.inf < i1.sup);
                    bool condition2 = (it.index() == (h1.size()-1)) && (i2.sup > i1.inf);
                    bool condition3 = (i2.sup > i1.inf) && (i2.inf < i1.sup);
                    if(condition1 || condition2 || condition3){
                         somme += i2.frequency;
                    }
               }
               if (somme < i1.frequency){
                    score += (i1.frequency - somme);
               }
               //cout << score <<" ";
          }
          //Pour améliorer la précision on répète de processus en inversant le role chaque histogramme
for(auto it = h2.begin(); it != h2.end(); ++it){
               Classe i1 = *it;
               double somme = 0;
               for(Classe i2 : h1){
                    //une classe fait partie des classes qui en recouvre une autre si la condition suivante est vérifier
                    bool condition1 = (it.index() == 0) && (i2.inf < i1.sup);
                    bool condition2 = (it.index() == (h2.size()-1)) && (i2.sup > i1.inf);
                    bool condition3 = (i2.sup > i1.inf) && (i2.inf < i1.sup);
                    if(condition1 || condition2 || condition3){
                         somme += i2.frequency;
                    }
               }
               if (somme < i1.frequency){
                    score += (i1.frequency - somme);
               }
               //cout << score <<" ";
          }
          return score;
          });
     //fonction basé sur l'enropie croisée
     /*compairFonctions.pushback([](const Histogram & h1, const Histogram & h2) -> double{
               //A CORRIGÉ: ne pas utiliser les centres de classes mais le cumul des fréquences du plus petit ensemble de classes qui de l'autre histo qui contien la classes
               //plus tard on pourra chercher un formule plus exacte
               double entropieH1(0), entropieH2(0), entropieH1H2(0), entropieH2H1(0);
               for(auto x: h1){
                    entropieH1 += x.frequency * log(h1.getClasse(x.centre).frequency);
                    }
               for(auto x: h2){
                    entropieH2 += x.frequency * log(h2.getClasse(x.centre).frequency);
                    }
               for(auto x: h1){
                    entropieH1H2 += x.frequency * log(h2.getClasse(x.centre).frequency);
                    }
               for(auto x: h2){
                    entropieH2H1 += x.frequency * log(h1.getClasse(x.centre).frequency);
                    }
               return entropieH1H2 + entropieH2H1 - entropieH1 - entropieH2;
          });*/
     //mise en place de la fonction voulue
     compairFonction = compairFonctions[i];
     return compairFonctions[i];
}

//Initialisation des membres statics
std::function<size_t(size_t)> Histogram::nbClassesFonction = Histogram::switchFonctionNbClasse(0);
std::function<double(const Histogram &, const Histogram &)> Histogram::compairFonction = Histogram::switchFonctionCompair(0);
//std::function<size_t(size_t)> Histogram::nbClassesFonction = Histogram::switchFonctionNbClasse(0);
//std::function<double(const Histogram &, const Histogram &)> Histogram::compairFonction = Histogram::switchFonctionCompair(0);
