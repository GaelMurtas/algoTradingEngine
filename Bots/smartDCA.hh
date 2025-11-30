/**********
 ********** SmartDCA UTILITY LIBRARY
 **********/ 



//specialisation for smart dca signal map
template<>
struct signalMap<smartDCA>{
     typedef uniqueOrder<REAL_TRADING, smartDCA> TYPE;
};

//constantes for writing convenance
constexpr size_t _length = tradingBot::windowSize;
constexpr auto _mean = mean<Tab<double>, _length>;
constexpr auto _var = var<Tab<double>, _length>;
constexpr auto _sd = sd<Tab<double>, _length>;
constexpr auto _getSmartDCAtrainer = []() -> trainingEnvironment<smartDCA> & {return *trainingEnvironment<smartDCA>::getInstance();};

//alias for simplify usage of the correct signal for smartDCA
using SignalSmartDCA = uniqueOrder<REAL_TRADING, smartDCA>;
