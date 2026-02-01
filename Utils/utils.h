#ifndef UTILS_EXCEPTION
#define UTILS_EXCEPTION 1
#include <iostream>
#include <fstream>
#include <functional>
#include <cmath>
#include <utility>
#include <type_traits>
//#include <algorithm>
#include "../Exception/exception.h"
//#include "../Trading/trading.h"
#include "iterator.hpp"

//pre-declaration for convenace
template<typename T, unsigned V, bool B>
class TabExt;

//tableau dynamique générique basique ultra rapide
template<typename T>
class Tab{
    protected:
      T * data;//les données stocker dans un tableau standard
      size_t length;//taille des données écrites tailles du tableau alloué en mémoire

     public:
      Tab();
      Tab(size_t);
      Tab(const Tab<T> &);
      ~Tab();

      //accesseurs
      //tous en mode inline pour une performance optimale
      inline T& operator[](size_t i){
          tabException::indexCheck(this, std::source_location::current(), length, i);
          return data[i];
        }

     const T& operator[](size_t i) const{
          tabException::indexCheck(this, std::source_location::current(), length, i);
          return data[i];
        }

      T& get (size_t i){
          tabException::indexCheck(this, std::source_location::current(), length, i);
          return data[i];
      }

      const T&  getConst (size_t i) const{
          tabException::indexCheck(this, std::source_location::current(), length, i);
          return data[i];
      }

      T getCopie (size_t i) const{
          tabException::indexCheck(this, std::source_location::current(), length, i);
          return data[i];
      }

      size_t size()const{
            return length;
      }

      //accesseur indicé de 1 à n !!!
      T& get1 (size_t i){
        tabException::indexCheck(this, std::source_location::current(), length, i-1);
        return data[i-1];
        }

      const T&  getConst1 (size_t i) const{
        tabException::indexCheck(this, std::source_location::current(), length, i-1);
        return data[i-1];
        }

      T  getCopie1 (size_t i) const{
        tabException::indexCheck(this, std::source_location::current(), length, i-1);
        return data[i-1];
        }

      //get the last entree in the tab
      const T & last(){
           return getConst1(length);
      }

      //autres utilitaires
      //sipression d'un élément O(n)
      void remove(const size_t &);
      //suppression de l'intgralité du tableau, seul les éléments sont supprimer, la mémoire est toujours alloué
      void clear();
      //fonction pour obtenir une valeur minimale (ou maximale) du tableau si T est l'un des type numérique prévu pour être utiliser avec cette fonction
      //c'est bien UNE valeur minimale et non pas LA valeur minimale car < peut ne pas être un ordre parfait
      T min() const;
      T max() const;
      //on revoi vrais et la position de l'élément si il a été trouver, faux et 0 sinon
      std::pair<bool, size_t> search(const T & searched) const;

      void operator=(const Tab<T> &);
      //concaténatiàon de tableau
      void operator += (const Tab<T> &);
      Tab<T> operator + (const Tab<T> &) const;

      //Iterator interaction part
      //alias for clarty
      using iterator = Iterator<Tab<T>, T, false>;
      using constIterator = Iterator<Tab<T>, T, true>;

      iterator begin(){
           return iterator(*this);
      }

      constIterator begin() const{
            return constIterator(*this);
        }

      iterator end(){
           //astuce pour renvoyer un itérateur à la position juste après la fin du tableau en exciquant la détection d'erreur du construcetur
          return iterator(*this, length);
      }

      constIterator end() const{
           //astuce pour renvoyer un itérateur à la position juste après la fin du tableau en exciquant la détection d'erreur du construcetur
          return constIterator(*this, length);
      }

      //friendship declaration
     template<typename Y, unsigned V, bool B>
     friend class TabExt;
};

/*
 * Tableau extancible customisable
 * T représente le Type de données
 * V la vitesse à laquelle le tableau grossi lorsqu'on ajoute des éléments, on allou V fois la taille actuelle en plus (+1 pour le cas v=0 et permettre de commancer a un tabExt vide)
 * B pour sélectioner l'une des deux option suivante:
 *   toujours garder le tableau en 1 bloc comme pour std::vector (lecture O(1), ajout d'éléments O(size()) (pire des cas))
 *   fragmenté en en plusieurs tableau au besoin (lecture O(LOGbaseV(size()), ajout d'éléments O(1))
 */

//implémentation pour le cas V>1 et B = true
template<typename T, unsigned V, bool B>
class TabExt : public Tab<T>{
     using Tab<T>::length, Tab<T>::data;
     protected:
          size_t alloc;//tailles du tableau alloué en mémoire

          void extend();//multipli par V la mémoire alloué

     public:
          TabExt();
          TabExt(size_t);
          TabExt(const Tab<T> &);
          TabExt(const TabExt<T,V,B> &);

          //accesseur
          size_t allocSize(){
               return alloc;
          }

          //ajout d'un éléments en fin de Tableau
          //si besoin on étant le tableu actuel ou en cré un nouveau que l'on relie en fonction de B
          void pushBack(const T&);
          TabExt& operator + (const T&);

