/*********
 ********* IMPLEMANTATION DES METHODES POUR LA GESTION GENERALISTE DES BOT DE TRADING
 *********/

using namespace std;

template<bool real, tradingAlgorithme bot, class ... args>
Signal<real, bot, args...>::Signal(args ... currentArgs): tuple<args ...>(currentArgs...){
               send();
          }

template<bool real, tradingAlgorithme bot, class ... args>
void Signal<real, bot, args...>::send(){
               TrainingEnvironment<bot>::placeOrder(*this);
          }

template<TradingBot* bot>
void EventScan<bot>::launch(){
     if(!instance) instance = new EventScan();
     instance->scan();
}

template<TradingBot* bot>
EventScan<bot>::~EventScan(){
     delete instance;
}

template<TradingBot* bot>
Event EventScan<bot>::eventMap(const string & str){
     for(char c : str){
          switch(c){
               //cas d'une nouvelle bougie
               case 'N':
                    return Event::newCandle;
               case 'S':
                    return Event::successfullBought;
          }
     }
}

template<TradingBot* bot>
void EventScan<bot>::scan(){
     while(true);
     string contant(readFile(path));
     //A FAIRE : Remplacer ce switch par un compile time for
     switch(eventMap(contant)){
          case Event::newCandle:
               bot->onEvent<Event::newCandle>();
               break;
          case Event::successfullBought:
               bot->onEvent<Event::successfullBought>();
     }
     std::this_thread::sleep_for(std::chrono::seconds(time));
}

template<TradingBot* ... bots>
void BotsHandler<bots ...>::launch(){
     if(instance){
          //gestion d'erreur à faire à la place
          std::cout << "ERROR: Bot Handeler double création." << std::endl;
          exit(-1);
     }
     instance = new BotsHandler<bots ...>();
     auto botList = makeTuple(bots ...);
     vector<jthread> threadVect;
     for(size_t i = 0; i < tuple_size_v<decltype(botList)>; ++i){
          jthread tmp(EventScan<&std::get<i>(botList)>::scan);
          threadVect.push_back(tmp);
     }
}

/*
 * Metrics default comportement
 */

template<tradingAlgorithme botType>
void Metrics<botType>::print(){
     using namespace std;
     cout << "\n- - - - - - - - - - \nObserved Metrics value :\n- - - - - - - - - -\n Total gain: " <<
          get<0>(*this) << endl <<
          " Max drowdawn: " <<
          get<1>(*this) << endl
          << "- - - - - - - - - -\n";
}


/*
 * TrainingEnvironment class implementation
 */

template<tradingAlgorithme botType>
TrainingEnvironment<botType>::TrainingEnvironment(const std::string & path, const TimeFrame & tf, botType & bot):
     Chart::Chart(path, tf), Metrics<botType>::Metrics(), tracker(botType::windowSize), trainedBot(& bot){
          //A FAIRE : Create error if windowSize >= Chart size
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::launchTraining(){
     for(;tracker < size(); ++tracker){
          readCandle();
          passOrders();
     }
     Metrics<botType>::print();
}

