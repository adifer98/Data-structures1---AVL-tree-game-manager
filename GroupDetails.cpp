#include "GroupDetails.h"
#include <memory>


GroupDetails::GroupDetails(int group_id, int group_num_players) : 
id(group_id), num_players(group_num_players)
{
    players_in_group = new AvlTree<PlayerDetails>();
    delete_tree = true;
    group_info_in_system = NULL;
}

GroupDetails::GroupDetails(GroupDetails& group)
{
    id = group.id;
    players_in_group = group.players_in_group;
    group_info_in_system = group.group_info_in_system;
    num_players = group.num_players;
    delete_tree =  group.delete_tree;
    group.delete_tree = false;
}


GroupDetails::~GroupDetails()
{
    group_info_in_system=NULL;
    if(delete_tree == true)
    {
        delete players_in_group;
    }
    else players_in_group = NULL;
}


void GroupDetails::addPlayer(PlayerDetails& new_player)
{
    this->increasePlayerNum(1);
    players_in_group->insert(new_player.getLevel(), new_player.getId(), &new_player);
}

void GroupDetails::removePlayer(PlayerDetails& new_player)
{
    players_in_group->remove(new_player.getLevel(), new_player.getId());
    this->increasePlayerNum(-1);
}

PlayerDetails& GroupDetails::findPlayer(int player_level, int player_id)
{
    return players_in_group->find(player_level, player_id)->GetInfo();
}


void GroupDetails::playerLevelIncrease(int player_level, int player_id, int amount)
{
    PlayerDetails& player = players_in_group->find(player_level, player_id)->GetInfo();
    players_in_group->remove(player_level, player_id);
    player.addToLevel(amount);
    players_in_group->insert(player.getLevel(), player_id, &player);
}

AvlTree<PlayerDetails>& GroupDetails::getPlayersTree()
{
    return *players_in_group;
}

void GroupDetails::setPlayersTree(AvlTree<PlayerDetails>* new_players_tree)
{
    AvlTree<PlayerDetails>* old_tree = players_in_group;
    players_in_group = new_players_tree;
    delete old_tree;
}

PlayerDetails& GroupDetails::getHighestLevelPlayer()
{
    return this->getPlayersTree().GetMax()->GetInfo();
}

int GroupDetails::getPlayersNum()
{
    return this->num_players;
}

void GroupDetails::increasePlayerNum(int amount)
{
    this->num_players += amount;
}

void GroupDetails::setGroupInfo(GroupDetails* new_group_info)
{
    group_info_in_system = new_group_info;
}

GroupDetails& GroupDetails::getGroupInfo()
{
    return *group_info_in_system;
}

void GroupDetails::setDeleteTree(bool new_delete_tree)
{
    delete_tree = new_delete_tree; 
}

//For tests
std::ostream& operator<<(std::ostream& is, GroupDetails& group)
{
    return is << group.getId();
}



