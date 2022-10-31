//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef CASHBOT_DEV_FLOW_BOT_HPP
#define CASHBOT_DEV_FLOW_BOT_HPP

#include <tgbot/tgbot.h>


#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "db.hpp"


class FlowBot {
public:
    FlowBot();

    ~FlowBot();

    void Start();

    void Stop();

    void getInfo();

    [[nodiscard]]  TgBot::Bot *get_botPtr();

    Expense *get_expensePtr();

    EnvKeeper *get_envKeeper();

private:
    bool _is_running;
    EnvKeeper _env_keeper;
    TgBot::Bot *_bot;
    Expense *_expense;


    void _initHandlers();

    int get_last_message_id();

    void _set_bot_commands();


};


#endif //CASHBOT_DEV_FLOW_BOT_HPP
