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

//Les différents événements qui nécessite une réaction de la part d'un bot de trading
enum class Event{
     newCandle, successfullBought
};

/*
 * classe mère pour les trading bots
 * Chaque bot aura sa déclaration dans un fichier spécialisé
 */
class TradingBot: protected Chart{
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
     template<Event event> void onEvent(){
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

//le concept de tradingAlgorithme représente les différent types de trading bot
template <class T>
concept tradingAlgorithme = std::is_base_of<TradingBot, T>::value;

/* 
 * Object représentant le trade final que l'on souhaite passer à un instant t.
 * Le premiers argument du template représente si le signal doit être exécuter en réel
 * ou bien si il est exécuter sur les données d'entrainement.
 * Les autres arguments sont pour le contenu du signal qui diffère en fonction des algos
 */
template<bool real, tradingAlgorithme bot, class ... args>
class Signal : public std::tuple<args ...>{
     public :
          Signal(args ...);
     private :
          //fonction d'envoi du signal appeller automatique à la construction
          void send();
};

//contenu du signal par défault il sera spécialiser si besoin dans les boliothèque de spécialisation de chaque bot
//long ou short, quantity, price
template<bool real, tradingAlgorithme bot>
using signal = Signal<real, bot, bool, double, double>;

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
 * On définit ici des métriques par défault mais chaque algo pour les redéfinir par un spécialisation
 * total gain et max drowdown
 */
template<tradingAlgorithme bot>
class Metrics : std::tuple<double, double>{
     using std::tuple<double, double>::tuple;
     public:
     void print();
};

/*
 * Enviroment contrôler pour un entraînement efficae des algorithmes
 * Nous utiliserons un chart dont on lira les bougie au fur et à meusure
 * à chaque nouvelle bougie lue, on considèrera un événement newCandle sur l'algorithme
 */
template<tradingAlgorithme botType>
class TrainingEnvironment : private Chart, Metrics<botType>{
     private:
          //keep track of readed candel
          size_t tracker;
          //the actual instant of the algo trained
          botType* trainedBot;
          //keep in memory the ordders still open
          tabExt<signal<0, botType>> openOrders;

          //reading a new candel and transmit to algo
          void readCandle();
          //try to pass open order and send events to traiding bot
          //we also upgrade the metrics here
          void passOrders();
          //we place the order given by the signal sending the trading bot
          void placeOrder(signal<0, botType>);

     public:
     //like for a chart we create from a dataPath
     TrainingEnvironment(const std::string &, const TimeFrame &, botType &);

     //lanch the training of the trading bot and print us the results
     void launchTraining();

     friend class Signal<0, botType>;
};

#endif
