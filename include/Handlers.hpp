//
// Created by Dmitry Morozov on 31/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
#define CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP

#include "Auth.hpp"
#include "Helper.hpp"
#include <tgbot/tgbot.h>

void handleHelpCommand(const TgBot::Message::Ptr &);

void handleExpensesCommand(const TgBot::Message::Ptr &);

void handleStopCommand(const TgBot::Message::Ptr &);

void send_wrong_auth_message(TgBot::Bot *, const long long &);


#endif //CASHBOT_FEATURE_EXPENSE_HANDLERS_HPP
