/*********
 ********* FICHIER A COMPILER POUR LE BOT SMARTDCA
 *********/

#include "tradingBots.h"
#include "smartDCA.hpp"
#include "tradingBots.tcc"
#include "smartDCA.tcc"

SmartDCA::SmartDCA(const std::string & path, TimeFrame tf):
     pendingBuyOrder(false), TradingBot(path, tf){
}

double SmartDCA::buyPrice(){
     return last().close * (1-(discountFactor*volReturn/10000));
}

double SmartDCA::sellPrice(){
     return meanEntree * (1-(minGain/100));
}

//exlicit template instiation
template class TrainingEnvironment<SmartDCA>;
