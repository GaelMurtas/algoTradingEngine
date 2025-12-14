#include <tuple>
#include <concepts>
#include <type_traits>

#ifndef META
#define META

//maybe usefull in certain context
inline void emptyFunction(){
}


/*
 * Meta function to have the number of ars for variatic template
 */ 

template<int n, class arg1, class ... args>
struct nbTemplateArgs_impl{
   const int nb = nbTemplateArgs_impl<n+1, args...>::nb;
};

template<int n, class arg1>
struct nbTemplateArgs_impl<n, arg1>{
   const int nb = n+1;
};

template<class ... args>
consteval int nbTemplateArgs(){
return nbTemplateArgs_impl<0, args ...>::nb;
}


#endif
