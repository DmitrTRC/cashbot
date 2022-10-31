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

    static void Stop();

    static void getInfo();

    [[nodiscard]] static TgBot::Bot *get_botPtr();

    static Expense *get_expensePtr();

private:
    static bool _is_running;
    EnvKeeper _env_keeper;
    static TgBot::Bot *_bot;
    static Expense *_expense;


    void _initHandlers();

    int get_last_message_id();

    void _set_bot_commands();
};


#endif //CASHBOT_DEV_FLOW_BOT_HPP
