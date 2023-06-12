#ifndef AVLTREE_H
#define AVLTREE_H

#include "Node.h"
#include <iostream>


template <class T>
class AvlTree {
    private:
        Node<T>* root;
        Node<T>* max;


        static void inOrderForT(Node<T>* node, T* array, int* index)
        {
            if (node == NULL)
                return;
            inOrderForT(node->getLeft(), array, index);
            array[*index] = node->GetInfo();
            *index = *index + 1;
            inOrderForT(node->getRight(), array, index); 
        }

        static void reverseInOrderForLevel(Node<T>* node, int* array, int* index)
        {
            if (node == NULL)
                return;
            reverseInOrderForLevel(node->getRight(), array, index); 
            array[*index] = node->GetSecondaryKey();
            *index = *index + 1;
            reverseInOrderForLevel(node->getLeft(), array, index);
        }

        static void inOrderForLevel(Node<T>* node, int* array, int* index)
        {
            if (node == NULL)
                return;
            inOrderForLevel(node->getLeft(), array, index);
            array[*index] = node->GetMainKey();
            *index = *index + 1;
            inOrderForLevel(node->getRight(), array, index); 
        }

        static void inOrderForID(Node<T>* node, int* array, int* index)
        {
            if (node == NULL)
                return;
            inOrderForLevel(node->getLeft(), array, index);
            array[*index] = node->GetSecondaryKey();
            *index = *index + 1;
            inOrderForLevel(node->getRight(), array, index); 
        }

        //Calculating the max of the int
        static int maximum(int a, int b)
        {
            if(a>b)
            {
                return a;
            }
            return b;
        }

        //Doing a right rotation
        static void rightRotation(Node<T>* B)
        {
            assert(B != NULL && B->getLeft() != NULL);
            Node<T>* A = B->getLeft();
            Node<T>* ar = A->getRight();
            B->setLeft(ar);
            if(ar != NULL)
            {
                ar->setUp(B);
            }
            A->setUp(B->getUp());
            A->setRight(B);
            B->setUp(A);

            //If A is not the root, update his father pointer to A
            if(A->getUp() != NULL)
            {
                if(A->getUp()->getRight() == B)
                {
                    A->getUp()->setRight(A);
                }
                else A->getUp()->setLeft(A);
            }

            //Updating A and B height
            B->updateHeight();
            A->updateHeight();
        }

        //Doing a left rotation
        static void leftRotation(AvlTree<T>* tree,Node<T>* A)
        {
            assert(A != NULL && A->getRight() != NULL);
            Node<T>* B = A->getRight();
            Node<T>* bl = B->getLeft();
            A->setRight(bl);
            if(bl != NULL)
            {
                bl->setUp(A);
            }
            B->setUp(A->getUp());
            B->setLeft(A);
            A->setUp(B);
            //If Bis not the root, update his father pointer to B
            if(B->getUp() != NULL)
            {
                if(B->getUp()->getRight() == A)
                {
                    B->getUp()->setRight(B);
                }
                else B->getUp()->setLeft(B);
            }
            //Updating A and B height
            A->updateHeight();
            B->updateHeight();
        }

        //Arranges the tree
        static void MakeOrder(AvlTree<T>* tree, Node<T>* start)
        {
            if(start == NULL)
            {
                return;
            }
            if(start->getBF() == -2)
            {
                if(start->getRight()->getBF() <= 0)
                {
                    //RR rotation
                    leftRotation(tree,start);
                } 
                else
                { 
                    //RL rotation
                    rightRotation(start->getRight());
                    leftRotation(tree,start);
                }
            }
            else if(start->getBF() == 2)
            {
                if(start->getLeft()->getBF() >= 0)
                {
                    //LL rotation
                    rightRotation(start);
                }
                else
                { 
                    //LR rotation
                    leftRotation(tree,start->getLeft());
                    rightRotation(start);
                }
            }

            start->updateHeight();

            //Update to root if needed
            if(start->getUp() == NULL)
            {
                tree->SetRoot(start);
                return;
            }
            MakeOrder(tree, start->getUp());
        }

        //Getting the next node in order
        static Node<T>* getNext(Node<T>* node)
        {
            assert(node != NULL && node->getLeft() != NULL && node->getRight() != NULL);
            Node<T>* ptr = node->getRight();
            while(ptr->getLeft() != NULL)
            {
                ptr = ptr->getLeft();
            }
            return ptr;
        }

        //Remove leaf from tree
        static void removeLeaf(AvlTree<T>* tree, Node<T>* V)
        {   
               Node<T>* father = V->getUp();

               //Update the max if V is max
               if(tree->GetMax()->GetMainKey() == V->GetMainKey() && tree->GetMax()->GetSecondaryKey() == V->GetSecondaryKey())
               {
                   tree->SetMax(father);
               }

               V->setUp(NULL);
               if(father !=NULL)
               {
                   if(father->getRight() == V)
                   {
                       father->setRight(NULL);
                   }
                   else if(father->getLeft() == V)
                   {
                       father->setLeft(NULL);
                   }
               }
               //V is leaf and root
               else
               {
                   tree->SetRoot(NULL);
               }
               delete V;
               MakeOrder(tree, father);
               return;
        }

        //Remove node with one child
        static void removeNodeWithOneChild(AvlTree<T>* tree, Node<T>* V)
        {
               //Get V child
               Node<T>* son;
               if(V->getRight() != NULL)
               {
                   son = V->getRight();
               }
               else 
               {
                   son = V->getLeft();
               }

               //Update tree max if V is the max
               if(tree->GetMax()->GetMainKey() == V->GetMainKey() && tree->GetMax()->GetSecondaryKey() == V->GetSecondaryKey())
               {
                   tree->SetMax(son);
               }

               //Get V father
               Node<T>* father = V->getUp();
               if(father == NULL)
               {
                   tree->SetRoot(son);
                   son->setUp(NULL);
                   delete V;
                   return;
               }
                if(V == father->getRight())
                {
                   father->setRight(son);
                }
                else
                {
                    father->setLeft(son);
                }
                son->setUp(father);
                delete V;
                MakeOrder(tree, father);
                return;
        }

