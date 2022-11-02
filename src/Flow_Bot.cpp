//
// Created by Dmitry Morozov on 24/9/22.
//

#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Handlers.hpp"
#include "Helper.hpp"

#include <tgbot/tgbot.h>

#include <iostream>
#include <memory>

/**
 * @brief Constructor
 */
FlowBot::FlowBot() {

    _is_running = false; /// bot is not running

    if (_env_keeper.is_Empty()) {
        std::cout << "The environment is empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    _bot = new TgBot::Bot(_env_keeper.get_Token());

    _set_bot_commands();
    _initHandlers();

    _expense = new Expense();
}

/**
 * @brief Destructor
 */
FlowBot::~FlowBot() {

    delete _bot;
    delete _expense;
}

/**
 * @brief It starts the bot
 */
void FlowBot::Start() {

    _is_running = true; /// bot is running

    getInfo();

    try {
        TgBot::TgLongPoll longPoll(*_bot);

        while (true) {

            std::cout << "Long poll started" << std::endl;

            longPoll.start();

            if (!_is_running) {
                throw TgBot::TgException("The bot is stopped by the user"); /// the bot is stopped by the user
            }

        }

    } catch (TgBot::TgException &e) {

        std::cout << "Bot stopped report : " << e.what() << std::endl;
        auto last_id = get_last_message_id();
        _env_keeper.set_last_stop_id(last_id);

    }
}

/**
 * @brief Showing the bot info
 */
void FlowBot::getInfo() {

    auto me = _bot->getApi().getMe();

    std::cout << "Bot username: \n" << me->username << std::endl;
    std::cout << "Bot id: " << me->id << std::endl;
    std::cout << "Bot first name: " << me->firstName << std::endl;
    std::cout << "Bot last name: " << me->lastName << std::endl;
    std::cout << "Bot user name: " << me->username << std::endl;
    std::cout << "Bot language code: " << me->languageCode << std::endl;
}

/**
 * @brief It stops the bot
 */
void FlowBot::Stop() {

    _is_running = false; /// bot is not running
}


/**
 * It initializes the bot's event handlers
 *
 * @brief It initializes the bot's event handlers
 * @details It initializes the bot's event handlers by lambda functions which call the corresponding handlers
 */
void FlowBot::_initHandlers() {

    _bot->getEvents().onCommand(Helper::onHelp, [&](const TgBot::Message::Ptr &message) {
        handleHelpCommand(this, message);
    });
    _bot->getEvents().onCommand("expenses", [&](const TgBot::Message::Ptr &message) {
        handleExpensesCommand(this, message);
    });
    _bot->getEvents().onCommand("stop", [&](const TgBot::Message::Ptr &message) {
        handleStopCommand(this, message);
    });

    _bot->getEvents().onCommand("categories", [&](const TgBot::Message::Ptr &message) {
        handleCategoriesCommand(this, message);
    });

    _bot->getEvents().onCommand("today", [&](const TgBot::Message::Ptr &message) {
        handleTodayCommand(this, message);
    });

    _bot->getEvents().onCommand("month", [&](const TgBot::Message::Ptr &message) {
        handleMonthCommand(this, message);
    });

    _bot->getEvents().onAnyMessage([&](const TgBot::Message::Ptr &message) {
        handleAnyMessage(this, message);
    });

}

/**
 * It gets the updates from the bot and returns the last message id
 *
 * @return The last message id
 */
int FlowBot::get_last_message_id() {

    auto updates = _bot->getApi().getUpdates();

    return updates.back()->message->messageId;
}


/**
 * It sets the commands that the bot will respond to
 */
void FlowBot::_set_bot_commands() {

    std::vector<TgBot::BotCommand::Ptr> commands; /// vector of commands pointers

    for (auto &[newCommand, commandDescription]: Helper::_bot_commands) {

        auto bot_command = std::make_shared<TgBot::BotCommand>();

        bot_command->command = newCommand;
        bot_command->description = commandDescription;
        commands.push_back(bot_command);
    }

    _bot->getApi().setMyCommands(commands);

}


/**
 * @brief This function returns a pointer to the bot object
 * @returns A pointer to the bot object.
 */
TgBot::Bot *FlowBot::get_botPtr() {

    return _bot;
}

/**
 * @brief This function returns a pointer to the expense object
 * @returns A pointer to the expense object.
 */
Expense *FlowBot::get_expensePtr() {

    return _expense;
}

/**
 * @brief This function returns a pointer to the environment keeper object
 * @returns A pointer to the environment keeper object.
 */
EnvKeeper *FlowBot::get_envKeeper() {

    return &_env_keeper;
}



