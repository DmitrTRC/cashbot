//
// Created by Dmitry Morozov on 30/9/22.
//

#ifndef CASHBOT_DEV_AUTH_HPP
#define CASHBOT_DEV_AUTH_HPP

#include "Env_Keeper.hpp"
//TODO: Refactor to Template Wrapper

//template<typename T>
//class Auth {
//    T f;
//public:
//    explicit Auth(T t) {
//
//        f = t;
//    }
//
//    T &operator()() {
//
//        return f;
//    }
//};
//
/////Use class Auth to wrap function
//void ifAuth() {
//
//    Auth auth = Auth([]() {
//        return 1;
//    });
//    auth();
//}
//
//bool isAuth() {
//
//    return true;
//
//}

/**
 * @brief Check if user is authorized
 * @param keeper EnvKeeper - pointer to EnvKeeper
 * @param user_id long long - user id
 * @returns true if user is authorized
 */
bool isAuthenticated(EnvKeeper *, const long long &);

#endif //CASHBOT_DEV_AUTH_HPP