        //Destroy the tree
        static void DestroyTree(Node<T>* node)
        {
            if(node==NULL)
            {
                return;
            }
            DestroyTree(node->getLeft());
            //node->setLeft(NULL);
            DestroyTree(node->getRight());
            //node->setRight(NULL);

            delete node;
        }
    public:

       //Constuctor
       AvlTree()
       {
           root=NULL;
           max=NULL;
       }

       //Defult distructor
       ~AvlTree()
       {
           DestroyTree(root);
       }

       //Getting tree root
       Node<T>* GetRoot()
       {
           return root;
       }
       
       //Getting max node of tree
       Node<T>* GetMax()
       {
           return max;
       }

       //Setting tree's root to new_root
       void SetRoot(Node<T>* new_root)
       {
          root = new_root;
       }
       
       //Set tree's max to new_max
       void SetMax(Node<T>* new_max)
       {
           max = new_max;
       }


       AvlTree<T>& operator=(const AvlTree<T>& tree)
       {
           if(this == &tree)
           {
               return *this;
           }
           root = tree.root;
           max = tree.max;
           return *this;
       }


       //Insert a new node to tree
       void insert(int main_key, int secondary_key, T* val)
       { 
           if(root == NULL)
           {
                try
                {
                    root = new Node<T>(*val, main_key, secondary_key);
                }catch(const std::bad_alloc& e)
                {
                    throw e;
                }
               max = root;
               return;
           }
           Node<T>* ptr = root;
           bool is_right = false;
           while(ptr != NULL)
           {
               ptr->AddHeight();
               if(ptr->nodeCmp(main_key, secondary_key))
               {
                   
                   if(ptr->getLeft()==NULL)
                   {
                       is_right = false;
                       break;
                   }
                   ptr = ptr->getLeft();
               }
               else
               {
                   if(ptr->getRight()==NULL)
                   {
                       is_right = true;
                       break;
                   }
                   ptr = ptr->getRight();
               }
           }
           if(is_right)
           {
               try
                {
                    ptr->setRight(new Node<T>(*val,main_key,secondary_key));
                }catch(const std::bad_alloc& e)
                {
                    throw e;
                }
                ptr->getRight()->setUp(ptr);
                ptr = ptr->getRight();
           }
           else 
           {
               try
                {
                    ptr->setLeft(new Node<T>(*val,main_key,secondary_key));
                }catch(const std::bad_alloc& e)
                {
                    throw e;
                }
                ptr->getLeft()->setUp(ptr);
                ptr = ptr->getLeft();
           }
           if(max->nodeCmp(main_key, secondary_key) == false)
           {
               max = ptr;
           }
           MakeOrder(this, ptr);
       }

       //Search for a node
       Node<T>* find(int main_key, int secondary_key)
       {
           Node<T>* ptr = root;
           while(ptr != NULL)
           {
               if(ptr->GetMainKey() == main_key && ptr->GetSecondaryKey() == secondary_key)
               {
                   return ptr;
               }

               if(ptr->nodeCmp(main_key, secondary_key))
               {
                   ptr = ptr->getLeft();
               }
               else
               {
                   ptr = ptr->getRight();
               }
           }
           return NULL;
       }

       //Remove node from tree
       void remove(int main_key, int secondary_key)
       {
           //Search if the node exist
           Node<T>* V = this->find(main_key, secondary_key);
           if(V == NULL)
           {
               return;
           }

           //If V is leaf - remove V
           if(V->isLeaf())
           {
               removeLeaf(this,V);
               return;
           }

           //If V has one child - replace V with his son
           if(V->hasOneChild())
           {
               removeNodeWithOneChild(this,V);
               return;
           }
            
            //Replace V with the next node in order (next must be a leaf of has one child)
            Node<T>* next = getNext(V);
            V->switchWithNode(next);
            //If next is leaf - remove next
           if(next->isLeaf())
           {
               removeLeaf(this,next);
               return;
           }

           //If next has one child - replace next with his son
           if(next->hasOneChild())
           {
               removeNodeWithOneChild(this,next);
               return;
           }
       }

       int* makeArrayForLevel(int num)
       {
           int* array;
           try{
               array = (int*)malloc(num * sizeof(int));
           } catch(const std::bad_alloc& e)
         {
            std::cerr << e.what() << '\n';
         }
           int index = 0;
           inOrderForLevel(root, array, &index);
           return array;
       }

       int* makeArrayForID(int num)
       {
           int* array;
           try{
               array = (int*)malloc(num * sizeof(int));
           } catch(const std::bad_alloc& e)
         {
            std::cerr << e.what() << '\n';
         }
           int index = 0;
           inOrderForID(root, array, &index);
           return array;
       }

       int* makeRevereArrayForLevel(int num)
       {
            int* array;
            try{
                array = (int*)malloc(num * sizeof(int));
            } catch(const std::bad_alloc& e)
            {
                throw e;
            }
           int index = 0;
           reverseInOrderForLevel(root, array, &index);
           return array;
       }

       T* makeArrayForT(int num)
       {
            if(num <= 0)
            {
                return NULL;
            }
            T* array;
            try{
                array = (T*)malloc(num * sizeof(T));
            } catch(const std::bad_alloc& e)
            {
                throw e;
            }
            int index = 0;
            inOrderForT(root, array, &index);
            return array;
       }



};

#endif