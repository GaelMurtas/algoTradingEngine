using namespace std;

//Intialisaétion des membres statiques
bool Exception::BYPASS = false;
bool Exception::THROW = false;
//string path = "../LOG";

Exception::Exception(const void* ptr, const std::source_location & lc, const ExceptionType & t): obj(ptr), location(lc), type(t) {
}

void Exception::affiche(){
    cout << "\n- - - - - - - - - -" << endl;
    string s = " WARNING ";
    switch (type){
        case (ExceptionType::error) :
            s = " ERROR ";
        break;
        case (ExceptionType::fatal_error) :
            s = " FATAL_ERROR ";
    }
    cout << s << endl;
    cout << "- - - - - - - - - -" << endl;
    cout << "fichier : " << location.file_name() << endl;
    cout << "fonction : " << location.function_name() << endl;
    cout << "ligne : " << location.line() << endl;
    if(obj) cout << "adresse de l’object : " << &obj << endl;
    cout << "- - - - - - - - - -" << endl;
    cout << message << endl;
    cout << "- - - - - - - - - -" << endl;
}

//Constructeurs des classes filles
IndexOutOfRange::IndexOutOfRange(const void*  ptr, const std::source_location & location, size_t s, size_t i) :
    Exception(ptr, location, Exception::ExceptionType::fatal_error), indexMax(s), indexPosition(i){
    message = "Echec de l'accès à l'élément ";
    message += std::to_string(i);
    message += " de l'objet Tab de taille :  \n";
    message += " size : ";
    message += std::to_string(s);
}

ForbidenConstruction::ForbidenConstruction(const std::source_location & location) :
    Exception(NULL, location, Exception::ExceptionType::error){
}

SuspectConstruction::SuspectConstruction(const std::source_location & location) :
    Exception(NULL, location, Exception::ExceptionType::warning){
}

void conversionErrorDetected(const std::source_location & l){
     conversionFailure e(NULL, l, Exception::ExceptionType::fatal_error);
     e.message = "Une conversion entre deux types d'un élément essentiel au programme a échoué.";
     e.affiche();
     if(Exception::THROW) throw e;
     else exit(-1);
}


void fileFormatErrorDetected(const std::source_location & l , const std::string & path){
     wrongFileFormat e(NULL, l, Exception::ExceptionType::fatal_error);
     e.message = "information manquante on illisible dans le fichier : \"";
     e.message += path;
     e.message += "\" .";
     e.affiche();
     if(Exception::THROW) throw e;
     else exit(-1);
}

/*inutile créé une erreur de compilation
ForbidenType::ForbidenType(const void* ptr, const std::source_location & location, const std::type_info & id):
    Exception(ptr, location, ExceptionType::fatal_error){
        message = "Type interdit : ";
        message += id.name();
    }*/
