#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include "library1.h"
#include "Node.h"
#include "PlayerDetails.h"
#include "GroupDetails.h"
#include "AvlTree.h"

class PlayersManager {

    private:
       AvlTree<PlayerDetails> players_in_game_by_level;
       AvlTree<PlayerDetails> players_in_game_by_ID;
       AvlTree<GroupDetails> non_empty_groups;
       AvlTree<GroupDetails> groups;
       int num_players;

       const int CONSTANT = 0;

       template <class T>
       static T* mergeSort(T* array1, int len1, T* array2, int len2)
       {
         int length = len1 + len2;
         T* new_array;
         try
         {
            new_array = (T*)malloc(length * sizeof(T));
         }
         catch(const std::bad_alloc& e)
         {
            throw e;
         }

         int index = 0, i1 = 0, i2 = 0;
         while(i1 < len1 && i2 < len2)
         {
            if(array1[i1] < array2[i2])
            {
               new_array[index] = array1[i1];
               i1++;
            }
            else
            {
               new_array[index] = array2[i2];
               i2++;
            }
            index++;
         }

         if(i1 == len1)
         {
            while(i2 < len2)
            {
               new_array[index] = array2[i2];
               i2++;
               index++;
            }
         }
         else //i2 == len2
         {
            while(i1 < len1)
            {
               new_array[index] = array1[i1];
               i1++;
               index++;
            }
         }

         return new_array;
      }



      static void buildWholeTree(Node<PlayerDetails>* start, int n)
      {
         assert(start != NULL && n >= 0);

         try{
            start->setHeight(n);
            if(n == 0)
            {
               return;
            }
            
            start->setLeft(new Node<PlayerDetails>());
            start->getLeft()->setUp(start);
            buildWholeTree(start->getLeft(), n-1);
            start->setRight(new Node<PlayerDetails>());
            start->getRight()->setUp(start);
            buildWholeTree(start->getRight(), n-1);
         }catch(const std::bad_alloc& e){
            throw e;
         }
      }

      
      static void makeAlmostWhole(Node<PlayerDetails>* start, int* num)
      {
         assert(start != NULL && num != NULL && *num >= 0);
         if(*num == 0)
         {
            return;
         }
         if(start->getHeight() == 0)
         {
            *num = *num - 1;
            if(start->getUp() != NULL)
            {
               if(start->getUp()->getRight() == start)
               {
                  start->getUp()->setRight(NULL);
               }
               else
               {
                  start->getUp()->setLeft(NULL);
                  start->getUp()->setHeight(0);
               }
            }
            delete start;
         }
         else
         {
            makeAlmostWhole(start->getRight(), num); 

            makeAlmostWhole(start->getLeft(), num);
            
            start->updateHeight();
         }
      }


      
      static void buildAlmostWholeTree(Node<PlayerDetails>* start, int n, int num)
      {
         assert(start != NULL && n >= 0 && num >= 0);

         try{
            buildWholeTree(start, n);
            makeAlmostWhole(start, &num);
         }catch(const std::bad_alloc& e)
         {
            throw e;
         }
      }



      static void addArray(Node<PlayerDetails>* start, PlayerDetails* array, int GroupID, int* index)
      {
         assert(index != NULL);
         if(start == NULL)
         {
            return;
         }
         addArray(start->getLeft(), array, GroupID, index);

         PlayerDetails* player = new PlayerDetails(array[*index].getId(), array[*index].getLevel(), GroupID);
         player->setPlayerInfo(&(array[*index].getPlayerInfo()));
         start->SetInfo(player);
         start->SetMainKey(player->getLevel());
         start->SetSecondaryKey(player->getId());
         player->getPlayerInfo().setGroupId(GroupID);
         *index = *index + 1;

         addArray(start->getRight(), array, GroupID, index);
      }


    public:
       
       PlayersManager()
       {
          num_players = 0;
       }

       ~PlayersManager()
       {

       }

       AvlTree<GroupDetails>& getGroups() { return groups;}

       AvlTree<PlayerDetails>& getPlayersByID() { return players_in_game_by_ID;}

       AvlTree<PlayerDetails>& getPlayersByLevel() { return players_in_game_by_level;}

       AvlTree<GroupDetails>& getNonEmptyGroupsTree() { return non_empty_groups;}

       StatusType AddGroup(int GroupID);

       StatusType AddPlayer(int PlayerID, int level, int GroupID);

       StatusType RemovePlayer(int PlayerID);

       StatusType ReplaceGroup(int GroupID, int ReplacementID);

       StatusType IncreaseLevel(int PlayerID, int LevelIncrease);

       StatusType GetHighestLevel(int GroupID, int* PlayerID);

       StatusType GetAllPlayersByLevel(int GroupID, int** Players, int* numOfPlayers);

       StatusType GetGroupsHighestLevel(int numOfGroups, int** Players);

       void Quit();

};

#endif