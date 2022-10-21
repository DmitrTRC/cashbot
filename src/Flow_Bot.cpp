//
// Created by Dmitry Morozov on 24/9/22.
//

#include <tgbot/tgbot.h>

#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Helper.hpp"

#include <iostream>
#include <memory>


FlowBot::FlowBot() {

    _is_running = false;

    if (_env_keeper.is_Empty()) {
        std::cout << "The environment is empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    _bot = new TgBot::Bot(_env_keeper.get_Token());
    _set_bot_commands();
    _initHandlers();

    _expense = new Expense();

}

FlowBot::~FlowBot() {

    delete _bot;
    delete _expense;
}

void FlowBot::Start() {

    _is_running = true;
    getInfo();
    try {
        TgBot::TgLongPoll longPoll(*_bot);

        while (true) {

            std::cout << "Long poll started" << std::endl;
            //Debug Only!
//            throw TgBot::TgException("The bot is stopped by tests");

            longPoll.start();

            if (!_is_running) {
                throw TgBot::TgException("The bot is stopped by the user");
            }
        }
    } catch (TgBot::TgException &e) {
        std::cout << "Bot stopped report : " << e.what() << std::endl;
        auto last_id = get_last_message_id();
        _env_keeper.set_last_stop_id(last_id);
    }
}

void FlowBot::getInfo() const {

    auto me = _bot->getApi().getMe();

    std::cout << "Bot username: \n" << me->username << std::endl;
    std::cout << "Bot id: " << me->id << std::endl;
    std::cout << "Bot first name: " << me->firstName << std::endl;
    std::cout << "Bot last name: " << me->lastName << std::endl;
    std::cout << "Bot user name: " << me->username << std::endl;
    std::cout << "Bot language code: " << me->languageCode << std::endl;
}

void FlowBot::Stop() {

    _is_running = false;
}

/// Check Auth
void FlowBot::_initHandlers() {

    _bot->getEvents().onCommand(Helper::onHelp, [&](const TgBot::Message::Ptr &message) {
        if (isAuthenticated(message->from->id)) {
            _bot->getApi().sendMessage(message->chat->id,
                                       Helper::helpMessage);
        } else {
            send_wrong_auth_message(message->from->id);
        }
    });
    _bot->getEvents().onCommand("expenses", [&](const TgBot::Message::Ptr &message) {

        if (isAuthenticated(message->from->id)) {
            auto expenses = _expense->getAllExpenses();
            std::string res_message = "Last expenses:\n";

            for (auto &expense: expenses) {
                res_message += expense.created + " " + std::to_string(expense.amount) + " " + expense.raw_text + "\n";
            }

            _bot->getApi().sendMessage(message->chat->id, res_message);

        } else {
            send_wrong_auth_message(message->from->id);
        }
    });
    _bot->getEvents().onCommand("stop", [&](const TgBot::Message::Ptr &message) {

        if (isAuthenticated(message->from->id)) {
            if (_env_keeper.get_last_stop_id() >= message->messageId) {
                _bot->getApi().sendMessage(message->chat->id,
                                           "The bot is already stopped");
            } else {
                _bot->getApi().sendMessage(message->chat->id,
                                           "The bot is stopped");
                Stop();
            }

        } else {
            send_wrong_auth_message(message->from->id);
        }
    });


    _bot->getEvents().onAnyMessage([&](const TgBot::Message::Ptr &message) {

        if (isAuthenticated(message->from->id)) {
            std::cout << "User : " << message->from->id << " wrote " << message->text
                      << std::endl;

// Check if the message is a command
            if (std::any_of(Helper::_bot_commands.begin(), Helper::_bot_commands.end(), [&message](
                    std::pair<std::string_view, std::string_view> command) {
                return message->text.find(command.first) != std::string::npos;
            })) {
                return;
            }

            _bot->getApi().sendMessage(message->chat->id,
                                       "Your message is: " + message->text); // Debug only!

            //Main part for add expense
            // DB::Expense* expense;
            DB::DBExpense *expense;
            try {
                expense = new DB::DBExpense(_expense->addExpense(message->text, message->from->id));
            } catch (std::exception &e) { //TODO: Add custom exception
                _bot->getApi().sendMessage(message->chat->id,
                                           "Wrong format");
                return;
            }

            std::string message_text = "Added expenses " + std::to_string(expense->amount) + " for " +
                                       expense->category_codename + "\n\n" + _expense->get_today_stat();
            _bot->getApi().sendMessage(message->chat->id, message_text);
            delete expense;

        } else {
            send_wrong_auth_message(message->from->id);
        }
    });
}

// GetUpdates and return last message id
int FlowBot::get_last_message_id() {

    auto updates = _bot->getApi().getUpdates();
    return updates.back()->message->messageId;
}

bool FlowBot::isAuthenticated(const long long &user_id) {

    auto ids = _env_keeper.get_user_ids();

    return ids.find(user_id) != ids.end();
}

void FlowBot::send_wrong_auth_message(const long long &user_id) {

    _bot->getApi().sendMessage(user_id, "You are not authenticated");
}

void FlowBot::_set_bot_commands() {

    std::vector<TgBot::BotCommand::Ptr> commands;

    for (auto &[newCommand, commandDescription]: Helper::_bot_commands) {
        auto bot_command = std::make_shared<TgBot::BotCommand>();
        bot_command->command = newCommand;
        bot_command->description = commandDescription;
        commands.push_back(bot_command);
    }


    _bot->getApi().setMyCommands(commands);
}



