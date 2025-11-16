/*********
 ********* IMPLEMANTATION DES METHODES POUR LA GESTION GENERALISTE DES BOT DE TRADING
 *********/

using namespace std;

/*
 * Signal base class implementation
 */

template<bool real, tradidingBot_type bot, class ... args>
Signal<real, bot, args...>::Signal(): tuple<args ...>(){
          FUNCTION_FOLLOW()
          }

template<bool real, tradidingBot_type bot, class ... args>
Signal<real, bot, args...>::Signal(args ... currentArgs): tuple<args ...>(currentArgs...){
          FUNCTION_FOLLOW()
               SignalMap<bot>::send(*this);
          }

template<bool real, tradidingBot_type bot, class ... args>
void Signal<real, bot, args...>::send(Signal<real, bot, args ...> order){
     FUNCTION_FOLLOW()
     TrainingEnvironment<bot>::getInstance()->placeOrder(order);
}

/*
 * Signal child classes implementation
 */

template<bool real, tradidingBot_type bot>
LimitOrder<real, bot>::LimitOrder(const Signal<real, bot, orderType, double, double> & sig){
          FUNCTION_FOLLOW()
     std::get<0>(*this) = std::get<0>(sig);
     std::get<1>(*this) = std::get<1>(sig);
     std::get<2>(*this) = std::get<2>(sig);
}

template<bool real, tradidingBot_type bot>
void LimitOrder<real, bot>::print() const{
     using namespace std;
     cout << "Signal Contant : type - " << [&](){if(getType()==orderType::buy) return "Buy"; return "Sell";}()
          << ", lot - " << getLot() << ", price - " << getPrice() << endl;
}

//send spesific comportement for the Unique Order
template<bool real, tradidingBot_type bot>
void UniqueOrder<real, bot>::send(UniqueOrder<real, bot> order){
     FUNCTION_FOLLOW()
     constexpr auto OpenOrders = TrainingEnvironment<bot>::getOpenOrders;
	if(order.getType() == orderType::buy){
          for(auto sig : OpenOrders()){
               //we ensure to keep the sell order(suppose to be unique)
               if(sig.getType() == orderType::sell){
                    OpenOrders()[0] = sig;
                    break;
               }
          }
     }
     else{
          for(auto sig :  OpenOrders()){
               //we ensure to keep the sell order(suppose to be unique)
               if(sig.getType() == orderType::buy){
                    OpenOrders()[0] = sig;
                    break;
               }
          }    
     }
     //we have two order max, one buy and one sell
     if (OpenOrders().size() <= 1){
          OpenOrders().pushBack(order);
     }
     else OpenOrders()[1] = order;
}

/*
 * EnevtScan
 */

template<TradingBot* bot>
void EventScan<bot>::launch(){
          FUNCTION_FOLLOW()
     if(!instance) instance = new EventScan();
     instance->scan();
}

template<TradingBot* bot>
EventScan<bot>::~EventScan(){
          FUNCTION_FOLLOW()
     delete instance;
}

template<TradingBot* bot>
Event EventScan<bot>::eventMap(const string & str){
     for(char c : str){
          FUNCTION_FOLLOW()
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
          FUNCTION_FOLLOW()
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
          FUNCTION_FOLLOW()
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

template<tradidingBot_type botType>
void Metrics<botType>::print() const{
          FUNCTION_FOLLOW()
     using namespace std;
     cout << "\n- - - - - - - - - - \nObserved Metrics value :\n- - - - - - - - - -\n Total gain: " <<
          getCopieGain()/baseCapital*100 << "% " << endl <<
          " Minimal capital value: " <<
          getCopieMinCapital()/baseCapital*100 << "% " << endl
          << "- - - - - - - - - -\n";
}
template<tradidingBot_type botType>
void Metrics<botType>::cashFlow(const double & amount){
     getCapital() += amount;
     getGain() += std::max(0.0, amount);
     getMinCapital() = std::min(getMinCapital(), getCapital()+amount);
}

/*
 * TrainingEnvironment class implementation
 */

template<tradidingBot_type botType>
TrainingEnvironment<botType>::TrainingEnvironment(const std::string & path, const TimeFrame & tf, botType & bot):
     Chart::Chart(path, tf), Metrics<botType>::Metrics(Metrics<botType>::baseCapital, 0, Metrics<botType>::baseCapital), tracker(botType::windowSize), trainedBot(& bot){
          FUNCTION_FOLLOW()
          //A FAIRE : Create error if windowSize >= Chart size or if an instance is already instantiate
          instance =  this;
          //trainedBot initialisation whith minimal amount of candle.
          for(size_t i = 0; i < tracker; i++){
               *trainedBot + getConst(i);
          }
}

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::launchTraining(){
          FUNCTION_FOLLOW()
     for(;tracker < size(); ++tracker){
          std::cout<< "\nTracker : "  << tracker 
               << " - - - Capital : " << this->getCopieCapital() <<std::endl;
          readCandle();
          passOrders();
     }
     Metrics<botType>::print();
}

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::readCandle(){
          FUNCTION_FOLLOW()
     trainedBot->template onEvent<Event::newCandle, Bougie>(get(tracker));
}

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::passOrders(){
          FUNCTION_FOLLOW()
     if(openOrders.size()>0){//A FAIRE: Arranger l'itérateur pour ne plus avoir besoin de cette ligne
     tabExt<SignalMap<botType>> toRemoveOrder;//remove order in the loop cause seg falt
     for(auto order : this->openOrders){
          if((std::get<0>(order) == orderType::buy) && (std::get<2>(order) >= getConst(tracker).low)){//buy case
          trainedBot->template onEvent<Event::successfullBought>();
          toRemoveOrder.pushBack(order);
          }
          else if((order.getType() == orderType::sell) && (order.getPrice() <= getConst(tracker).high)){//sell case
          trainedBot->template onEvent<Event::successfullSold>();
          toRemoveOrder.pushBack(order);
          }
     }
     if(toRemoveOrder.size()){
          for(auto order : toRemoveOrder){
               removeOrder(order);
          }
     }
}
}

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::placeOrder(const SignalMap<botType> & sig){
          FUNCTION_FOLLOW()
     openOrders.pushBack(sig);
}

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::removeOrder(const SignalMap<botType> & order){
     FUNCTION_FOLLOW()
     for(uint i = 0; i < openOrders.size(); ++i){
          if(openOrders[i] == order){
               openOrders.remove(i);
               return;
          }
     }
}

//A FAIRE: Écrire une méthode CLEAR pour la class TAB qui efface les données et l'utiliser pour ré écrire cette fonction
/*template<tradidingBot_type botType>
void TrainingEnvironment<botType>::removeOrders(){
     FUNCTION_FOLLOW()
     tabExt<signal<0, botType>> tmp;
     openOrders = tmp;
}*/

template<tradidingBot_type botType>
void TrainingEnvironment<botType>::printOpenOrders(){
     std::cout << "OpenOrders :\n";
     if(openOrders.size()>0){//A FAIRE: arranger l'itérateur pour éviter cette ligne
     for(auto sig :  openOrders){
          sig.print();
     }}
}
