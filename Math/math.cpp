/*********
 ********* MATH CLASS AND FUNCTION IMPLEMANTATION FOR NON TEMPLATE PART
 ********* AND EXLICIT INSTANCIATION FOR TEMPLATES
 *********/

#include "../Utils/utils.h"
#include "math.h"
#include "math.ipp"

/*
 * explicit template instantiation 
 * to not include the tempate implemantion in other cpp
 * it may keep compilation time low
 */
//A FAIRE: généraliser l'instantiation explicite des templates aux autres bibliothjèques.
template double mean<tab<double>>(const tab<double> &);
template double var<tab<double>>(const tab<double> &);
template double sd<tab<double>>(const tab<double> &);
template double mean<tab<double>, 20>(const tab<double> &);
template double var<tab<double>, 20>(const tab<double> &);
template double sd<tab<double>, 20>(const tab<double> &);
