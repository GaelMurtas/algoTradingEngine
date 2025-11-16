/*********
 ********* FICHIER A COMPILER POUR LE BOT SMARTDCA
 *********/

#include "tradingBots.h"
#include "smartDCA.hpp"
#include "tradingBots.tcc"
#include "smartDCA.tcc"

SmartDCA::SmartDCA(const std::string & path, TimeFrame tf):
     TradingBot(path, tf){
          FUNCTION_FOLLOW()
}

double SmartDCA::buyPrice(){
          FUNCTION_FOLLOW()
     return last().close * (1-(discountFactor*volReturn/10000));
}

double SmartDCA::sellPrice(){
          FUNCTION_FOLLOW()
     return meanEntree * (1+(minGain/100));
}

//exlicit template instiation
template class TrainingEnvironment<SmartDCA>;
