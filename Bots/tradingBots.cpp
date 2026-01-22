/**********
 ********** Implementation for tradingBots related general comportement related template free methods
 ********** Main file to compile for the tradingBots library
 **********/
#include "tradingBots.hpp"
#include "tradingBots.tcc"

/*
 * class tradingState
 */

tradingState::tradingState(const std::string & path): stateFileContent(path){
     //A Faire : gestion d'exception, vérification de la mise en forme du stateafile
     //une gestion dans un else après le if et une autre si les strings voulu ne se transforme pas en size_t
     //we first search the position of the key words
     //search give the one dimentional position
     std::pair<bool, size_t> initCapLocation = stateFileContent.search("initial_capital");
     std::pair<bool, size_t> currentCapLocation = stateFileContent.search("current_capital");
     std::pair<bool, size_t> posLotLocation = stateFileContent.search("current_position_lot");
     std::pair<bool, size_t> posEntreeLocation = stateFileContent.search("current_position_entree");
     if(initCapLocation.first && currentCapLocation.first && posLotLocation.first&& posEntreeLocation.first){
          try{
               initialCapital = std::stod(stateFileContent[initCapLocation.second+1]);
               currentCapital = std::stod(stateFileContent[currentCapLocation.second+1]);
               currentPositionSize = std::stod(stateFileContent[posLotLocation.second+1]);
               currentPositionEntree = std::stod(stateFileContent[posEntreeLocation.second+1]);
          }
          catch(...){
               //À faire : meilleure gestion d'exception
               //on aimerais savoir quelle exception exactement est déclachédans le try
               //et aussi quels attributs exactement sont concernés
               conversionErrorDetected(std::source_location::current());
          }
     }
     else{
          //we execute this branch only if a keyword is missing in the stateFile
          fileFormatErrorDetected(std::source_location::current(), stateFilePath);
     }
}
