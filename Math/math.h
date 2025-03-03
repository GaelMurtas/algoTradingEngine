/*********
 ********* GENERAL LIBRARY FOR MATH STATS AND PROBA
 *********/
#ifndef MATH_EXCEPTION
#define MATH_EXCEPTION 1

//compute the mean of the values on a given obj
template<class T> double mean(const T &);
//simple variance
template<class T> double var(const T &);
//sandard deviation
template<class T> double sd(const T &);

//option to use only the most recent values
//s indicate the number of value to use
template<class T, size_t s> double mean(const T &);
template<class T, size_t s> double var(const T &);
template<class T, size_t s> double sd(const T &);

#endif
