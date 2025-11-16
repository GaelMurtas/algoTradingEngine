/**********
 ********** BIBLIOTHEQUE GENERARALISTE POUR LES BOTS DE TRADINGS
 **********/

#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>
#include <concepts>
#include <type_traits>
#include "../Trading/trading.h"
#include "../Math/math.h"

 //variable servant à activer le tarding réel ou à a compiler la versio d'entrainement
 #ifndef REAL_TRADING
 #define REAL_TRADING 0

//activation des exceptions spécifique à cette bibliothèque
#define TRADINGBOTS_EXCEPTION 1

//Option permettant de suivre l'ordre d'appel des fonctions dans le programme
//utilisé pour débugage
 #define TRADINGBOTS_DEBUG 0
 #if TRADINGBOTS_DEBUG
 #define FUNCTION_FOLLOW() functionFollow(std::source_location::current());
 #else
 #define FUNCTION_FOLLOW() emptyFunction();
 #endif
 
//mettre cette fonction dans bibliothèque utils
inline void functionFollow(const auto location){
	std::cout << "function `" << location.function_name()
              << "` form file `" << location.file_name() << " (line - "
              << location.line() << ")`"
              << " called\n";
}

inline void emptyFunction(){
}

//Les différents événements qui nécessite une réaction de la part d'un bot de trading
enum class Event{
     newCandle, successfullBought, successfullSold
};

/*
 * classe mère pour les trading bots
 * Chaque bot aura sa déclaration dans un fichier spécialisé
 */
class TradingBot: public Chart{
     public:
     //nombre de bougie à utiliser dans l'historique
     static constexpr unsigned windowSize = 20;
     private:
     //fichier contenant les données historiques
     std::string dataPath;

     protected:
     //Ajoute au tableau une nouvelle bougie si il y en a bien une dans le fichier de donnée
     //si il n'y a pas de nouvelle bougie on créé une exception
     void addLastCandle(){
          ////version temporaire, on fera plus rapide, de plus il faudra géré la prise en compte de windowSize.
          Chart newCandleCheckChart(dataPath, timeFrame());
          if ((size()<newCandleCheckChart.size())&&(get(size())!=newCandleCheckChart.get(size()+1))){
               pushBack(newCandleCheckChart.get(size()+1));
          }
          //A FAIRE: warning si pas de bougie ajouté
     }


     public:
     //simple constructeur qui sera réutiliser dans la construction des classes filles
     TradingBot(const std::string & path, TimeFrame tf): dataPath(path), Chart(path, tf){
          //erreur à renvoyer le nombre de bougie est inférieu à windowSize.
     }

     //ce que l'algo doit faire suite à la détection d'un événement
     //Dans cette classe qui n'as pas vocation à être utiliser directement on déclare une erreur si cette fonctions est appeller
     //En effet ce sont dans les classes filles que l'on va géré les évément via des spécialisations de cette méthode
     //Si cette fonction est appeller c'est donc qu'un évément non prévu tente d'être géré par un bot
     template<Event event, class ... types> void onEvent(const types & ...){
          std::cout << "Evenement inconu détecté !\n";
          exit(-1);
     }
};

//Pour chaque algorithme qui tournera il nous faudra un et un seul EventScan
template<TradingBot* bot>
class EventScan{
     public:
     //on lance la détection d'events
     static void launch();

     private:
     //fichier a scaner en continue, on regarde le fichier jusqu'à détecter le code d'un event
     static const std::string path;
     //ici on veut un signlon car on ne souhaite pas que plusieurs gestionaire d'events puisse exister sur le mm algo
     static EventScan* instance;
     //condient de temps attende avant de revérifier le fichier en seconde
     inline static const double time = 10;

     EventScan();
     ~EventScan();

     //renvoie l'évent correspondant au code
     static Event eventMap(const std::string &);
     //lance la boucle infini de scan de fichier
     void scan();
};

/* 
 * Object représentant le trade final que l'on souhaite passer à un instant t.
 * Le premiers argument du template représente si le signal doit être exécuter en réel
 * ou bien si il est exécuter sur les données d'entrainement.
 * Les autres arguments sont pour le contenu du signal qui diffère en fonction des algos
 */

///le concept de tradidingBot_type représente les différent types de trading bot
template <class T>
concept tradidingBot_type = std::is_base_of<TradingBot, T>::value;

//empty Class use for concepts and gerericity proposes
class iSignal{
};

//to later use any trading signal as template parameter
template <class T>
concept signal_type = std::is_base_of<iSignal, T>::value;

//main tradibg signal class, the generic one
template<bool real, tradidingBot_type bot, class ... args>
class Signal : public std::tuple<args ...>, iSignal{
     public :
          Signal();
          Signal(args ...);
     private :
          //fonction d'envoi du signal appeller automatique à la construction
          //static to be able to use in constructor
          static void send(Signal<real, bot, args ...>);
};

//To differenciate the buy and the sell signal
enum class orderType{
     buy, sell
};

