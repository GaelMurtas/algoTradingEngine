/*********
 ********* IMPLEMANTATION DES METHODES POUR LA GESTION GENERALISTE DES BOT DE TRADING
 *********/

using namespace std;

/*
 * Signal default implementation
 */
template<bool real, tradingAlgorithme bot, class ... args>
Signal<real, bot, args...>::Signal(): tuple<args ...>(){
          }

template<bool real, tradingAlgorithme bot, class ... args>
Signal<real, bot, args...>::Signal(args ... currentArgs): tuple<args ...>(currentArgs...){
               send();
          }

template<bool real, tradingAlgorithme bot, class ... args>
void Signal<real, bot, args...>::send(){
               TrainingEnvironment<bot>::getInstance()->placeOrder(*dynamic_cast<Signal<real, bot, args ...>>(this));
          }

template<bool real, tradingAlgorithme bot>
DefaultSignal<real, bot>::DefaultSignal(const Signal<real, bot, bool, double, double> & sig){
     std::get<0>(*this) = std::get<0>(sig);
     std::get<1>(*this) = std::get<1>(sig);
     std::get<2>(*this) = std::get<2>(sig);
}

/*
 * EnevtScan
 */

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

/*
 * BotsHandler
 */

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
     Chart::Chart(path, tf), Metrics<botType>::Metrics(Metrics<botType>::baseCapital, -1, -1), tracker(botType::windowSize), trainedBot(& bot){
          //A FAIRE : Create error if windowSize >= Chart size or if an instance is already instantiate
          instance =  this;
          //trainedBot initialisation whith minimal amount of candle.
          for(size_t i = 0; i < tracker; i++){
               *trainedBot + getConst(i);
          }
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::launchTraining(){
     for(;tracker < size(); ++tracker){
          readCandle();
          passOrders();
     }
     Metrics<botType>::print();
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::readCandle(){
     trainedBot->template onEvent<Event::newCandle, Bougie>(get(tracker));
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::passOrders(){
     for(auto order : this->openOrders){
          if(std::get<0>(order) && (std::get<2>(order) >= getConst(tracker).low)){//buy case
          trainedBot->template onEvent<Event::successfullBought>();
          this->getCapital() =  this->getCapital() - order.getLot()* order.getPrice();
          removeOrder(order);
          }
          else if(!order.getType() && (order.getPrice() <= getConst(tracker).high)){//sell case
          trainedBot->template onEvent<Event::successfullSold>();
          this->getCapital() = this->getCapital() + order.getLot()* order.getPrice();
          removeOrder(order);
          }
     }
     this->getGain() = std::max(this->getGain(), this->getCapital() - this->baseCapital);
     //A FAIRE: CALCUL FAUX METRE LA BONNE FORMULE
     this->getDrawdown() = std::min(this->getDrawdown(),  this->getGain());
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::placeOrder(const signal<0, botType> & sig){
     openOrders + sig;
}

template<tradingAlgorithme botType>
void TrainingEnvironment<botType>::removeOrder(const signal<0, botType> & order){
     tabExt<signal<0, botType>> tmp;
     for(auto sig :  openOrders){
          if(sig != order){
               tmp + sig;
          }
     }
     openOrders = tmp;
}
