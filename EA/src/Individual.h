/*
 * EA individual, representing a potential solution to the Network Mapping
 * problem, along with its fitness value
 */

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "NetworkMapping.h"

class Individual
{
public:
   /*
    * Constructor
    * @param from [IN] virtual network
    * @param to   [IN] physical network
    */
   Individual(Network *from, Network *to):
      m_solution(from, to),
      m_fitness(0)
   {}

   virtual ~Individual(){}

   void* operator new (size_t size, Individual *individual)
   {
      return individual;
   }


   NetworkMapping m_solution;

   double         m_fitness;
};

#endif
