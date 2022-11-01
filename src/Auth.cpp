//
// Created by Dmitry Morozov on 30/9/22.
//

#include "Auth.hpp"
#include "Env_Keeper.hpp"

/**
 * It checks if the user is authenticated.
 *
 * @param keeper the EnvKeeper object that was passed to the function
 * @param user_id The user id of the user you want to check.
 * @returns true if the user is authenticated, false otherwise.
 */
bool isAuthenticated(EnvKeeper *keeper, const long long &user_id) {

    auto ids = keeper->get_user_ids();

    return ids.find(user_id) != ids.end();
}