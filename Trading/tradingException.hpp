/*
 * GESTION DES EXCEPTIONS POUR LA BIBLIOTHÈQUE TRADING.H
 */

//Classe Bougie
class Bougie_Incorrecte{
  public:
    Bougie bougie;
    
    Bougie_Incorrecte(const Bougie & b){
      bougie = b;}
      
    void Message(){
        std::cout << "La bougie de valeur : ";
        bougie.affiche();
        std::cout << " est incorrecte. \n";}
};

//Vlasse TimeFrame
class TimeFrameException{
    TimeFrame tf;

    public:
    TimeFrameException(const TimeFrame & tf2){
        tf = tf2;
        }

    static void constructionImpossible(const TimeUnit & t, const size_t & s){
        std::cout << "Tentative de construction d'une timeFrame de taille nulle ! \n";
        std::terminate();
    }

    void dureeIndivisibles( const TimeFrame & tf2){
        std::cout << "Les times frames suivantes sont non divisibles : \n";
        tf.affiche();
        tf2.affiche();
        throw *this;
    }
};

//Classe Chart
class ChartException{
    const Chart * chartAdr;
    public:
        ChartException(const Chart * adr){
            chartAdr = adr;
        }
        void constructionImpossible(){
            std::cout << "Construction de l'objet Chart Impossible !" <<std::endl;
            throw(*this);
        }
};
