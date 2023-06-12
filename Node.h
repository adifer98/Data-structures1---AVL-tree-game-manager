#ifndef NODE_H
#define NODE_H

#include <memory>
#include <cassert>
#include <iostream>
#include <math.h> 

template <class T>
class Node {
    T* info;
    Node* up;
    Node* left;
    Node* right;
    int main_key;
    int secondary_key;
    int height;

    public:
       //Constructor
       Node(T& node_info, int node_main_key=0, int node_secondary_key=0) : main_key(node_main_key), secondary_key(node_secondary_key)
       {
            try
            {
                info = new T(node_info);
            }catch(const std::bad_alloc& e)
            {
                throw e;
            }
            height = 0;
            left = NULL;
            right = NULL;
            up = NULL;
       }
    
       //Default Constructor 
       Node()
       {
           main_key = 0;
           secondary_key = 0;
           height = 0;
           info = NULL;
           left = NULL;
           right = NULL;
           up = NULL;
       }
       
       //Destructor 
       ~Node()
       {
           delete info;
           left = NULL;
           right = NULL;
           up = NULL;
       }

       //Get main key
       int GetMainKey() {return main_key;}

       //Get secondary key
       int GetSecondaryKey() {return secondary_key;}
    
       //Get left child
       Node* getLeft() {return left;}

       //Get right child
       Node* getRight() {return right;}

       //Get father
       Node* getUp() {return up;}

       //Get height
       int getHeight() {return height;}

       //Set node's height
       void setHeight(int h) { height = h;}
       
       //Set node's left child
       void setLeft(Node* node) { left = node; }

       //Set node's right child
       void setRight(Node* node) { right = node; }

       //Set node's father
       void setUp(Node* node) { up = node; }

       //Adding 1 to node's height
       void AddHeight() {height++;}

       //Set node's main key
       void SetMainKey(int new_key) {this->main_key = new_key;}

       //Set node's secondary key
       void SetSecondaryKey(int new_key) {this->secondary_key = new_key;}
 
       //Get node's info
       T& GetInfo(){return *(this->info);}

       //Set node's info
       void SetInfo(T* new_info)
       {
           info = new_info;
       }

       //returns true if the particular node is greater than the other node, and returns false elsewhere.
       bool nodeCmp(int node_main_key, int node_secondary_key)
       {
           assert(*this != NULL);
           if(main_key > node_main_key)
           {
               return true;
           }
           if(main_key < node_main_key)
           {
                return false;
           }
           if(secondary_key < node_secondary_key)
           {
               return true;
           }
           return false;
       }

       //Check if the node is a leaf. Return true if so, false otherwise.
       bool isLeaf()
       {
           if(right == NULL && left == NULL)
           {
               return true;
           }
           return false;
       }
    
       //Check if the node has one child. Return true if so, false otherwise.
       bool hasOneChild()
       {
           if(right == NULL && left != NULL)
           {
               return true;
           }
           if(right != NULL && left == NULL)
           {
               return true;
           }
           return false;
       }

       //Return node's BF
       int getBF()
       {
           return nodeGetHighet(left) - nodeGetHighet(right);
       }
       
       //Swap between two nodes
       void switchWithNode(Node* node)
       {
           /*Node* temp_left = left;
           Node* temp_right = right;
           Node* temp_up = up;
           Node* temp_node_left = node->left;
           Node* temp_node_right = node->right;
           Node* temp_node_up = node->up;

           int temp_height = height;

           node->setUp(temp_up);
           if(node->getUp() != NULL)
           {
               if(node->getUp()->getLeft() == this)
               {
                   node->getUp()->setLeft(node);
               }
               else node->getUp()->setRight(node);
           }
           node->setLeft(temp_left);
           if(node->getLeft() != NULL)
           {
               node->getLeft()->setUp(node);
           }

           node->setRight(temp_right);
           if(node->getRight() != NULL)
           {
               node->getRight()->setUp(node);
           }
           
           //on this:

           up = temp_node_up;
           if(up != NULL)
           {
               if(up->getLeft() == node)
               {
                   up->setLeft(this);
               }
               else up->setRight(this);
           }

           left = temp_node_left;
           if(left != NULL)
           {
               left->up = this;
           }

           right = temp_node_right;
           if(right != NULL)
           {
               right->up = this;
           }

           height = node->height;
           node->height = temp_height;
*/
           int temp1 = main_key;
           int temp2 = secondary_key;
           T* temp3 = info;

           main_key = node->main_key;
           secondary_key = node->secondary_key;
           info = &node->GetInfo();

           node->main_key = temp1;
           node->secondary_key = temp2;
           node->SetInfo(temp3);
       }
       
       //Get node's height
       friend int nodeGetHighet(Node* node)
       {
          if(node == NULL)
          {
              return -1;
          }
       return node->height;
       }

       //Update the highet of node
        void updateHeight()
        {
            height = fmax(nodeGetHighet(left),nodeGetHighet(right)) + 1;
        }
};

#endif