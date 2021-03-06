/* 
 * A class representing a mapping from a virtual network to a physical network
 */

#include "NetworkMapping.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm>

inline bool link_greater(const Link &l1, const Link &l2)
{
   return (l1.weight > l2.weight) ? true: false;
}


NetworkMapping::NetworkMapping(Network *from, Network *to):
   m_from(from),
   m_to(to),
   m_link_capacity(to),
   m_node_capacity(NULL),
   m_mapping(NULL),
   m_sorted_links(NULL),
   m_num_links(0),
   m_mapped_weights(NULL)
{
   if (m_from->size() > 0)
   {
      m_mapping = (int*)malloc(sizeof(int) * m_from->size());
      m_sorted_links = (Link*)malloc(sizeof(Link) * m_from->size() * m_from->size());
   }

   // initialize mapping to -1 (unmapped)
   for (int i = 0; i < m_from->size(); i++)
   {
      m_mapping[i] = -1;
      for (int j = i+1; j < m_from->size(); j++)
      {
         if (m_from->link_weight(i, j) >= 0)
         {
            m_sorted_links[m_num_links] = Link(i, j, m_from->link_weight(i, j));
            m_num_links++;
         }
      }
   }

   std::sort(m_sorted_links, m_sorted_links + m_num_links, link_greater);
   if (m_to->size() > 0)
   {
      m_node_capacity = (double*)malloc(sizeof(double) * m_to->size());
      m_mapped_weights = (double*)malloc(sizeof(double) * m_to->size());
   }
   reset_node_capacity();
}

NetworkMapping::~NetworkMapping()
{
   free(m_node_capacity);
   m_node_capacity = NULL;
   free(m_sorted_links);
   m_sorted_links = NULL;
   m_num_links = 0;
   free(m_mapping);
   m_mapping = NULL;
}

void NetworkMapping::reset_node_capacity()
{
   for (int i = 0; i < m_to->size(); i++)
   {
      m_node_capacity[i] = m_to->node_weight(i);
   }
}

void NetworkMapping::reset_link_capacity()
{
   m_link_capacity = m_to;
}

int NetworkMapping::valid_nodes()
{
   int node_count = 0;
   for (int i = 0; i < m_from->size(); i++)
   {
      if (m_from->node_weight(i) <= m_node_capacity[m_mapping[i]])
      {
         m_node_capacity[m_mapping[i]]-=m_from->node_weight(i);
         node_count++;
      }
   }
   reset_node_capacity();
   return node_count;
}

void NetworkMapping::calculate_mapped_weights()
{
   clear_mapped_weights();
   for (int i = 0; i < m_from->size(); i++)
   {
      m_mapped_weights[m_mapping[i]]+=m_from->node_weight(i);
   }
}

double NetworkMapping::node_balance_ratio_sum()
{
   double sum = 0;
   calculate_mapped_weights();
   for (int i = 0; i < m_to->size(); i++)
   {
      sum+=(m_mapped_weights[i]/m_to->node_weight(i));
   }
   return sum;
}

double NetworkMapping::node_balance_ratio_product()
{
   double product = 1;
   calculate_mapped_weights();
   for (int i = 0; i < m_to->size(); i++)
   {
      if (m_mapped_weights[i] > 0)
      {
         product*=(m_mapped_weights[i]/m_to->node_weight(i));
      }
   }
   return product;
}

void NetworkMapping::clear_mapped_weights()
{
   memset(m_mapped_weights, 0, m_to->size()*sizeof(double));
}

int NetworkMapping::valid_links()
{
   bool valid = false;
   int link_count = 0;
   for (int i = 0; i < m_num_links; i++)
   {
      if (m_sorted_links[i].weight >= 0)
      {
         valid = m_link_capacity.embed_path(m_mapping[m_sorted_links[i].row], 
            m_mapping[m_sorted_links[i].column], 
            m_sorted_links[i].weight);
         if (valid)
         {
            link_count++;
         }
      }
   }
   reset_link_capacity();
   return link_count;
}

void NetworkMapping::print_mapping(FILE *output_file)
{
   for (int i = 0; i < m_from->size(); i++)
   {
      fprintf(output_file, "%d -> %d\n", i, m_mapping[i]);
   }
}
