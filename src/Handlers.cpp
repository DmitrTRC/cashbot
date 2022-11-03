//
// Created by Dmitry Morozov on 31/10/22.
//
#include "Auth.hpp"
#include "Env_Keeper.hpp"
#include "Expense.hpp"
#include "Flow_Bot.hpp"
#include "Helper.hpp"

#include <tgbot/tgbot.h>

/**
 * It sends a message to the user saying that they are not authenticated
 *
 * @param botPtr A pointer to the bot object.
 * @param user_id The user's id
 */
void send_wrong_auth_message(FlowBot *botPtr, const long long &user_id) {

    botPtr->get_botPtr()->getApi().sendMessage(user_id, "You are not authenticated");
}


/**
 * Handler for the [/start, /help] commands
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
void handleHelpCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, Helper::helpMessage(), false, 0, nullptr,
                                                   "HTML");
    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }

}

/**
 * Handler for the [/expenses] command
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
void handleExpensesCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {

        auto expenses = botPtr->get_expensePtr()->getAllExpenses();

        std::string res_message = "Last expenses:\n";

        for (auto &expense: expenses) {
            res_message +=
                    std::to_string(expense.id) + " : " + expense.created + " " + std::to_string(expense.amount) + " " +
                    expense.raw_text + "\n";
        }

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, res_message);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }

}

/**
 * Handler for the [/stop] command
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
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

/**
 * Handler for [Add expense, /del] commands
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
void handleAnyMessage(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {

        auto expenses = botPtr->get_expensePtr()->getAllExpenses();

        std::cout << "User : " << message->from->id << " wrote " << message->text << std::endl;


        if (std::any_of(Helper::_bot_commands.begin(), Helper::_bot_commands.end(),
                        [&message]( ///Check if the message is a command
                                std::pair<std::string_view, std::string_view> command) {
                            return message->text.find(command.first) != std::string::npos;
                        })) {

            return;
        }

        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                   "Your message is: " + message->text); // Debug only!

        if (StringTools::startsWith(message->text, "/del")) {

            auto id = std::stoll(message->text.substr(5));
            botPtr->get_expensePtr()->deleteExpense(id);
            botPtr->get_botPtr()->getApi().sendMessage(message->chat->id,
                                                       "Expense with id " + std::to_string(id) + " deleted");
        } else {

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
        }

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }

}

/**
 * Handler for the [/categories] command
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
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

/**
 * Handler for the [/Today] command
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
void handleTodayCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {
        std::cout << "TODAY Auth OK" << std::endl;
        auto today_stat = botPtr->get_expensePtr()->get_today_stat();
        //std::string today_stat = "TEST STAT";
        std::cout << "TODAY Stat OK" << std::endl;
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, today_stat);
        std::cout << "TODAY Send OK" << std::endl;

    } else {
        std::cout << "TODAY Auth FAIL" << std::endl;
        send_wrong_auth_message(botPtr, message->from->id);
    }

}

/**
 * Handler for the [/month] command
 *
 * @param botPtr A pointer to the bot object.
 * @param message The message object that was sent to the bot.
 */
void handleMonthCommand(FlowBot *botPtr, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(botPtr->get_envKeeper(), message->from->id)) {

        auto month_stat = botPtr->get_expensePtr()->get_month_stat();
        botPtr->get_botPtr()->getApi().sendMessage(message->chat->id, month_stat);

    } else {
        send_wrong_auth_message(botPtr, message->from->id);
    }

}