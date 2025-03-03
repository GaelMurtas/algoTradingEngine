#include "utilsException.hpp"

template<typename T, unsigned V, bool B> TabExt<T, V, B>::TabExt(): tab<T>(), alloc(0){
}

template<typename T, unsigned V, bool B> TabExt<T, V, B>::TabExt(size_t n): tab<T>(n), alloc(n){
    TabExtException::constructorCheck(n,std::source_location::current());
}

template<typename T, unsigned V, bool B> TabExt<T, V, B>::TabExt(const tab<T> & A) : tab<T>(A), alloc(A.length){
}

template<typename T, unsigned V, bool B> void TabExt<T, V, B>::extend(){
  if (length < alloc) return;
  alloc+=V*length+1;
  T* tmp = new T[alloc];
  for(size_t i=0;i<length;i++){tmp[i]=data[i];}
  delete[] data;
  data=tmp;
}

template<typename T, unsigned V, bool B> void TabExt<T, V, B>::pushBack(const T& C){
  extend();
  data[length]=C;
  ++length;
}

template<typename T, unsigned V, bool B> TabExt<T, V, B>& TabExt<T, V, B>::operator+(const T& A){
    pushBack(A);
  return *this;}

