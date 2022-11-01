//
// Created by Dmitry Morozov on 31/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
#define CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP

#include <tgbot/tgbot.h>
#include "Flow_Bot.hpp"

void handleHelpCommand(FlowBot *, const TgBot::Message::Ptr &message);

void handleExpensesCommand(FlowBot *, const TgBot::Message::Ptr &);

void handleStopCommand(FlowBot *, const TgBot::Message::Ptr &);

void handleAnyMessage(FlowBot *, const TgBot::Message::Ptr &);

void handleCategoriesCommand(FlowBot *, const TgBot::Message::Ptr &);

void send_wrong_auth_message(FlowBot *, const long long &);


#endif //CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
