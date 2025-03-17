/**********
 ********** TEMPLATE IMPLEMANTATION POUR SmartDCA
 **********/ 

using signalSmartDCA = signal<REAL_TRADING, SmartDCA>;
constexpr size_t _length = TradingBot::windowSize;
constexpr auto _mean = mean<Tab<double>, _length>;
constexpr auto _var = var<Tab<double>, _length>;
constexpr auto _sd = sd<Tab<double>, _length>;
constexpr auto getSmartDCAtrainer = [](){return *TrainingEnvironment<SmartDCA>::getInstance();};

//we need a specific comportement for selling on this algorithme
//sell signal will overide all others sells signal
template<>
void Signal<REAL_TRADING, SmartDCA, bool, double, double>::send(){
     if(std::get<0>(*this) == false){
          tabExt<signalSmartDCA> tmp;
          for(auto sig :  getSmartDCAtrainer().openOrders){
               if(sig.getType()){
                    tmp + sig;
               }
          }
          getSmartDCAtrainer().openOrders = tmp;
     }
     getSmartDCAtrainer().placeOrder(*this);
}

template<> void SmartDCA::onEvent<Event::newCandle>(){
         addLastCandle();
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         /*signal buy(BuyPrice());
         signal sell(sellPrice());*/
     }

template<> void SmartDCA::onEvent<Event::newCandle, Bougie>(const Bougie & cand){
         *this + cand;
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         if(!pendingBuyOrder){
               signalSmartDCA buy(true, baseLot, buyPrice());
               //A FAIRE: pas toujours nécessais on peut accéléré l'algo en sautant ce calcul dans ceratin cas
               signalSmartDCA sell(false, actualLot, sellPrice());
         }
     }

template <>
void SmartDCA::onEvent<Event::successfullBought>(){
}

template <>
void SmartDCA::onEvent<Event::successfullSold>(){
}
