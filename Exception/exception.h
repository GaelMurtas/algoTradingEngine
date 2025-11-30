#include <source_location>
#include <iostream>
#include <functional>

#ifndef _EXCEPTION
#define _EXCEPTION
class Exception {

    public:
		//addresse de l'object dans lequel l'exception est déclancher
		const void* obj;
        
        //localisation du déclanchement de l'exception
        std::source_location location;

		//passer a true pour ignorer complétement la gestion d'exception
		//À utiliser uniquement au cours du dévelloppement où dans certain cas précis pour accéléré un peu l'exécution
		static bool BYPASS;
		
		//passer a true pour renvoyer les exceptions au lieu de tenter de les gérées
		//cela donne la possibilité de façon indépendante du comportement prévu par leur classe lors de leur détections en cas de besoin
		static bool THROW;
		
		//chemin du fichier de LOG
		//static std::string path;


	protected :
		std::string message;

    enum class ExceptionType {
	    warning, //Préviens l'utilisateur d'un danger potentiel le programme tante de continuer sans changement
	    error, //on tente de continuer l'exécution mais après avoir géré l'exception en moditiant un variable par exemple
	    fatal_error //on arrête immédiatement l'exécution
    };
		ExceptionType type;
		
		//construction interdite le constructeur sera seulement appeller par les classes filles
		Exception(const void*, const std::source_location &, const ExceptionType &);
		Exception() = delete;
		
		//afficahge des information de l'erreur dans le terminal
		void affiche();
		
        //afficahge des information de l'erreur dans un fichier de LOG - À FAIRE -
		//void toLOG();
};

/*
 * Fonction de détection des exception pour les diff&rentes Classes
 */
namespace tabException
{
    void indexCheck(const void*, const std::source_location &, size_t, size_t);
    void constructorCheck(const std::source_location & , size_t &);
    /*Pas nécessaire, ici on aurrais une erreur de compilation
     * inline void typeCheck(const void* ptr, const std::source_location & location, const std::type_info & id){
        if(Exception::BYPASS) return;
        bool b = 0;
        std::string validTypes[5] = {typeid(double).name(), typeid(float).name(), typeid(size_t).name(), typeid(long int).name(), typeid(int).name()};
        for(std::string type : validTypes){
            b += (id.name() == type);
        }
        if (!b){
            forbidenType e(ptr, location, id);
            e.affiche();
            if(Exception::THROW) throw e;
            else exit(-1);
        }
    }*/

        /*
    class AccessError : public Exception{
        public:
        //vérification des paramètres des accesseurs
        static void accessCheck(const void*, const std::source_location &, size_t, size_t);

        private :
        using Exception::Exception;
        size_t size;
        AccessError(const void *, const std::source_location &, size_t);
        void accessDenied(size_t s);
    };

    class ConstructorException : public Exception{
        public:
            //vérification des paramètres du constructeur
            static void constructorCheck(const std::source_location & , size_t);
        private:
        using Exception::Exception;
        size_t size;
        ConstructorException(const std::source_location &, size_t);
        void emptyTab();
        void largeTab();

    };*/
}


/*
 * Fonction d'identification généraliste efficace, géré par template pour faire le maximum au moment de la compilation
 */

//vérification qu'une valeur donnée soit dans les bornes établies

template<bool activateException, typename valueType>
class ValueOutOfBound: private Exception{
     using Exception::Exception;
     public:
     static inline void check(const void* ptr, const std::source_location & location, const valueType & value, const valueType & min, const valueType & max){
          if(Exception::BYPASS) return;
          if ((value<min)||(value>max)){
               ValueOutOfBound e(ptr, location, ExceptionType::fatal_error);
               e.message = "La valeur ";
               e.message += std::to_string(value);
               e.message += " est en dehors des bornes prrévues:\nmin ";
               e.message += std::to_string(min);
               e.message += "\nmax ";
               e.message += std::to_string(max);
               e.affiche();
               if(Exception::THROW) throw e;
               else exit(-1);
          }
     }
};
//spécialisation pour le cas ou l'on souhaite désactivé la gestion d'exception
template<typename valueType>
class ValueOutOfBound<false, valueType> : private Exception{
     using Exception::Exception;
     public:
     static inline void check(const void* ptr, const std::source_location & location, const valueType & value, const valueType & min, const valueType & max){
     }
};

template<bool activateException, typename ... valuesTypes>
class SuspectInputs : private Exception{
     using Exception::Exception;
     public:
     static inline void check(std::source_location location, valuesTypes ... values, std::function<bool(valuesTypes ...)> condition){
          if(BYPASS) return;
          if(!condition(values ...)){
               SuspectInputs e(NULL, location, ExceptionType::warning);
               e.message = "Valeures suspectes repérées comme input d'une fonction.";
               e.affiche();
               if(THROW) throw e;
          }
     }
};
template<typename ... valuesTypes>
class SuspectInputs<false, valuesTypes ...> : private Exception{
     using Exception::Exception;
     public:
     static inline void check(std::source_location location, valuesTypes ... values, std::function<bool(valuesTypes ...)> condition){
     }
};

//exception pour tester la validitée des pointeurs
template<bool activateException, typename ptrType>
class UnexpectedEmptyPointer : private Exception{
     using Exception::Exception;
     public:
     static inline void check(std::source_location location, ptrType * ptr){
          if constexpr (activateException == 1){
               if(BYPASS || ptr) return;
               UnexpectedEmptyPointer e(ptr, location, ExceptionType::fatal_error);
               e.message = "Pointeur null inattendu détecté.";
               e.affiche();
               if(Exception::THROW) throw e;
               else exit(-1);
          }
     }
};

/*
 * les différentes exceptions du programme
 * a mettre a jour dans le style de la gestion d'exceptions amélioré ci-dessus
 */
class IndexOutOfRange : public Exception{
    friend void tabException::indexCheck(const void*, const std::source_location &, size_t, size_t);
    private :
        using Exception::Exception;
        size_t indexMax, indexPosition;
        IndexOutOfRange(const void *, const std::source_location &, size_t, size_t);
    };

class ForbidenConstruction : public Exception{
    friend void tabException::constructorCheck(const std::source_location &, size_t &);
    private :
        using Exception::Exception;
        ForbidenConstruction(const std::source_location &);
};

class SuspectConstruction : public Exception{
    friend void tabException::constructorCheck(const std::source_location &, size_t &);
    private :
        using Exception::Exception;
        SuspectConstruction(const std::source_location &);
};

/*inutes car le cas géré par cette exception cré une erreur de compilation
class ForbidenType : public Exception{
     friend void tabException::typeCheck(const void*, const std::source_location &, const std::type_info &);
    private :
        using Exception::Exception;
        ForbidenType(const void*, const std::source_location &, const std::type_info &);
};*/


//Ancien systhem a suprimer petit a petit
class TabExtException : public Exception{ 
    
    using Exception::Exception;

    private :
        size_t size; 
        size_t alloc;

        TabExtException(const void *, const std::source_location &, const ExceptionType &, const size_t &, const size_t &);
        
        void constructionFailed();

        void accessDenied(const size_t & s);

    public :
        //vérification des paramètres du constructeur
        static void constructorCheck(size_t &, const std::source_location & location);
        
        //vérification des paramètres des accesseurs
        static void accessCheck(const void*, const std::source_location &, const size_t &, const size_t &, const size_t &);
    };

#endif
