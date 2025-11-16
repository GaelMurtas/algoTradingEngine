/**********
 ********** TEMPLATE IMPLEMANTATION POUR SmartDCA
 **********/ 

using SignalSmartDCA = UniqueOrder<REAL_TRADING, SmartDCA>;
constexpr size_t _length = TradingBot::windowSize;
constexpr auto _mean = mean<Tab<double>, _length>;
constexpr auto _var = var<Tab<double>, _length>;
constexpr auto _sd = sd<Tab<double>, _length>;
constexpr auto _getSmartDCAtrainer = []() -> TrainingEnvironment<SmartDCA> & {return *TrainingEnvironment<SmartDCA>::getInstance();};

/*template<> void SmartDCA::onEvent<Event::newCandle>(){
     FUNCTION_FOLLOW()
         addLastCandle();
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         //signal buy(buyPrice());
         //signal sell(sellPrice());
     }*/

template<>
void SmartDCA::onEvent<Event::newCandle, Bougie>(const Bougie & cand){
     FUNCTION_FOLLOW()
     std::cout << "new Candle detected:\n";
     cand.affiche();
     pushBack(cand);
     meanReturn = 100 * (_mean(returns())) / mean(_length);
     volReturn = 100 * (_sd(returns())) / mean(_length);
     std::cout << "Stats : mean - "  << meanReturn << " vol - " << volReturn << std::endl;
     std::cout << "Claculated buy & sell signals : \n   " << buyPrice() << "x" << baseLot
          << "\n   " << sellPrice( )<< "x" << actualLot << std::endl;
     if (actualLot>= baseLot) SignalSmartDCA sell(orderType::sell, actualLot, sellPrice());
     SignalSmartDCA buy(orderType::buy, baseLot, buyPrice());
     }

template <>
void SmartDCA::onEvent<Event::successfullBought>(){
     FUNCTION_FOLLOW()
     actualLot += baseLot;
     double price = buyPrice();
     //meanEntree calcul
     if(actualLot == baseLot){
          meanEntree = price;}
     else{
          double nbBuy = actualLot/baseLot;
          meanEntree = (meanEntree*(nbBuy-1) + price)/nbBuy;}
     //metrics update
     _getSmartDCAtrainer().cashFlow(-baseLot*price);
     std::cout << "Achat effectuer sur la bougie : \n";
     last().affiche();
     //pendingBuyOrder = false;
}

template <>
void SmartDCA::onEvent<Event::successfullSold>(){
     FUNCTION_FOLLOW()
     _getSmartDCAtrainer().cashFlow(actualLot*sellPrice());
     actualLot = 0;
     std::cout << "vente effectuer sur la bougie : \n";
     last().affiche();
}
