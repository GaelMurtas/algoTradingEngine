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



/*
 * Classes responsible to the real trading algorithms workflow
 * for algo we allow communication with the correct file to transmit order and react to event.
 * it's able to managing multiple trading bots simultaneously.
 * Its role includes launching and maintaining each algorithm’s event scans and execution in separate threads.
 */

//class to save the current state of trading activiti
//for real trading bots need to have acces to trading history
//for instance the adverage adverage price of buying, current positions in the market ...
//template<tradidingBot_type botType> voir plus tard si necessaire
class tradingState{
     private:
     //trading state data
     double initialCapital;
     double currentCapital;
     //positions encode as entree price and entree size (negative if short)
     //A FAIRE: faire une classe position générale qui permet d'avoir plus infos
     //nombres d'entrées, valeurs actuelles de la position, date d'entrées ...
     double currentPositionEntree, currentPositionSize;
     //no need to save the state of current open order cause it's save in a specific order file

     //trading state file related infos
     const std::string stateFilePath;
     Table stateFileContent;

     protected:
          //method to use by botCommunicationWay
          void readState();
          void writeState();

     public :
     tradingState(const std::string & path);
     ~tradingState();
     //accessors
     const double & getItitCap() const{
          return initialCapital;
     }
     double & getCap(){
          return currentCapital;
     }
     double & getPosEntree(){
          return currentPositionEntree;
     }
     double & getPosSize(){
          return currentPositionSize;
     }

          void writeState(const tradingState &);
          tradingState getState();
};


//class to ensure comunication bettewn itch bot and corrects files
//heritate of trading state for writing convenance
//use only for real trading
template<tradidingBot_type botType>
class botCommunicationWay: public tradingState{
     public:
          //the trading bot witch comunicatitate via this instance
          const tradingBot * adrBot;
          //the path of communication files
          const std::string dataPath, eventPath, orderPath;

          //open and verify access to file in constructor
          //also charge existing file in memory
          botCommunicationWay(const tradingBot &, const std::string &, const std::string &, const std::string &, const std::string &);

          //close all files
          ~botCommunicationWay();

          //actual function use by over class to allow comunication between the bots and the files
          void addOrder(const SignalMap<botType> &);
          void addEvent(const event &);
          event getLastEvent();
          Bougie getLastCandel();

     private:
          //contant of comunication file loaded in memory
          Table dataTable, eventTable, orderTable;
          //write and read corespond table in files
          void readOrders();
          void readEvents();
          void writeOreders();
          void writeEvents();
};

//we weed to wtach out the change in key files for bots to react
//cette classe peut être utile a un niveau plus générale
//il faudra peut-être la déplacer dans utils
class fileWatcher{
     public:
     //we build the class form the collection of path to the files to watch
     fileWatcher(const Tab<std::string>);
     //pause the thread untill a change in a file is detected
     static void launch();

     private:
     //files to contuinusly fileWatche
     static const tab<std::string> path;
     //waiting time in second
     inline static const double time = 10;

     //lanch infinit loop for a particular file
     void scan(const std::string & path);
};

//class with actual botHandler implementation
//it's responsible to lanch trading, transmit and load information to the trading algorithm
//this disaign permit specialisation of the botHandler for certain type of bots if needed
//iBotHandler is the interface to use to simplify manipulatation of bot for real trading
template<tradidingBot_type botType>
class botHandler : public iBotHandler, protected botCommunicationWay<botType>{
     public :
          static void launchRealTrading(const botType &);
          static void stopRealTrading(const botType &);

     private :
          //load the tradingState to the trading bot 
          void loadState();
          //launch scan of file to detect events
          void launchEventDetection();
          //transmit the event to tradingBot
          void processEvent(event);
          //we writedown the orders receve form the bots
          void placeOrder(const SignalMap<botType> & order);
          //for save the actual state at stopping the real trading
          void saveState();

};

//class to manipulate all bots at once
//The singleton pattern is used here because we don't want multiple botsHandler even for different work 
//we prefer manage all bots in a unique instance
//heritate from 'BotsHandlers' witch is an alias for an array the 'botHandler' of eatch bot.
//
//A Faire : Implémenter cette classe dès que la gestion efficce de multiples algorithme en simultanée sera devenu une necessitée
/*template<tradingBot* ... bots>
class botsHandler : public BotsHandlers<bots ...>{
     public :
     static void launch();

     private :
     //A Faire : stocker l'instance dans une variable globale
     //ainsi le sigleton restera efficace malgrès l'utilisation de template
     //on veux être certain de ne pas avoir plusieurs instances même pour des templates differents
     static iBotsHandler* instance;
     botsHandler();
};*/


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
