#include "PlayerDetails.h"



PlayerDetails::PlayerDetails(int player_id, int player_level, int player_group_id) : id(player_id), level(player_level),
group_id(player_group_id) 
{
    player_info_in_system = NULL;
}

PlayerDetails::PlayerDetails(PlayerDetails& player)
{
    id = player.id;
    level = player.level;
    group_id = player.group_id;
    player_info_in_system = player.player_info_in_system;
}

PlayerDetails::~PlayerDetails()
{
    player_info_in_system = NULL;
}

int PlayerDetails::getId()
{
    return id;
}

int PlayerDetails::getLevel()
{
    return level;
}

int PlayerDetails::getGroupId()
{
    return group_id;
}

void PlayerDetails::setLevel(int new_level)
{
    level = new_level;
}

void PlayerDetails::setGroupId(int new_group_id)
{
    group_id = new_group_id;
}

void PlayerDetails::addToLevel(int amount)
{
    level += amount;
}

void PlayerDetails::setPlayerInfo(PlayerDetails* new_player_info)
{
    player_info_in_system = new_player_info;
}

PlayerDetails& PlayerDetails::getPlayerInfo()
{
    return *(player_info_in_system);
}

PlayerDetails& PlayerDetails::operator=(const PlayerDetails& player)
{
    if(this == &player)
    {
        return *this;
    }
    id = player.id;
    level = player.level;
    group_id = player.group_id;
    player_info_in_system = player.player_info_in_system;
    return *this;
}

bool PlayerDetails::operator<(const PlayerDetails& player)
{
    if(this == &player)
    {
        return false;
    }
    if(level < player.level)
    {
        return true;
    }
    if(level == player.level && player.id < id)
    {
        return true;
    }
    return false;
}

//For tests
std::ostream& operator<<(std::ostream& is, PlayerDetails& player)
{
    return is << player.getId();
}
