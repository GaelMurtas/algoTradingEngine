/**********
 ********** SMARTDCA TRADING BOT SPECIFIC CLASS DECRARATIONS
 **********/

/*
 * class of the smart dca trading TradingBot
 * it's a tradingBot class decedent
 */
class smartDCA : public tradingBot{
     //ici on a besoin de transmetre le type d'ordre et la quantité en pourcentage du montant total du compte
     private :
          //moyenne et volatilité en porcentage sur les dernières bougie
          double meanReturn;
          double volReturn;
          //1 if a buy order is alredy pending    TJ NECESSAIRE ?
          //bool pendingBuyOrder;
          //keep track of the actual lot in the dca position
          double actualLot;
          //keep track the mean entree point
          double meanEntree;

          //we want to buy under the last price of a small factor
          //it will multipli the volatility to take it in account (en pourcentage)
          static inline const double discountFactor = 50;
          //gain minimal et maximal actepté pour clore une position (en postion en porcentage)
          static inline const double minGain = 0.5;
          //Inutilisé pour moment on gérera l'adaption dinamique plus tard
          static inline const double maxGain = 10;
          //lot de base utilisé pour chaque achat
          static inline const double baseLot = 1;

     public:
          smartDCA(const std::string&, TimeFrame);
          template<event evn, class ... types> void onEvent(const types & ...);

          //buy and sellPrices calculation based on last candles
          double buyPrice();
          double sellPrice();
};

#include "smartDCA.hh"

