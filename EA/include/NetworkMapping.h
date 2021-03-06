/* 
 * A class representing a mapping from a virtual network to a physical network
 */

#ifndef NETWORK_MAPPING_H
#define NETWORK_MAPPING_H

#include "Network.h"

struct Link
{
   Link(int r, int c, double w):row(r),column(c),weight(w){}
   int row;
   int column;
   double weight;
};


class NetworkMapping
{
public:
   NetworkMapping(Network *from, Network *to);

   virtual ~NetworkMapping();

   /*
    * Calculates and returns the number of nodes whose weight constraints
    * are satisfied under this mapping
    */ 
   int valid_nodes();

   /*
    * Calculates and returns the ratio of the number of nodes whose weight 
    * constraints are satisfied under this mapping to the total number of nodes
    */
   inline double valid_nodes_ratio() { return ((double)valid_nodes()/m_from->size());}

   /*
    * Calculates and returns the product of the 
    * ratios of virtual nodes/assigned physical nodes
    */
   double node_balance_ratio_product();

   /*
    * Calculates and returns the sum of the 
    * ratios of virtual nodes/assigned physical nodes
    */
   double node_balance_ratio_sum();


   /*
    * Calculates and returns the number of links whose weight constraints
    * are satisfied under this mapping
    */
   int valid_links();

   /*
    * Calculates and returns the ratio of the the number of links whose weight 
    * constraints are satisfied under this mapping to the total number of links
    */
   inline double valid_links_ratio() { return ((double)valid_links()/m_num_links); }

   inline int get_from_size(){return m_from->size();}
   inline int get_to_size(){return m_to->size();}

   void print_mapping(FILE *output_file);
 
   int     *m_mapping;       // an array of size m_from->size()
private:

   void reset_node_capacity();
   void reset_link_capacity();
   void clear_mapped_weights();
   void calculate_mapped_weights();

   Network *m_from;
   Network *m_to;

   Network  m_link_capacity; // a copy of m_to, modified to reflect the risidual
                             // capacity of m_to under this mapping
   double  *m_node_capacity; // an array of size m_to->size()


   Link    *m_sorted_links;  // link indices of the m_from network, sorted in descending order
   int      m_num_links;     // size of m_sorted_links array

   double  *m_mapped_weights; // array storing the sum of virtual network node weights mapped 
                              // to each node of the physical network
                              // an array of size m_to->size()
};
#endif

