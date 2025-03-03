#ifndef UTILSTMPEXC
#define UTILSTMPEXC

/*
 * GESTION DES EXEPTIONS POUR LES TEMPLATES DE LA BIBLIOTHÈQUE UTILS.H
 * Un fichier a part est nécessaire pour éviter des problème de double inclusion de la partie non template
 */ 

template<typename T>
class TabExt2Error { 
    private :
        size_t size; 
        size_t alloc;
        size_t nbl;
        size_t nbc;
  
    public :

        TabExt2Error(const size_t & s, const size_t & a):
            size(s), alloc(a), nbl(s), nbc(1) {};
    
        TabExt2Error(const size_t & s, const size_t & a, const size_t & l, const size_t & c):
            size(s), alloc(a), nbl(l), nbc(c) {};
    
        TabExt2Error(const TabExt<T> & tab){
            TabExt2Error(tab.size, tab.alloc);
        };

        TabExt2Error(const TabExt2<T> & tab):
            TabExt2Error(tab.size, tab.alloc, tab.nbl, tab.nbc){};


        void affiche(){
            std::cout << "size : " << size << std::endl << "alloc : " << alloc << std::endl << "nbl : " << nbl << std::endl << "nbc : " << nbc << std::endl;}
    
        void constructionFailed(){
            std::cout << "Échec de la construction de l'objet TabExt avec les paramètes : " << std::endl;
            affiche();
            throw *this;
        };

        void accessDenied(const size_t & s){
            std::cout << "Echec de l'accès à l'élément "<< s <<" de l'objet TabExt avec les paramètes : " << std::endl;
            affiche();
            throw *this;
        };

        void accessDenied(const size_t & s1,const size_t & s2){
            std::cout << "Echec de l'accès à l'élément ("<< s1 << "," << s2 <<") de l'objet TabExt avec les paramètes : " << std::endl;
            affiche();
            throw *this;
        };
    };

#endif
