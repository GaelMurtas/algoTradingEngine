/**********
 ********** Implementation for tradingBots related general comportement related template free methods
 ********** Main file to compile for the tradingBots library
 **********/
#include "tradingBots.hpp"
#include "tradingBots.tcc"

using namespace std;

/*
 * class tradingState
 */

tradingState::tradingState(const std::string & path): stateFileContent(path), stateFilePath(path){
     //A Faire : gestion d'exception, vérification de la mise en forme du stateafile
     //une gestion dans un else après le if et une autre si les strings voulu ne se transforme pas en size_t
     readState();
}

tradingState::~tradingState(){
     writeState();
}

void tradingState::refreshTable(){
     TabExt2<string> newTableCotant(4,2);
     newTableCotant.get(1,1) = "initial_capital";
     newTableCotant.get(2,1) = "current_capital";
     newTableCotant.get(3,1) = "current_position_lot";
     newTableCotant.get(4,1) = "current_position_entree";
     try{
          newTableCotant.get(1,2) = to_string(initialCapital);
          newTableCotant.get(2,2) = to_string(currentCapital);
          newTableCotant.get(3,2) = to_string(currentPositionSize);
          newTableCotant.get(4,2) = to_string(currentPositionEntree);
     }
     catch(...){
          conversionErrorDetected(std::source_location::current());
     }
     //delete[] stateFileContent;
     stateFileContent = Table(newTableCotant);
}

void tradingState::readState(){
     //actualReading(initialCapital, currentCapital, currentPositionSize, currentPositionEntree, path, stateFileContent);
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

void tradingState::writeState(){
     refreshTable();
     stateFileContent.CSV(stateFilePath);

}