//basic limit order, could be used by many trading bots
//long ou short, quantity, price
template<bool real, tradidingBot_type bot>
class LimitOrder : public Signal<real, bot, orderType, double, double>{
     using Signal<real, bot, orderType, double, double>::Signal;
     public:
          orderType getType() const{//true for buy false for sell
               return get<0>(*this);
          }
          double getLot() const{//true for buy false for sell
               return get<1>(*this);
          }
          double getPrice()const {//true for buy false for sell
               return get<2>(*this);
          }

          //simple print for debug
          void print() const;

     //to alow motherclass convertion
     LimitOrder(const Signal<real, bot, orderType, double, double> &);
};

//Limit order but we ensure to be unique
//it can be only one order of the same type in the eatch trading bot in a given market
template<bool real, tradidingBot_type bot>
class UniqueOrder : public LimitOrder<real, bot>{
     using LimitOrder<real, bot>::LimitOrder;
     public:
          //we redifind the send to automaticly remove older order when a new one is sending.
          static void send(UniqueOrder<real, bot>);
};

//alias to map the corret signal type to eatch trading bot type
//default comportent useless, made to use only the spesialisation
//spesialisation define in eatch trading bots own library
template<tradidingBot_type bot>
struct signalMap{
     //here we put a "typedef" - the choosen type - "TYPE" in the specialisation of this struct
};
//alias to use
template<tradidingBot_type bot>
using SignalMap = signalMap<bot>::TYPE;



/*
* Classe respossable de géré les différents bots de trading en simultané
* son rôle est entre autre de lancer et de maintenir dans différents threads les Events scan de chaque algo
* ici aussi on utilisera du signton
*/
class BotsHandlerInterface{
     public:
     //lancement de la détection d'évéments et donc des différents algorithmes
     static void launch();

     private:
     static BotsHandlerInterface* instance;
     BotsHandlerInterface() = delete;
};

template<TradingBot* ... bots>
class BotsHandler : public BotsHandlerInterface{
     public :
     static void launch();

     private :
     BotsHandler();
};

/*
 * Pour chaque algorithme on aura des métriques bien précises à traker
 * Ainsi nous pourons meusure les performances
 * On définit ici des métriques par défault mais chaque algo peut les redéfinir par un spécialisation
 * capital : solde actuel du compte en dollards
 * total gain : total de tous les gains et pertes engendré par le bot de trading
 * min capital : le plus bas enregister des réserves en dollards du compte
 */
template<tradidingBot_type bot>
class Metrics : std::tuple<double, double, double>{
     using std::tuple<double, double, double>::tuple;
     public:
     void print() const;
     static constexpr double baseCapital = 1000000;
     
     //metrics base accessors
     double getCopieCapital() const{
         return std::get<0>(*this);
     }
     double getCopieGain() const{
         return std::get<1>(*this);
     }
     double getCopieMinCapital() const{
         return std::get<2>(*this);
     }

     //to enter expenses or earnings after eatch successfull trade
     //capital, total gain & min captial automaticly compute
     void cashFlow(const double &);

     protected:
     //metrics reference accessors
     double& getCapital(){
         return std::get<0>(*this);
     }
     double& getGain(){
         return std::get<1>(*this);
     }
     double& getMinCapital(){
         return std::get<2>(*this);
     }
};

/*
 * Enviroment contrôler pour un entraînement efficae des algorithmes
 * Nous utiliserons un chart dont on lira les bougie au fur et à meusure
 * à chaque nouvelle bougie lue, on considèrera un événement newCandle sur l'algorithme
 * pour l'instant on pévoit la possibilité d'entrainer qu'un seul bot à la fois sur un seul jeu de données
 * A FAIRE: prévoir la possibilité d'entrainer sur plusieurs jeux de données en parallèles
 *          Avoir un vrais singleton, ici on peut avoir autant d'instance différente que de "botType" différents
 */
template<tradidingBot_type botType>
class TrainingEnvironment : private Chart, public Metrics<botType>{
     private:
          //in order to insure that we keep a unique TrainingEnvironment at the time
          static inline TrainingEnvironment<botType>* instance;
          //keep track of readed candel
          size_t tracker;
          //the actual instant of the algo trained
          botType* trainedBot;
          //keep in memory the ordders still open
          tabExt<SignalMap<botType>> openOrders;

          //reading a new candel and transmit to algo
          void readCandle();
          //try to pass open order and send events to traiding bot
          //we also upgrade the metrics here
          void passOrders();
          //we place the order given by the signal sending the trading bot
          

     public:
     //like for a chart we create from a dataPath
     TrainingEnvironment(const std::string &, const TimeFrame &, botType &);

     //lanch the training of the trading bot and print us the results
     void launchTraining();

     //usely done at the signal creation
     void placeOrder(const SignalMap<botType> &);
     //remove un order, use to cancel one or if successfull passed or replace for a new one.
     void removeOrder(const SignalMap<botType> &);
     //remove all orders at once
     void removeOrders();

     //accessor
     static TrainingEnvironment* getInstance(){
          UnexpectedEmptyPointer<TRADINGBOTS_EXCEPTION, TrainingEnvironment>::check(std::source_location::current(), instance);
          return instance;
     }
     
     static tabExt<SignalMap<botType>> & getOpenOrders(){
          return getInstance()->openOrders;
     }
     //simple print for debug
     void printOpenOrders();

     friend SignalMap<botType>;
};

#endif
