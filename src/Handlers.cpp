//
// Created by Dmitry Morozov on 31/10/22.
//
#include "Auth.hpp"
#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Helper.hpp"
#include "Handlers.hpp"

void send_wrong_auth_message(const long long &user_id) {

    FlowBot::get_botPtr()->getApi().sendMessage(user_id, "You are not authenticated");
}


void handleHelpCommand(const TgBot::Message::Ptr &message) {

    auto botPtr = FlowBot::get_botPtr();

    if (isAuthenticated(message->from->id)) {

        botPtr->getApi().sendMessage(message->chat->id,
                                     Helper::helpMessage());
    } else {
        send_wrong_auth_message(message->from->id);
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
        send_wrong_auth_message(message->from->id);
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
        send_wrong_auth_message(message->from->id);
    }
}

void handleAnyMessage(const TgBot::Message::Ptr &message) {

    auto botPtr = FlowBot::get_botPtr();

    if (isAuthenticated(message->from->id)) {
        auto expenses = FlowBot::get_expensePtr()->getAllExpenses();

        std::cout << "User : " << message->from->id << " wrote " << message->text
                  << std::endl;

//Exit if the message is a command
        if (std::any_of(Helper::_bot_commands.begin(), Helper::_bot_commands.end(), [&message](
                std::pair<std::string_view, std::string_view> command) {
            return message->text.find(command.first) != std::string::npos;
        })) {
            return;
        }

        botPtr->getApi().sendMessage(message->chat->id,
                                     "Your message is: " + message->text); // Debug only!


        DB::DBExpense *expense;
        try {
            expense = new DB::DBExpense(FlowBot::get_expensePtr()->addExpense(message->text, message->from->id));
        } catch (std::exception &e) { //TODO: Add custom exception
            botPtr->getApi().sendMessage(message->chat->id,
                                         "Wrong format");
            return;
        }

        std::string message_text = "Added expenses " + std::to_string(expense->amount) + " for " +
                                   expense->category_codename + "\n\n" + FlowBot::get_expensePtr()->get_today_stat();
        botPtr->getApi().sendMessage(message->chat->id, message_text);
        delete expense;

    } else {
        send_wrong_auth_message(message->from->id);
    }
}


