//
// Created by Dmitry Morozov on 30/9/22.
//

#include "Auth.hpp"
#include "Env_Keeper.hpp"

bool isAuthenticated(EnvKeeper *keeper, const long long &user_id) {

    auto ids = keeper->get_user_ids();

    return ids.find(user_id) != ids.end();
}