//
// Created by Dmitry Morozov on 31/10/22.
//

#include "Handlers.hpp"

void handleHelpCommand(TgBot::Bot *_bot, const TgBot::Message::Ptr &message) {

    if (isAuthenticated(message->from->id)) {
        _bot->getApi().sendMessage(message->chat->id,
                                   Helper::helpMessage());
    } else {
        send_wrong_auth_message(_bot, message->from->id);
    }
}


void send_wrong_auth_message(TgBot::Bot *_bot, const long long &user_id) {

    _bot->getApi().sendMessage(user_id, "You are not authenticated");
}
