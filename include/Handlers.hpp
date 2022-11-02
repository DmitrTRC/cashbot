//
// Created by Dmitry Morozov on 31/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
#define CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP

#include "Flow_Bot.hpp"

#include <tgbot/tgbot.h>

/**
 * @brief Handler for the [/start, /help] commands
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleHelpCommand(FlowBot *, const TgBot::Message::Ptr &message);

/**
 * @brief Handler for the [/expenses] command
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleExpensesCommand(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Handler for the [/stop] command
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleStopCommand(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Handler for [Add expense, /del] commands
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleAnyMessage(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Handler for the [/categories] command
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleCategoriesCommand(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Handler for the [/today] command
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleTodayCommand(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Handler for the [/month] command
 *
 * @param bot Pointer to the bot
 * @param message Pointer to the message
 */
void handleMonthCommand(FlowBot *, const TgBot::Message::Ptr &);

/**
 * @brief Sending the message if wrong auth
 *
 * @param bot Pointer to the bot
 * @param user_id User id
 */
void send_wrong_auth_message(FlowBot *, const long long &);


#endif //CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
