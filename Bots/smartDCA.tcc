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
     std::cout << "new Candle detected:\n";
     cand.affiche();
         *this + cand;
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         std::cout << "Stats : mean - "  << meanReturn << " vol - " << volReturn << std::endl;
         if(!pendingBuyOrder){
               signalSmartDCA buy(true, baseLot, buyPrice());
               std::cout <<  "buy order : lot - " << baseLot << " price - " << buyPrice() << std::endl;
               pendingBuyOrder == true;
               //A FAIRE: pas toujours nécessais on peut accéléré l'algo en sautant ce calcul dans ceratin cas
               if(actualLot >= baseLot){
                    signalSmartDCA sell(false, actualLot, sellPrice());
                    std::cout <<  "Sell order : lot - " << actualLot << " price - " << sellPrice() << std::endl;
               }
               getSmartDCAtrainer().printOpenOrders();
          }
     }

template <>
void SmartDCA::onEvent<Event::successfullBought>(){
     std::cout << "Achat effectuer sur la bougie : \n";
     last().affiche();
     pendingBuyOrder = false;
}

template <>
void SmartDCA::onEvent<Event::successfullSold>(){
     std::cout << "vente effectuer sur la bougie : \n";
     last().affiche();
}
