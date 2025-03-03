using namespace std;

Table::Table(const TabExt2<string> & stab): TabExt2<string>::TabExt2<string>(stab){
    //initatilisation des autres arguments
    dl = '\n';
    dc = ',';
    df = ' ';
}

Table::Table(const size_t & l, const size_t & c): TabExt2<string>::TabExt2<string>(l,c){
    dl = '\n';
    dc = ',';
    df = ' ';
}

Table::Table(const string & path, const char & delL, const char & delC, const char & def):
    TabExt2<string>(getFileLines(path,delL), getFileCols(path,delL,delC)){
    string data = readFile(path);
    size_t i = 1;
    size_t j = 1;
    size_t l = data.length()-1;//le dernier caractère est un /n inutile
    for (size_t k=0; k<l; k++){
        if(data[k] == delL){
            i++;
            j=1;}
        else{
            if(data[k] == delC){ j++;}
            else{
                if(data[k]!=def) get(i,j)+=data[k];}
        }
    }
    //initialisation par défault des valeurs nulles
    for(i=1;i<=nLines();i++){
        for(j=1;j<=nCols();j++){
            if(get(i,j).empty())get(i,j)=def;
        }
    }
    //initatilisation des autres arguments
    dl = delL;
    dc = delC;
    df = def;
}

Table::Table(const string & path):
    Table(path,'\n',',',' '){};

string Table::toString() const{
    string s;
    for(size_t i=1; i<=nLines();i++){
        for(size_t j=1;j<nCols();j++){
                    s += getCopie(i,j);
                    s += dc;
                    s += df;
                }
                s += getCopie(i,nCols());
                s += dl;
    }
    return s;
}


void Table::affiche() const{
    cout << "----------\n" << toString() << "----------\n";
}

void Table::CSV(const string & path) const{
    ofstream file(path, ifstream::out);
    if (!file.is_open()) {
        FileError e(path);
        e.accessError();
    }
    file << toString();
}

string readFile(const string & path) {
    string s;
    char c;
     ifstream file(path, ifstream::in);
    if (!file.is_open()) {
        FileError e(path);
        e.accessError();
    }
    while(file.get(c)){ s += c;}
    file.close();
    if (s.empty()) {
        FileError e(path);
        e.emptyFile();
    }
    return s;
}

size_t getFileLines( const string & path, const char & delL){
    string data = readFile(path);
    size_t i = 1;
    size_t l = data.length()-1;//le dernier caractère est un /n inutile
   for (size_t k=0; k<l; k++){
    if(data[k] == delL) i++;
    }
   return i;
}

size_t getFileCols( const string & path, const char & delL, const char & delC){
    string data = readFile(path);
    size_t j = 1;
    size_t count = 1;//en cas d'inégalité des lignes on veut récupéré la valeur de la plus grande ligne
    size_t l = data.length();
    for (size_t k=0; k<l; k++){
     if(data[k] == delL){
        if(j>count)count=j;
            j=1;}
            if(data[k] == delC){ j++;}
    }
   return count;
}
