//
// Created by Dmitry Morozov on 31/10/22.
//
#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Handlers.hpp"

void send_wrong_auth_message(TgBot::Bot *_bot, const long long &user_id) {

    _bot->getApi().sendMessage(user_id, "You are not authenticated");
}


void handleHelpCommand(const TgBot::Message::Ptr &message) {

    auto botPtr = FlowBot::get_botPtr();

    if (isAuthenticated(message->from->id)) {

        botPtr->getApi().sendMessage(message->chat->id,
                                     Helper::helpMessage());
    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleExpensesCommand(const TgBot::Message::Ptr &message) {

    auto botPtr = FlowBot::get_botPtr();
    if (isAuthenticated(message->from->id)) {
        auto expenses = FlowBot::get_expensePtr()->getAllExpenses();
        std::string res_message = "Last expenses:\n";

        for (auto &expense: expenses) {
            res_message += expense.created + " " + std::to_string(expense.amount) + " " + expense.raw_text + "\n";
        }

        botPtr->getApi().sendMessage(message->chat->id, res_message);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleStopCommand(const TgBot::Message::Ptr &message) {

    auto botPtr = FlowBot::get_botPtr();

    if (isAuthenticated(message->from->id)) {
        if (EnvKeeper::get_last_stop_id() >= message->messageId) {
            botPtr->getApi().sendMessage(message->chat->id,
                                         "The bot is already stopped");
        } else {
            botPtr->getApi().sendMessage(message->chat->id,
                                         "The bot is stopped");
            FlowBot::Stop();
        }

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}


