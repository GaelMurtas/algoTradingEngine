template<typename T> Tab<T>::Tab(): data(nullptr), length(0){
}

template<typename T> Tab<T>::Tab(size_t n){
    tabException::constructorCheck(std::source_location::current(), n);
    if(n == 0) {n = 1;}
    data = new T[n];
    length = n;
}

template<typename T> Tab<T>::Tab(const Tab<T> & A) : data(new T[A.length]), length(A.length){
    //erreur si a est vide on utilisera le constructeur pas défault
    for(size_t i=0;i<length;i++){
        data[i]=A.data[i];
    }
}

template<typename T> Tab<T>::~Tab(){
    delete[] data;
}

template<typename T> void Tab<T>::remove(const size_t & e){
    if (e<length){//A faire: renvoyer un warning sinon
    for(size_t i=e;i<length-1;i++){
      data[i]=data[i+1];
    }
    length--;
  }
}

template<typename T> void Tab<T>::clear(){
    for(size_t i=0;i<length;i++){
      data[i]= T();
    }
    length = 0;
}

template<typename T> T Tab<T>::min() const {
    T min = data[0];
    for(size_t i = 1; i < length; ++i){
        if(data[i] < min){
            min = data[i];
        }
    }
    return min;
}

template<typename T> T Tab<T>::max() const {
    T max = data[0];
    for(size_t i = 1; i < length; ++i){
        if(data[i] > max){
            max = data[i];
        }
    }
    return max;
}

template<typename T> std::pair<bool, size_t> Tab<T>::search(const T & searched) const{
    for(size_t i = 0; i < length; ++i){
        if(getConst(i) == searched){
            return std::pair<bool, size_t>(true, i);
        }
    }
    return std::pair<bool, size_t>(false, (size_t)-1);
}

template<typename T> void Tab<T>::operator = (const Tab& tab){
    length = tab.length;
    delete[] data;
    data = new T[length];
    for(size_t i=0; i<length; i++){
        data[i] = tab.data[i];
    }
}


template<typename T> void Tab<T>::operator +=(const Tab& tab){
    size_t oldLength = length;
    T* oldData = data;
    length += tab.lenth;
    data = new T[length];
    size_t i = 0;
    for (; i < oldLength; i++){
        data[i] = oldData[i];
    }
    for (; i < length; i++){
        data[i] = tab[i-oldLength];
    }
    delete[] oldData;
}

template<typename T> Tab<T> Tab<T>::operator+(const Tab & A) const{
    Tab newTab(*this);
    newTab += A;
  return newTab;}
