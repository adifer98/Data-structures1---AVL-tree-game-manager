#ifndef PLAYER_DETAILS_H
#define PLAYER_DETAILS_H

#include "AvlTree.h"
#include <memory>


class PlayerDetails {
    private:
        int id;
        int level;
        int group_id;
        PlayerDetails* player_info_in_system;

    public:
        PlayerDetails(int player_id=0, int player_level=0, int player_group_id=0);
        PlayerDetails(PlayerDetails& player);
        ~PlayerDetails();
        int getId();
        int getLevel();
        int getGroupId();
        void setLevel(int new_level);
        void setGroupId(int new_group_id);
        void addToLevel(int amount);
        void setPlayerInfo(PlayerDetails* new_player_info);
        PlayerDetails& getPlayerInfo();
        PlayerDetails& operator=(const PlayerDetails& player);
        bool operator<(const PlayerDetails& player);
        

        //For tests
        friend std::ostream& operator<<(std::ostream& is, PlayerDetails& player);

        
};


#endif