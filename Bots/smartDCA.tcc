/**********
 ********** TEMPLATE IMPLEMANTATION POUR SmartDCA
 **********/ 

using signalSmartDCA = signal<REAL_TRADING, SmartDCA>;
constexpr size_t _length = TradingBot::windowSize;
constexpr auto _mean = mean<Tab<double>, _length>;
constexpr auto _var = var<Tab<double>, _length>;
constexpr auto _sd = sd<Tab<double>, _length>;

template<> void SmartDCA::onEvent<Event::newCandle>(){
         addLastCandle();
         meanReturn = 100 * (_mean(returns())) / mean(_length);
         volReturn = 100 * (_sd(returns())) / mean(_length);
         /*signal buy(BuyPrice());
         signal sell(sellPrice());*/
     }
