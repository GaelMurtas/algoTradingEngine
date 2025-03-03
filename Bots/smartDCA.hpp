/**********
 ********** BIBLIOTHEQUE DE SPECIALISATION POUR LE BOT DE TRADING SMARTDCA
 **********/

class SmartDCA : public TradingBot{
     //ici on a besoin de transmetre le type d'ordre et la quantité en pourcentage du montant total du compte
     using TradingBot::TradingBot;
     private :
          //moyenne et volatilité en porcentage sur les dernières bougie
          double meanReturn;
          double volReturn;
          //gain minimal et maximal actepté pour clore une position (en postion en porcentage)
          double nimGain;
          double maxGain;

     template<Event event> void onEvent();
};

