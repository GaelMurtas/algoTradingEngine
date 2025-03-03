#include "utilsException.hpp"

template<typename T> size_t tabExt2ConstChec(const size_t & nl, const size_t & nc){
    size_t prod = nl*nc;
    /*if((nl<1)||(nc<1)){
        TabExt2Error<T> e(prod,prod,nl,nc);
        e.constructionFailed();}*/
    return prod;
}

template<typename T> TabExt2<T>::TabExt2(const tabExt<T> & ntab):tabExt<T>(ntab){
    nbl = ntab.nbl;
    nbc = ntab.nbc;
}

template<typename T> TabExt2<T>::TabExt2(const size_t & nl, const size_t & nc):tabExt<T>(tabExt2ConstChec<T>(nl, nc)){
    nbl=nl;
    nbc=nc;
}

template<typename T> T& TabExt2<T>::get(const size_t & i, const size_t & j)
{
  /*if ((i<1)||(i>nbl)||(j<1)||(j>nbc))
    {
       TabExt2Error<T> e(*this);
       e.accessDenied(i,j);
    }*/
  return tabExt<T>::get1((i-1)*nbc+j);
}

template<typename T>  T TabExt2<T>::getCopie(const size_t & i, const size_t & j) const
{
  /*if ((i<1)||(i>nbl)||(j<1)||(j>nbc))
    {
       TabExt2Error<T> e(*this);
       e.accessDenied(i,j);
    }*/
  return tabExt<T>::getCopie1((i-1)*nbc+j);
}


template<typename T> size_t TabExt2<T>::nLines() const
{
    return nbl;
}

template<typename T> size_t TabExt2<T>::nCols() const
{
    return nbc;
}
