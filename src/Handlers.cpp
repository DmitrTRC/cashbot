//
// Created by Dmitry Morozov on 31/10/22.
//
#include "Auth.hpp"
#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Helper.hpp"


void send_wrong_auth_message(FlowBot *botPtr, const long long &user_id) {

    botPtr->get_botPtr()->getApi().sendMessage(user_id, "You are not authenticated");
}


void handleHelpCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {


    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                   Helper::helpMessage());
    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleExpensesCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        auto expenses = botPtr->get_expensePtr()->getAllExpenses();
        std::string res_message = "Last expenses:\n";

        for (auto &expense: expenses) {
            res_message += expense.created + " " + std::to_string(expense.amount) + " " + expense.raw_text + "\n";
        }

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, res_message);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleStopCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {


    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        if (botPtr->get_envKeeper()->get_last_stop_id() >= message->messageId) {
            botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                       "The bot is already stopped");
        } else {
            botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                       "The bot is stopped");
            botPtr->Stop();
        }

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleAnyMessage(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        auto expenses = botPtr->get_expensePtr()->getAllExpenses();

        std::cout << "User : " << message->from->id << " wrote " << message->text
                  << std::endl;

//Exit if the message is a command
        if (std::any_of(Helper::_bot_commands.begin(), Helper::_bot_commands.end(), [&message](
                std::pair<std::string_view, std::string_view> command) {
            return message->text.find(command.first) != std::string::npos;
        })) {
            return;
        }

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                   "Your message is: " + message->text); // Debug only!


        DB::DBExpense *expense;
        try {
            expense = new DB::DBExpense(botPtr->get_expensePtr()->addExpense(message->text, message->from->id));
        } catch (std::exception &e) { //TODO: Add custom exception
            botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                       "Wrong format");
            return;
        }

        std::string message_text = "Added expenses " + std::to_string(expense->amount) + " for " +
                                   expense->category_codename + "\n\n" + botPtr->get_expensePtr()->get_today_stat();
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, message_text);
        delete expense;

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleCategoriesCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        auto categories = botPtr->get_expensePtr()->getCategoriesPtr()->getAllCategories();
        std::string res_message = "Categories:\n";

        for (auto &category: categories) {
            res_message += category.codename + " " + category.name + "\n";
        }

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, res_message);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleTodayCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        auto today_stat = botPtr->get_expensePtr()->get_today_stat();
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, today_stat);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}

void handleMonthCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        auto month_stat = botPtr->get_expensePtr()->get_month_stat();
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, month_stat);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }
}