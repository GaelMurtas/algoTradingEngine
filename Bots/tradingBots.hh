/**********
 ********** TRADING BOTS COMPILETIME AND UTILITIES LIBRARY
 **********/

#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>
#include <concepts>
#include <type_traits>
#include "../Trading/trading.h"
#include "../Math/math.h"
#include "../meta/meta.hh"

 //variable servant à activer le tarding réel ou à a compiler la versio d'entrainement
 #define REAL_TRADING 0

//activation des exceptions spécifique à cette bibliothèque
#define TRADINGBOTS_EXCEPTION 1

//Option permettant de suivre l'ordre d'appel des fonctions dans le programme
//utilisé pour débugage
 #define TRADINGBOTS_DEBUG 0
 #if TRADINGBOTS_DEBUG
 #define FUNCTION_FOLLOW() functionFollow(std::source_location::current());
 #else
 #define FUNCTION_FOLLOW() emptyFunction();
 #endif

class tradingBot;

//le concept de tradidingBot_type représente les différent types de trading bot
template <class T>
concept tradidingBot_type = std::is_base_of<tradingBot, T>::value;

//Les différents événements qui nécessite une réaction de la part d'un bot de trading
enum class event{
     newCandle, successfullBought, successfullSold
};


template<bool real, tradidingBot_type bot, class ... args>
class signal;

//empty signal interface use for concepts and gerericity proposes
class iSignal{
};

//to later use any trading signal as template parameter
template <class T>
concept signal_type = std::is_base_of<iSignal, T>::value;

//To differenciate the buy and the sell signal
enum class orderType{
     buy, sell
};

//alias to map the corret signal type to eatch trading bot type
//default comportent useless, made to use only the spesialisation
//spesialisation define in eatch trading bots own library
template<tradidingBot_type bot>
struct signalMap{
     //here we put a "typedef" - the choosen type - "TYPE" in the specialisation of this struct
     //here we put a non sens default option to obligate the specialisation
     typedef std::false_type TYPE;
};
//alias to use
template<tradidingBot_type bot>
using SignalMap = signalMap<bot>::TYPE;

/*
 * botHandler meta programming specific stuff
 * we need to transform a collection of bots in a colletion of communicatinWay
 * one for eatch bot
 */ 
/*
//general case for recurrence
//index vraiment urile ?
template<int index, tradingBot* bot1, tradingBot* ... bots, botCommunicationWay ... ways>
struct botCommunicationWays_impl{
     typedef botCommunicationWays_impl<index-1, botCommunicationWay ... ways, botCommunicationWay<bot1>, bots ...>::TYPE TYPE;
}

//for the first call evry computation still to do
//spé vraiment utile ?
template<int index, tradingBot* bot1, tradingBot* ... bots>
struct botCommunicationWays_impl<bot1, bots ... , index>{
     typedef botCommunicationWays_impl<index-1, botCommunicationWay<bot1>, bots ...>::TYPE TYPE;
}

//base case all communicatinWay are computed
template<botCommunicationWay ... ways>
struct botCommunicationWays_impl<ways ..., 0>{
     typedef std::tuple<ways ...> TYPE;
}
*/


/*
 * alias used for inheritance in botHandler
 */

//the iterface for bot botHandler it exist to simplify notations and simtaxes
class iBotHandler{
};

template<tradingBot* ... bots>
using BotsHandlers = std::array<iBotHandler, nbTemplateArgs<bots ...>>;




//Others utilities
//mettre cette fonction dans bibliothèque utils
inline void functionFollow(const auto location){
	std::cout << "function `" << location.function_name()
              << "` form file `" << location.file_name() << " (line - "
              << location.line() << ")`"
              << " called\n";
}

