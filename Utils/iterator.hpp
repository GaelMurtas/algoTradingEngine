#ifndef ITERATOR_EXCEPTION
#define ITERATOR_EXCEPTION 1
//#include <iostream>
//#include <fstream>
//#include <functional>
//#include <cmath>
//#include <utility>
#include <type_traits>
#include "../Exception/exception.h"
//#include "../Trading/trading.h"
//#include "iterator.hpp"

/*
 * Definition of a gereric Iterator
 * use it on data structure on data of type T
 * it use a simple size_t to check the position and the [] operator to access to element
 */

//on veut pouvoir obtenir un type constant ou non en fonction du besoin
template<class T, bool Const> struct isConst{
     using type = T;
};
template<class T> struct isConst<T, true>{
     using type = const T;
};

//T is the type of the data not the strusture, S is the structure type
template<class S,class T, bool Const>
class Iterator
{
     using dataPtr = isConst<T, Const>::type *;
     using dataRef = isConst<T, Const>::type &;
     using structPtr = isConst<S, Const>::type *;
     using structRef = isConst<S, Const>::type &;


   private :
     //indice actuel de l'itérateur
     size_t current;
     //adresse de l'objet auquel l'itérateur est associé
     structPtr object;

   public:
     Iterator(structRef obj): current(0), object(&obj){
     }

     Iterator(structRef obj, size_t index): current(index), object(&obj){
         tabException::indexCheck(this->object, std::source_location::current(), object->size(), current);
     }

     //accesseurs
     size_t index() const{
         return current;
     }

     dataRef operator*() const{
           return (*object)[current];
     }

     dataPtr operator ->() const{
         return &(*object)[current];
     }

     //prefix implement
     Iterator operator ++(){
         tabException::indexCheck(object, std::source_location::current(), object->size(), current);
         ++current;
         return *this;
     }

     //prefix implement
     Iterator operator ++(int useless){
         tabException::indexCheck(this->object, std::source_location::current(), object->size(), current);
         Iterator tmp = *this;
         ++current;
         return tmp;
     }

     bool operator ==(const Iterator & it) const{
         return (current == it.current) && (object == object);
     }

     bool operator !=(const Iterator & it) const{
         return (current != it.current) || (object != object);
     }

     bool operator <(const Iterator & it)const{
         return (current < it.current) && (object == object);
     }
 };
#endif
