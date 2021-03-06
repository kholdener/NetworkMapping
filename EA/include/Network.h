/*
 * Network class impelents graph as a collection of weighted nodes and
 * weighted edges
 */

#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>


class Network
{
public:
   // Constructor used for reading input from a file
   Network(FILE *input_file);

   // Copy constructor
   Network(Network *network);

   // Constructor used for testing
   Network(double **matrix, double *weights, int size);

   virtual ~Network();

   // assignment operator
   Network& operator=(Network *network);

   inline int size() const { return m_size; }

   inline double node_weight(int node) const { return m_node_weights[node]; }

   inline double link_weight(int row, int column) const { return m_link_weights[row][column]; }

   /* 
    * Determines whether the network has a path from node1 to node2
    * such that every edge along the path is no less than the given weight
    */
   bool path_exists(int node1, int node2, double weight);

   bool embed_path(int node1, int node2, double weight);

   void print(FILE *f);
private:
   /*
    * Provided that a path (satisfying weight constraint) from node1 to node 2
    * exists and is encoded in node_ancestor array, reduces the capacity of the
    * this path by weight
    * @pre - path from node1 to node2, satisfying the constraint of 'weight' exists and
    *        is encoded in node_ancestor array of size m_size. For example, if the path
    *        from node 0 to node 4 is: 0, 3, 5, 4, then node_ancestor array will be
    *        [-1 , -1, 0, 5, 3]
    * @post - the capacity of each edge along the path from node1 to node 2 is reduced
    *         by 'weight'
    *
    */
   void reduce_capacity(int node1, int node2, int *node_ancestor, double weight);

   /*  
    * Initializes m_size based on the number of nodes defined in the input_file
    * @param input_file [IN] input file in the format:
    * node1 node2 edge_weight (optional)
    * ...
    * node1_weight
    * node2_weight
    * ....
    */
   void init_size(FILE *input_file);

   double **m_link_weights; // graph representation, with edge weights
   double  *m_node_weights;     // weights on graph nodes
   int      m_size;        // number of graph nodes
};
#endif

