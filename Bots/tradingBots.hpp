/**********
 ********** CLASS DECLATION FOR TRADING BOTS
 **********/


#ifndef TRADINGBOTS
#define TRADINGBOTS
#include "tradingBots.hh"

/*
 * Base class for all trading bots.
 * Each bot has its own declaration in a dedicated file.
 */
class tradingBot: public Chart{
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
     tradingBot(const std::string & path, TimeFrame tf): dataPath(path), Chart(path, tf){
          //erreur à renvoyer le nombre de bougie est inférieu à windowSize.
     }

     //ce que l'algo doit faire suite à la détection d'un événement
     //Dans cette classe qui n'as pas vocation à être utiliser directement on déclare une erreur si cette fonctions est appeller
     //En effet ce sont dans les classes filles que l'on va géré les évément via des spécialisations de cette méthode
     //Si cette fonction est appeller c'est donc qu'un évément non prévu tente d'être géré par un bot
     template<event evn, class ... types> void onEvent(const types & ...){
          std::cout << "Evenement inconu détecté !\n";
          exit(-1);
     }
};

/* 
 * Object representing the final trade to be executed at a given time t.
 * The first template argument indicates whether the signal must be executed
 * in real trading or on training/backtest data.
 * The other template arguments represent the content of the signal, which varies
 * depending on the algorithm.
 * Class made to be a base class—see derived classes in the .hpp files.
 */
template<bool real, tradidingBot_type bot, class ... args>
class signal : public std::tuple<args ...>, iSignal{
     public :
          signal();
          signal(args ...);
     private :
          //fonction d'envoi du signal appeller automatique à la construction
          //static to be able to use in constructor
          static void send(signal<real, bot, args ...>);
};

//basic limit order, could be used by many trading bots
//long ou short, quantity, price
template<bool real, tradidingBot_type bot>
class limitOrder : public signal<real, bot, orderType, double, double>{
     using signal<real, bot, orderType, double, double>::signal;
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
     limitOrder(const signal<real, bot, orderType, double, double> &);
};

//Limit order but we ensure to be unique
//it can be only one order of the same type in the eatch trading bot in a given market
template<bool real, tradidingBot_type bot>
class uniqueOrder : public limitOrder<real, bot>{
     using limitOrder<real, bot>::limitOrder;
     public:
          //we redifind the send to automaticly remove older order when a new one is sending.
          static void send(uniqueOrder<real, bot>);
};



template<tradingBot* bot>
class eventScan{
     public:
     //on lance la détection d'events
     static void launch();

     private:
     //fichier a scaner en continue, on regarde le fichier jusqu'à détecter le code d'un event
     static const std::string path;
     //ici on veut un signlon car on ne souhaite pas que plusieurs gestionaire d'events puisse exister sur le mm algo
     static eventScan* instance;
     //condient de temps attende avant de revérifier le fichier en seconde
     inline static const double time = 10;

     eventScan();
     ~eventScan();

     //renvoie l'évent correspondant au code
     static event eventMap(const std::string &);
     //lance la boucle infini de scan de fichier
     void scan();
};



/*
 * Class responsible to the real trading algorithms worklow
 * for algo we allow communication with the correct file to transmit order and react to event.
 * it's able to managing multiple trading bots simultaneously.
 * Its role includes launching and maintaining each algorithm’s event scans and execution in separate threads.
 */

//The singleton pattern is used here at interface level
//it's because we don't want multiple botsHandler even for different work 
//we prefer manage all bots in a unique instance
//
//TEMPORAIRE : on ne sait pas encore la structure du disaign de cette partie
class iBotsHandler{
     public:
     //lancement de la détection d'évéments et donc des différents algorithmes
     static void launch();

     private:
     static iBotsHandler* instance;
     iBotsHandler() = delete;
};

//BotsHandeler main class
//heritate from 'BotCommunicationWays' witch is an alias for an array the 'botCommunicationWay' of eatch bot.
template<tradingBot* ... bots>
class botsHandler : public iBotsHandler, public BotCommunicationWays<bots ...>{
     public :
     static void launch();

     private :
     botsHandler();
};

//class to ensure comunication bettewn itch bot and correct file
//use only for real trading
//Peut-être a mettre en template paramétré par 'botType' si certaines spésembles nécessaire
class botCommunicationWay{
     public:
          const tradingBot * adrBot;
          const std::string dataPath, eventPath, orderPath;

     //open and verify access to file in constructor
     botCommunicationWay();

     //close all files
     ~botCommunicationWay();
};


/*
 * Each algorithm tracks a specific set of performance metrics.
 * These metrics allow us to measure and evaluate performance.
 * Default metrics are defined here, but each algorithm may override them
 * through specialization.
 *
 * capital      : current account balance in dollars
 * total gain   : cumulative gains and losses generated by the trading bot
 * min capital  : lowest recorded dollar balance of the account
 */
template<tradidingBot_type bot>
class metrics : std::tuple<double, double, double>{
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
 * Controlled environment for efficient algorithm training.
 * A chart is used as data source, and candles are read one by one.
 * For each new candle, a `newCandle` event is triggered on the algorithm.
 * For now, only one bot can be trained at a time on a single dataset.
 * A FAIRE: prévoir la possibilité d'entrainer sur plusieurs jeux de données en parallèles
 *          Avoir un vrais singleton, ici on peut avoir autant d'instance différente que de "botType" différents
 */
template<tradidingBot_type botType>
class trainingEnvironment : private Chart, public metrics<botType>{
     private:
          //in order to insure that we keep a unique TrainingEnvironment at the time
          static inline trainingEnvironment<botType>* instance;
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
     trainingEnvironment(const std::string &, const TimeFrame &, botType &);

     //lanch the training of the trading bot and print us the results
     void launchTraining();

     //usely done at the signal creation
     void placeOrder(const SignalMap<botType> &);
     //remove un order, use to cancel one or if successfull passed or replace for a new one.
     void removeOrder(const SignalMap<botType> &);

     //accessor
     static trainingEnvironment* getInstance(){
          UnexpectedEmptyPointer<TRADINGBOTS_EXCEPTION, trainingEnvironment>::check(std::source_location::current(), instance);
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
