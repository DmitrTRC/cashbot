//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef CASHBOT_DEV_FLOW_BOT_HPP
#define CASHBOT_DEV_FLOW_BOT_HPP

#include <tgbot/tgbot.h>


#include "Env_Keeper.hpp"
#include "db.hpp"


class FlowBot {
public:
    FlowBot ();

    ~FlowBot ();

    void Start ();

    void Stop ();

    void getInfo () const;

private:
    bool _is_running;
    EnvKeeper _env_keeper;
    TgBot::Bot *_bot;

    botDB _db_handler;

    void _initHandlers ();

    int get_last_message_id ();

    bool isAuthenticated (const long long &user_id);

    void send_wrong_auth_message (const long long &user_id);

    void _set_bot_commands ();
};


#endif //CASHBOT_DEV_FLOW_BOT_HPP
