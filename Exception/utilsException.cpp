//Gestion d'exceptrion de la classe Tab
using namespace tabException;

void tabException::indexCheck(const void* ptr, const std::source_location & location, size_t s, size_t i){
    if(Exception::BYPASS) return;
    if (i >= s){
    IndexOutOfRange e(ptr, location, s, i);
    e.affiche();
    if(Exception::THROW) throw e;
    else exit(-1);
    }
}

/*tabException::AccessError::AccessError(const void* ptr, const std::source_location & location, size_t s):Exception(ptr, location, Exception::ExceptionType::fatal_error), size(s){
}

void tabException::AccessError::accessDenied(size_t s){
        message = "Echec de l'accès à l'élément ";
        message += std::to_string(s);
        message += " de l'objet Tab de taille :  \n";
        message += " size : ";
        message += std::to_string(size);
        affiche();
}*/

//Exception ConstrictorException
void tabException::constructorCheck(const std::source_location & location, size_t & s){
    if(Exception::BYPASS) return;
    if (s == 0){
        ForbidenConstruction e(location);
        e.message = "La construction d'un object \"Tab\" de taille 0 n'est pas permisse.\nVeuillez utiliser le constructeur par défauld à la place cependant l'exception à été géré.";
        e.affiche();
        s = 1;
        if(Exception::THROW) throw e;
    }
    if (s > 32000000000){
        SuspectConstruction e(location);
        e.message = "Construction d'un object \"Tab\" de taille ";
        e.message += std::to_string(s);
        e.message += ". L'object fera plus de 32Go, assurez-vous d'avoir suffisement de RAM.";
        e.affiche();
        if(Exception::THROW) throw e;
    }
}

/*ConstructorException::ConstructorException(const std::source_location & location, size_t s) :
    Exception(NULL, location, [](size_t s){if(s==0) return ExceptionType::error; return ExceptionType::warning;}(s)), size(s){
}

void ConstructorException::emptyTab(){
    message = "La construction d'un object \"Tab\" de taille 0 n'est pas permisse.\nVeuillez utiliser le constructeur par défauld à la place cependant l'exception à été géré.";
    affiche();
}
void ConstructorException::largeTab(){
    message = "Construction d'un object \"Tab\" de taille ";
    message += std::to_string(size);
    message += ". L'object fera plus de 32Go, assurez-vous d'avoir suffisement de RAM.";
    affiche();
}*/


//Gestion d'exception pour la classe TabExt
void TabExtException::constructorCheck(size_t & n, const std::source_location & location){
    if(Exception::BYPASS) return;
    try{
        if(n<1){
                TabExtException e(NULL,location, ExceptionType::error, n, n);
                e.constructionFailed();
        }
    }
    catch(TabExtException){
        if(Exception::THROW) throw;
        n = 1;
}
};

void TabExtException::accessCheck(const void* ptr, const std::source_location & location, const size_t & s, const size_t & a, const size_t & i){
    if(Exception::BYPASS) return;
    try{
        if ((i<1)||(i>s))
        {
        TabExtException e(ptr, location, ExceptionType::fatal_error, a, s);
        e.accessDenied(i);
        }
    }
    catch(TabExtException){
        if(Exception::THROW) throw;
        exit(-1);
    }
};

TabExtException::TabExtException(const void* ptr, const std::source_location & location, const ExceptionType & t, const size_t & s, const size_t & a):Exception(ptr, location, t), size(s), alloc(a){
};

void TabExtException::constructionFailed(){
            message = "Échec de la construction de l'objet TabExt avec les paramètes : \n"; 
            message += " size : " ;
            message += std::to_string(size);
            message += "\n alloc : ";
            message += std::to_string(alloc); 
            message += "\nParamètre corrigé avec la valeur 1.";
            affiche();
            throw *this;
        };

void TabExtException::accessDenied(const size_t & s){
            message = "Echec de l'accès à l'élément ";
            message += std::to_string(s);
            message += " de l'objet TabExt avec les paramètes :  \n";
            message += " size : ";
            message += std::to_string(size);
            message += "\n alloc : ";
            message += std::to_string(alloc);
            message += "\nCe tableau est indicer de 1 jusqu'à sa taille";
            affiche();
            throw *this;
        };
