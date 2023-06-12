#include "PlayersManager.h"
#include <math.h> 

//Fot Test!
void inOrder(Node<PlayerDetails>* node)
{
    if (node == NULL)
     return;
    inOrder(node->getLeft());
    std::cout << "   " <<node->GetInfo() << std::endl;
    inOrder(node->getRight()); 
     
}


StatusType PlayersManager::AddGroup(int GroupID)
{
    if(GroupID <= 0)
    {
        return INVALID_INPUT;
    }
    if(groups.find(GroupID, CONSTANT))
    {
        return FAILURE;
    }
    GroupDetails new_group(GroupID);

    try
    {
        groups.insert(GroupID, CONSTANT, &new_group);
    }catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    
    return SUCCESS;
}


StatusType PlayersManager::AddPlayer(int PlayerID, int level, int GroupID)
{
    if(PlayerID <= 0 || level < 0 || GroupID <= 0)
    {
        return INVALID_INPUT;
    }
    Node<GroupDetails>* node_group = groups.find(GroupID, CONSTANT);
    if(node_group == NULL)
    {
        return FAILURE;
    }
    Node<PlayerDetails>* node_player = players_in_game_by_ID.find(PlayerID, CONSTANT);
    if(node_player != NULL)
    {
        return FAILURE;
    }

    PlayerDetails new_player(PlayerID, level, GroupID);

    //Insert the player to the palyers tree by ID
    try
    {
        players_in_game_by_ID.insert(PlayerID, CONSTANT, &new_player);
    }catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    
    new_player.setPlayerInfo(&(players_in_game_by_ID.find(PlayerID,CONSTANT)->GetInfo()));
    node_group->GetInfo().addPlayer(new_player);

    //Insert the player to the palyers tree by level
    try
    {
        players_in_game_by_level.insert(level, PlayerID, &new_player);
    }catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    
    
    num_players++;

    //Add the group to non empty groups tree if nedded
    if(node_group->GetInfo().getPlayersNum() == 1)
    {
        GroupDetails copy_group(GroupID,1);
        copy_group.setPlayersTree(&node_group->GetInfo().getPlayersTree());
        copy_group.setGroupInfo(&node_group->GetInfo());
        copy_group.setDeleteTree(false);

        try
        {
            non_empty_groups.insert(GroupID,CONSTANT,&copy_group);
        }catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }

    }
    //Update players num in non empty groups tree group
    else
    {
        non_empty_groups.find(GroupID,CONSTANT)->GetInfo().increasePlayerNum(1);
    }
    
    return SUCCESS;
}


StatusType PlayersManager::RemovePlayer(int PlayerID)
{
    if(PlayerID <= 0)
    {
        return INVALID_INPUT;
    }
    Node<PlayerDetails>* node_player = players_in_game_by_ID.find(PlayerID, CONSTANT);
    if(node_player == NULL)
    {
        return FAILURE;
    }

    PlayerDetails* PlayerInfo = &node_player->GetInfo();
    int groupID = PlayerInfo->getGroupId();

    Node<GroupDetails>* node_group = non_empty_groups.find(groupID,CONSTANT);

    //Delete the player from his group in non empty groups tree
    node_group->GetInfo().removePlayer(*PlayerInfo);
    node_group->GetInfo().getGroupInfo().increasePlayerNum(-1);

    //Delete the group from non empty group tree, if the group is epmty
    if(node_group->GetInfo().getPlayersNum()==0)
    {
        non_empty_groups.remove(groupID,CONSTANT);
    }

    //Delete the player from the other trees
    int PlayerLevel = PlayerInfo->getLevel();
    players_in_game_by_level.remove(PlayerLevel, PlayerID);
    players_in_game_by_ID.remove(PlayerID, CONSTANT);

    num_players--;

    return SUCCESS;
}



StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID)
{
    //checks inputs
    if(GroupID <= 0 || ReplacementID <= 0 || GroupID == ReplacementID)
    {
        return INVALID_INPUT;
    }
    //checks if the group exists
    Node<GroupDetails>* node_group = groups.find(GroupID, CONSTANT);
    Node<GroupDetails>* node_replacement = groups.find(ReplacementID, CONSTANT);
    if(node_group == NULL || node_replacement == NULL)
    {
        return FAILURE;
    }
    GroupDetails* groupID_info = &node_group->GetInfo();
    GroupDetails* replacement_info = &node_replacement->GetInfo();

    //If gruopID is empty
    if(groupID_info->getPlayersNum()==0)
    {
        groups.remove(GroupID,CONSTANT);
        return SUCCESS;
    }

    //makes two arrays
    int n_Group = groupID_info->getPlayersNum();
    int n_Replacement = replacement_info->getPlayersNum();

    PlayerDetails* array_group;
    PlayerDetails* array_replacement;
    try{
        array_group = groupID_info->getPlayersTree().makeArrayForT(n_Group);
    } catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    try{
        array_replacement = replacement_info->getPlayersTree().makeArrayForT(n_Replacement);
    } catch(const std::bad_alloc& e)
    {
        free(array_group);
        return ALLOCATION_ERROR;
    }
    
    

    //merges the arrays
    PlayerDetails* merged_array;
    try{
        merged_array = mergeSort(array_group, n_Group, array_replacement, n_Replacement);
    } catch(const std::bad_alloc& e)
    {
        free(array_group);
        free(array_replacement);
        return ALLOCATION_ERROR;
    }
    
    //makes a new tree:
    //gets the potential size of the tree
    int n = 0, num = 0;
    while(num < (n_Group + n_Replacement))
    {
        num += pow(2,n);
        n++;
    }
    //adresses the tree
    AvlTree<PlayerDetails>* mix_tree;
    Node<PlayerDetails>* root;
    try{
        mix_tree = new AvlTree<PlayerDetails>();
        root = new Node<PlayerDetails>();

    } catch(const std::bad_alloc& e)
    {
        free(array_group);
        free(array_replacement);
        free(merged_array);
        return ALLOCATION_ERROR;
    }
    mix_tree->SetRoot(root);


    //makes the tree
    int length = num - n_Group - n_Replacement;
    try{
        buildAlmostWholeTree(root, n - 1, length);
        int index = 0;
        addArray(root, merged_array, ReplacementID, &index);
    }catch(const std::bad_alloc& e)
    {
        free(array_group);
        free(array_replacement);
        free(merged_array);
        return ALLOCATION_ERROR;
    }
    
    //sets the max
    PlayerDetails max;
    PlayerDetails group_max = groupID_info->getHighestLevelPlayer();
    if(replacement_info->getPlayersNum() == 0)
    {
        max = group_max;
    }
    else
    {
        PlayerDetails replacement_max = replacement_info->getHighestLevelPlayer();
        if(group_max < replacement_max)
        {
            max = replacement_max;
        }
        else max = group_max;
    }
    max.setGroupId(ReplacementID);

    Node<PlayerDetails>* max_node = mix_tree->find(max.getLevel(), max.getId());  //!=NULL O(log(n_group+n_replacement))
    mix_tree->SetMax(max_node);

    //removes the groups
    non_empty_groups.remove(GroupID,CONSTANT);
    non_empty_groups.remove(ReplacementID,CONSTANT);
    groups.remove(GroupID, CONSTANT);
    
    //adresses and adds the new group tree
    replacement_info->setPlayersTree(mix_tree);
    
    //makes changes in the non empty groups tree
    GroupDetails copy_group(ReplacementID,n_Group+n_Replacement);
    copy_group.setPlayersTree(mix_tree);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    copy_group.setGroupInfo(replacement_info);
    copy_group.setDeleteTree(false);

    try{
        non_empty_groups.insert(ReplacementID,CONSTANT,&copy_group);
    } catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    replacement_info->increasePlayerNum(n_Group);

    free(array_group);
    if(array_replacement!=NULL)
    {
        free(array_replacement);
    }
    free(merged_array);

    return SUCCESS;
}



StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease)
{
    if(PlayerID <= 0 || LevelIncrease <= 0)
    {
        return INVALID_INPUT;
    }
    Node<PlayerDetails>* node_player = players_in_game_by_ID.find(PlayerID, CONSTANT);
    if(node_player == NULL)
    {
        return FAILURE;
    }
    int PlayerLevel = node_player->GetInfo().getLevel();
    int groupID = node_player->GetInfo().getGroupId();
    AvlTree<PlayerDetails>& players_tree = non_empty_groups.find(groupID,CONSTANT)->GetInfo().getPlayersTree();
    players_tree.remove(PlayerLevel, PlayerID);
    players_in_game_by_level.remove(PlayerLevel, PlayerID);
    node_player->GetInfo().setLevel(PlayerLevel + LevelIncrease);

    try{
        players_tree.insert(PlayerLevel + LevelIncrease, PlayerID, &node_player->GetInfo());
        players_tree.find(PlayerLevel + LevelIncrease, PlayerID)->GetInfo().setPlayerInfo(&node_player->GetInfo());
        players_in_game_by_level.insert(PlayerLevel + LevelIncrease, PlayerID, &node_player->GetInfo());
    } catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}


StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID)
{
    //Input check
    if(PlayerID==NULL || GroupID==0)
    {
        return INVALID_INPUT;
    }  

    //Getting the highest level player in the whole game
    if(GroupID<0)
    {
        if(this->num_players==0)
        {
            *PlayerID=-1;
            return SUCCESS;
        }
        *PlayerID = players_in_game_by_level.GetMax()->GetSecondaryKey();
        return SUCCESS;
    }

    //Getting the highest level player in GroupID
    else
    {
        //If the group doesnt exist
        if(groups.find(GroupID,CONSTANT)==NULL)
        {
            return FAILURE;
        }

        //If the group is empty
        if(groups.find(GroupID,CONSTANT)->GetInfo().getPlayersNum()==0)
        {
            *PlayerID=-1;
            return SUCCESS;
        }

        *PlayerID = groups.find(GroupID,CONSTANT)->GetInfo().getPlayersTree().GetMax()->GetSecondaryKey();
        return SUCCESS;
    }
    return SUCCESS;
}


StatusType PlayersManager::GetAllPlayersByLevel (int GroupID, int **Players, int *numOfPlayers)
{
    //Input check
    if(GroupID==0 || Players==NULL || numOfPlayers==NULL)
    {
        return INVALID_INPUT;
    }

    //~~~~~~~~~~If groupID>0 - return thr players of the group~~~~~~~~~~
    if(GroupID>0)
    {
        //If the group doesnt exist
        if(this->groups.find(GroupID,CONSTANT)==NULL)
        {
            return FAILURE;
        }

        GroupDetails& group = this->groups.find(GroupID,CONSTANT)->GetInfo();

        //If the group is empty
        if(group.getPlayersNum()==0)
        {
            *Players=NULL;
            *numOfPlayers=0;
            return SUCCESS;
        }

        //If there is players in the group
        try{
            *Players = group.getPlayersTree().makeRevereArrayForLevel(group.getPlayersNum());
        } catch(const std::bad_alloc& e)
        {
            return ALLOCATION_ERROR;
        }
        *numOfPlayers=group.getPlayersNum();
        return SUCCESS;
    }

    //~~~~~~~~~~If group<0 - return the players of the whole game~~~~~~~~~~
    //If there are no players in the game
    if(this->num_players==0)
    {
        *Players=NULL;
        *numOfPlayers=0;
        return SUCCESS;
    }
    //If there is players in the game
    try{
        *Players = this->players_in_game_by_level.makeRevereArrayForLevel(this->num_players);
    } catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    *numOfPlayers=this->num_players;
    return SUCCESS;
}


//Do inorder search on highest_players_in_groups tree numOfGroups times and put players ID on array
void nonEmptyGroupsTreeInorder(Node<GroupDetails>* node,int* counter, int* array, int numOfGroups)
{
    if(node==NULL || *counter==numOfGroups)
    {
        return;
    }
    nonEmptyGroupsTreeInorder(node->getLeft(),counter,array,numOfGroups);
    
    if(*counter==numOfGroups)
    {
        return;
    }
    array[*counter] = node->GetInfo().getPlayersTree().GetMax()->GetInfo().getId();
    *counter = *counter + 1;
    nonEmptyGroupsTreeInorder(node->getRight(),counter,array,numOfGroups);

}

StatusType PlayersManager::GetGroupsHighestLevel (int numOfGroups, int **Players)
{
    //Input check
    if(Players==NULL || numOfGroups<1)
    {
        return INVALID_INPUT;
    }

    //Making the array
    int* new_players;
    try{
        new_players = (int *)malloc(sizeof(int)*numOfGroups);
    }catch(const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    int counter=0;
    nonEmptyGroupsTreeInorder(non_empty_groups.GetRoot(),&counter,new_players,numOfGroups);

    //Check if there is less then numOfGroups players
    if(counter < numOfGroups)
    {
        free(new_players);
        return FAILURE;
    }
 
    *Players = new_players;
    return SUCCESS;
}



