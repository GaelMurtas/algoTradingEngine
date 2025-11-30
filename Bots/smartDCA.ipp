/**********
 ********** TEMPLATE SPECIALIZATION FOR SmartDCA
 **********/ 

/*template<> void SmartDCA::onEvent<Event::newCandle>(){
     FUNCTION_FOLLOW()
         addLastCandle();
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         //signal buy(buyPrice());
         //signal sell(sellPrice());
     }*/

template<>
void smartDCA::onEvent<event::newCandle, Bougie>(const Bougie & cand){
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
void smartDCA::onEvent<event::successfullBought>(){
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
void smartDCA::onEvent<event::successfullSold>(){
     FUNCTION_FOLLOW()
     _getSmartDCAtrainer().cashFlow(actualLot*sellPrice());
     actualLot = 0;
     std::cout << "vente effectuer sur la bougie : \n";
     last().affiche();
}
