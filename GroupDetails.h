#ifndef GROUP_DETAILS_H
#define GROUP_DETAILS_H

#include "AvlTree.h"
#include "PlayerDetails.h"
#include <memory>

class GroupDetails{
    private:
        int id;
        int num_players;
        AvlTree<PlayerDetails>* players_in_group;
        bool delete_tree;
        GroupDetails* group_info_in_system;

    public:
        GroupDetails(int group_id, int group_num_players=0);
        GroupDetails(GroupDetails& group);
        ~GroupDetails();
        void addPlayer(PlayerDetails& new_player);
        void removePlayer(PlayerDetails& new_player);
        PlayerDetails& findPlayer(int player_level, int player_id);
        void playerLevelIncrease(int player_level, int player_id, int amount);
        AvlTree<PlayerDetails>& getPlayersTree();
        void setPlayersTree(AvlTree<PlayerDetails>* new_players_tree);
        PlayerDetails& getHighestLevelPlayer();
        int getPlayersNum();
        void increasePlayerNum(int amount);
        void setGroupInfo(GroupDetails* new_group_info);
        GroupDetails& getGroupInfo();
        void setDeleteTree(bool new_delete_tree);


        //For tests
        int getId() { return id;}
        friend std::ostream& operator<<(std::ostream& is, GroupDetails& player);

};

#endif