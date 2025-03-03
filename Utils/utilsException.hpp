#ifndef UTILSEXC
#define UTILSEXC

/*
 * GESTION DES EXEPTIONS POUR LA BIBLIOTHÈQUE UTILS.H 
 */

class FileError
{
    private :
        std::string path;

    public :
        FileError(const std::string & s): path(s){}
        
        void accessError(){ 
            std::cerr << "Ouverture du fichier '" << path << "' impossible. \n";
            throw *this;}
        void emptyFile(){
            std::cerr << "Le fichier '" << path << "' est vide. \n";
            throw *this;}
};



#endif
