//
// Created by Dmitry Morozov on 24/9/22.
//

#ifndef CASHBOT_DEV_FLOW_BOT_HPP
#define CASHBOT_DEV_FLOW_BOT_HPP

#include "db.hpp"
#include "Env_Keeper.hpp"
#include "Expense.hpp"

#include <tgbot/tgbot.h>


/**
 * @brief Bot class
 * @details Class that contains all bot logic
 */
class FlowBot {

public:
    /**
     * @brief Constructor
     */
    FlowBot();

    /**
     * @brief Destructor
     */
    ~FlowBot();


    /**
     * @brief Bot initialization
     * @details Initializes bot and EventLoop
     */
    void Start();

    /**
     * @brief Bot shutdown
     * @details Stops bot and EventLoop
     */
    void Stop();


    /**
     * @brief Show bot parameters
     */
    void getInfo();

    /**
     * @brief get a pointer to the bot
     * @return pointer to the bot
     */
    [[nodiscard]]  TgBot::Bot *get_botPtr();

    /**
     * @brief get a pointer to Expense object
     * @return  pointer to Expense object
     */
    Expense *get_expensePtr();

    /**
     * @brief get a pointer to EnvKeeper object
     * @return pointer to EnvKeeper object
     */
    EnvKeeper *get_envKeeper();

private:
    bool _is_running; ///< Bot running flag
    EnvKeeper _env_keeper; ///< Environment variables keeper
    TgBot::Bot *_bot; ///< Bot object
    Expense *_expense; ///< Expense object


    /**
     * @brief Both handlers initialization
     */
    void _initHandlers();

    /**
     * @brief Returns last message id
     * @returns last message id
     */
    int get_last_message_id();

    /**
     * @brief Sets all bot commands
     */
    void _set_bot_commands();

};


#endif //CASHBOT_DEV_FLOW_BOT_HPP