          void operator=(const Tab<T> &);
          void operator=(const TabExt<T, V, B> & ntab){
               operator = (static_cast<Tab<T>>(ntab));
          }
};

//cas B=false
template<typename T, unsigned V>
class TabExt<T, V, false> : TabExt<TabExt<T,V,true>, V, true>{
     /*
      * Implémentaion à faire
      */
};

//cas V=0
template<typename T>
class TabExt<T, 0, true> : Tab<T>{
     public:
          //on peut toujours ajouter un éléments mais ici le tableau sera recopier à chaque fois
          void ajouterElement(const T&);
};

template<typename T>
class TabExt<T, 0, false> : TabExt<T,0,true>{
};

//tab classes alias for convenace
template <typename T>
using tab = Tab<T>;

template <typename T>
using tabExt = TabExt<T, 2, true>;

/*
//template<typename T> class TabExtError;
//tableau Extensible rapide et pratique (une fois terminé x))
template<typename T>
class TabExt {
    protected:
      T * tab;//les données stocker dans un tableau standard
      size_t   size;//taille des données écrites
      size_t   alloc;//tailles du tableau alloué en mémoire

      void extend();//double la mémoire alloué
 
 public:
  TabExt();
  TabExt(size_t);
  TabExt(const TabExt<T> &);
  ~TabExt();
  //TabExt& operator=(const tabExt&);
  
  //accesseur indice de 0 à n-1 inline
  T& operator[](size_t i){
         tabException::indexCheck(this, std::source_location::current(), size, i);
     return tab[i];
  }
  //accesseur !!! Indice de 1 à n !!!
  T& get (const size_t &);
  const T&  getConst (const size_t &) const;
  T  getCopie (const size_t &) const;

  //size_t  RechercherElement(const T&); // return the position of the element or -1
  size_t taille()const;  
  void ajouterElement(const T&); // add an element at the end (possibly extend array) 
  TabExt& operator + (const T&); // mm chose que AjouterElement
  void suprimerElement(const size_t &);          // remove the element at the given position
  
  //friend class TabExt2Error<T>;
  friend class TabExtException;

  //iterator retated part
      template<bool Const> class Iterator;

      Iterator<false> begin(){
           return Iterator<false>(*this);
      }

      Iterator<true> begin() const{
            return Iterator<true>(*this);
        }

      Iterator<false> end(){
           //astuce pour renvoyer un itérateur à la position juste après la fin du tableau en exciquant la détection d'erreur du construcetur
          return ++Iterator<false>(*this, size-1);
      }

      Iterator<true> end() const{
           //astuce pour renvoyer un itérateur à la position juste après la fin du tableau en exciquant la détection d'erreur du construcetur
          return ++Iterator<true>(*this, size-1);
      }
};


//Itérateur de la classe tabExt
//Plus tard on remplacera les deux itérateurs (de la class tab et tabExt par un seul itérateur générique
template<class T, bool Const> struct TabExtIsConst;
template<class T> struct TabExtIsConst<T, true>{
     using type = const TabExt<T>;
};
template<class T> struct TabExtIsConst<T, false>{
     using type = TabExt<T>;
};

template<class T>
template<bool Const>
class TabExt<T>::Iterator
{
     using tabTptr = TabExtIsConst<T, Const>::type *;
     using tabTref = TabExtIsConst<T, Const>::type &;

   private :
     //indice actuel de l'itérateur
     size_t current;
     //objet auquel l'itérateur est associé
     tabTptr object;

   public:
     Iterator(tabTref obj): current(0), object(&obj){
     }

     Iterator(tabTref obj, size_t index): current(index), object(&obj){
         tabException::indexCheck(this->object, std::source_location::current(), object->size, current);
     }

     //accesseurs
     size_t index() const{
         return current;
     }

     T& operator*() const{
           return (*object).tab[current];
     }

     T* operator ->() const{
         return &(*object).tab[current];
     }

     //prefix implement
     Iterator operator ++(){
         tabException::indexCheck(this->object, std::source_location::current(), object->size, current);
         ++current;
         return *this;
     }

     //prefix implement
     Iterator operator ++(int useless){
         tabException::indexCheck(this->object, std::source_location::current(), object->size(), current);
         Iterator tmp = *this;
         ++current;
         return tmp;
     }

     bool operator ==(const Iterator & it) const{
         return (current == it.current) && (object == object);
     }

     bool operator !=(const Iterator & it) const{
         return (current != it.current) || (object != object);
     }

     bool operator <(const Iterator & it)const{
         return (current < it.current) && (object == object);
     }
 };
*/


