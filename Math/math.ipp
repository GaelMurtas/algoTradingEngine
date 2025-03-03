/*********
 ********* MATH TEMPLATES METHODES AND FUNCTIONS IMPLEMANTATION
 *********/

#include<math.h>

template<class T>
double mean(const T & V)
{
  double M = 0;
  for (auto x:  V)
    {
      M += x;
    }
  double N = M/V.size();
  return N;
}


template<class T>
double var(const T & V)
{
  double M = 0;
  for (auto x:  V)
    {
      M += x*x;
    }
  return M/V.size() - mean(V)*mean(V);
}

template<class T>
double sd(const T & V)
{
  return sqrt(var<T>(V));
}

//A FAIRE: GESTION D'EXCEPTION
//si s > size déclanché un warning et faire le calcul avec la taille max
template<class T, size_t s>
double mean(const T & V)
{
  double M = 0;
  for (size_t i = V.size()-s; i<V.size();  ++i)
    {
      M += V[i];
    }
  double N = M/s;
  return N;
}

template<class T, size_t s>
double var(const T & V)
{
  double M = 0;
  for (size_t i = V.size()-s; i<V.size();  ++i)
    {
      M += V[i]*V[i];
    }
  return M/s - mean<T,s>(V)*mean<T,s>(V);
}

template<class T, size_t s>
double sd(const T & V)
{
  return sqrt(var<T,s>(V));
}



