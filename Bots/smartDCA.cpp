/*********
 ********* FICHIER A COMPILER POUR LE BOT SMARTDCA
 *********/

#include "tradingBots.hpp"
#include "smartDCA.hpp"
#include "tradingBots.tcc"
#include "smartDCA.ipp"

smartDCA::smartDCA(const std::string & path, TimeFrame tf):
     tradingBot(path, tf){
          FUNCTION_FOLLOW()
}

double smartDCA::buyPrice(){
          FUNCTION_FOLLOW()
     return last().close * (1-(discountFactor*volReturn/10000));
}

double smartDCA::sellPrice(){
          FUNCTION_FOLLOW()
     return meanEntree * (1+(minGain/100));
}

//exlicit template instiation
template class trainingEnvironment<smartDCA>;