//tableau dynamique en dimension 2
template<typename T> class TabExt2 : private tabExt<T>
{
     //A faire : 
     // operateur []
     // modifier get pour être indexé de 0 a n-1
     // créé plus d'accesseurs getconbst getref ...
     // créé accesseur indexer de 1 a n (get1, get1const ...)
     // choisir entre ajout rapide de ligne et de colone a imlémenter ou hérité durectement de tab et non tabExt
     // (sans possibilité d'ajout d'éléments le coter extensibles est inutile

  private:
    size_t nbl;//nombre de ligne
    size_t nbc;//nombre de colonnes
    
  public:
    TabExt2(const tabExt<T> &);
    TabExt2(const TabExt2<T> &);
    TabExt2(const size_t &, const size_t &);
    
    //accesseur
    T& get(const size_t &, const size_t &);
    T getCopie(const size_t &, const size_t &) const;
    size_t nLines() const;
    size_t nCols() const;

    //operators
    void operator=(const TabExt2<T> &);

    //this class from tradingBots library need to access the search function for univariate tab
    friend class tradingState;
};


//feuille de donné pouvant lire et écrire en csv
class Table : public TabExt2<std::string>
{
    using TabExt2<std::string>::TabExt2;
    using TabExt2<std::string>::operator=;

    private:
    char dl,dc,df;//les délimiteur de lignes et colonnes pour les entrer ou sorties, ainsi que la valeur par défault des donnée vides
    
    std::string toString() const;//Renvoi un string avec tout le contenu de la table

  public :
    Table(const TabExt2<std::string> &);
    Table(const size_t &, const size_t &);
    //construction à partir du contenu d'un fichier
    Table(const std::string &);// délimiteur '\n', ',' et ' ' par défault
    Table(const std::string & path, const char &, const char &, const char &);
    void CSV(const std::string &) const;//écriture du contenue dans un fichier format csv
    void affiche() const;
};

/* Une classe histogramme pour pouvoir comparé différentes distribution entre elles ou avoir une représentation simple de donnée
 * On cré aussi un classe Classe qui représente une section d'un histogramme.
 * On pourra ainsi extraire de l'information des données facillement
 */
class Histogram;
namespace histogram{
    class Classe{
        private :
            //nombre d'élément dans la classe
            size_t quantity;
            //fréquence par rapport au total d'éléments dans l'historamme
            double frequency;
            //borne inf de la classe
            double inf;
            //born sup de la classe;
            double sup;
            //milieu de la classe
            double center;
            //l'étendu de la classe
            double spread;

            //constructeur privé car cette classe ne doit être utilisé que par Histogram
            Classe();
            //constructeur paramétré initialisant les attribut
            //prise en conte de quantity, inf, supp et totalElements de histo
            Classe(size_t, double, double, size_t);
            //on veut pouvoir savoir si deux classe sont les mêmes
            bool operator ==(const Classe &) const;

            friend Histogram;
            friend Tab<Classe>;

        public :
            void print() const;
    };
}

class Histogram : public Tab<histogram::Classe> {
    /*
     * le tableau de donnée a représenter sera divviser en un certain nombre de classes séparé.
     * Pour chaques classes on contera le nombre d'éléments dans cette classe.
     * C'est à dire le nombre d'éléments dont la valeur est entre le nim et le max de la classe.
     */
    private :
        //nombre d'éléments représenté au total dans l'histogramme
        size_t totalElements;
        //formule de calcul du nombre de classe de l'histogramme
        //On veut pouvoir en changer au besoin
        static std::function<size_t(size_t)> nbClassesFonction;
        //fonction de comparaison entre deux histogrammes
        //On veut pouvoir comparer deux histogramme idéalement le plus différents possibles, par exemples avec des tailles de classe différentes
        //On obtien alors un score de différence, il est plus bas lorsque deux histogramme se ressemble
        //De plus on veut pouvoir changer la fonction de comparaison au besoin
        static std::function<double(const Histogram &, const Histogram &)> compairFonction;

        Table toTable();

    public :
        Histogram(Tab<double>);
        
        //On veut pouvoir accéder à la classe contenant un valeur donné
        const histogram::Classe getClasse(double) const;
        //estimation de la prob qu'une valeure soit dans un intervale
        double probaInterval(double borneInf, double borneSup) const;

        void print();
        void CSV(const std::string &);

        //utilise la fonction de comparaison stocker dans compairFonction pour comparer deux histogramme.
        static inline double compair(const Histogram & h1, const Histogram & h2){
              return compairFonction(h1, h2);
        }

        //fonctions pour changer de fonction de comparaison ou de calcul du nombre de classe au besoin
        static std::function<size_t(size_t)> switchFonctionNbClasse(size_t i);
        static std::function<double(const Histogram &, const Histogram &)> switchFonctionCompair(size_t i);
};

/*
 * FONCTIONS UTILITAIRES
 */

//revoi un string avec le contenu d'un ficher
std::string readFile(const std::string &);

//nombre de lignes d'un fichier delL de délimiteur des lignes
size_t getFileLines( const std::string & path, const char & delL);//nombre de lignes d'un fichier delL de délimiteur des lignes
size_t getFileCols( const std::string & path, const char & delL, const char & delC);

#include "tab.hpp"
#include "tabExt.hpp"
#include "tabExt2.hpp"
#endif
