//
// Created by Dmitry Morozov on 31/10/22.
//
#include "Auth.hpp"
#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Helper.hpp"


void send_wrong_auth_message(FlowBot *botPtr, const long long &user_id) {

    botPtr->_bot->getApi().sendMessage(user_id, "You are not authenticated");
}


void handleHelpCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {


    if (isAuthenticated(message->from->id)) {

        botPtr->_bot->getApi().sendMessage(message->chat->id,
                                           Helper::helpMessage());
    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleExpensesCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(message->from->id)) {
        auto expenses = botPtr->_expense->getAllExpenses();
        std::string res_message = "Last expenses:\n";

        for (auto &expense: expenses) {
            res_message += expense.created + " " + std::to_string(expense.amount) + " " + expense.raw_text + "\n";
        }

        botPtr->_bot->getApi().sendMessage(message->chat->id, res_message);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleStopCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {


    if (isAuthenticated(message->from->id)) {
        if (botPtr->_env_keeper.get_last_stop_id() >= message->messageId) {
            botPtr->_bot->getApi().sendMessage(message->chat->id,
                                               "The bot is already stopped");
        } else {
            botPtr->_bot->getApi().sendMessage(message->chat->id,
                                               "The bot is stopped");
            botPtr->Stop();
        }

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleAnyMessage(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(message->from->id)) {
        auto expenses = botPtr->_expense->getAllExpenses();

        std::cout << "User : " << message->from->id << " wrote " << message->text
                  << std::endl;

//Exit if the message is a command
        if (std::any_of(Helper::_bot_commands.begin(), Helper::_bot_commands.end(), [&message](
                std::pair<std::string_view, std::string_view> command) {
            return message->text.find(command.first) != std::string::npos;
        })) {
            return;
        }

        botPtr->_bot->getApi().sendMessage(message->chat->id,
                                           "Your message is: " + message->text); // Debug only!


        DB::DBExpense *expense;
        try {
            expense = new DB::DBExpense(botPtr->_expense->addExpense(message->text, message->from->id));
        } catch (std::exception &e) { //TODO: Add custom exception
            botPtr->_bot->getApi().sendMessage(message->chat->id,
                                               "Wrong format");
            return;
        }

        std::string message_text = "Added expenses " + std::to_string(expense->amount) + " for " +
                                   expense->category_codename + "\n\n" + botPtr->_expense->get_today_stat();
        botPtr->_bot->getApi().sendMessage(message->chat->id, message_text);
        delete expense;

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

