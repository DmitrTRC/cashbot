//
// Created by Dmitry Morozov on 24/9/22.
//

#include <tgbot/tgbot.h>

#include "Flow_Bot.hpp"

#include <iostream>
#include <chrono>


FlowBot::FlowBot () {
    _is_running = false;

    if (_env_keeper.is_Empty ()) {
        std::cout << "The environment is empty" << std::endl;
        exit (EXIT_FAILURE);
    }

    _bot = new TgBot::Bot (_env_keeper.get_Token ());

    _initHandlers ();
}

FlowBot::~FlowBot () { delete _bot; }

void FlowBot::Start () {
    _is_running = true;
    getInfo ();
    try {
        TgBot::TgLongPoll longPoll (*_bot);

        while (true) {

            std::cout << "Long poll started" << std::endl;


            longPoll.start ();

            if (!_is_running) {
                throw TgBot::TgException ("The bot is stopped by the user");
            }
        }
    } catch (TgBot::TgException &e) {
        std::cout << "Bot stopped report : " << e.what () << std::endl;
        auto last_id = get_last_message_id ();
        _env_keeper.set_last_stop_id (last_id);
    }
}

void FlowBot::getInfo () const {
    auto me = _bot->getApi ().getMe ();

    std::cout << "Bot username: \n" << me->username << std::endl;
    std::cout << "Bot id: " << me->id << std::endl;
    std::cout << "Bot first name: " << me->firstName << std::endl;
    std::cout << "Bot last name: " << me->lastName << std::endl;
    std::cout << "Bot user name: " << me->username << std::endl;
    std::cout << "Bot language code: " << me->languageCode << std::endl;
}

void FlowBot::Stop () {
    _is_running = false;
}

/// Check Auth
void FlowBot::_initHandlers () {
    _bot->getEvents ().onCommand ("start", [ & ] (TgBot::Message::Ptr message) {
        if (isAuthenticated (message->from->id)) {
            _bot->getApi ().sendMessage (message->chat->id, "Hello, " + message->from->firstName);
        } else {
            send_wrong_auth_message (message->from->id);
        }

    });

    _bot->getEvents ().onCommand ("stop", [ & ] (TgBot::Message::Ptr message) {

        if (_env_keeper.get_last_stop_id () >= message->messageId) {
            _bot->getApi ().sendMessage (message->chat->id,
                                         "The bot is already stopped");
        } else {
            _bot->getApi ().sendMessage (message->chat->id,
                                         "The bot is stopped");
            Stop ();
        }

    });

    _bot->getEvents ().onAnyMessage ([ & ] (TgBot::Message::Ptr message) {
        std::cout << "User : " << message->from->id << " wrote " << message->text
                  << std::endl;
        if (StringTools::startsWith (message->text, "/start")) {
            return;
        }
        _bot->getApi ().sendMessage (message->chat->id,
                                     "Your message is: " + message->text);
    });
}

// GetUpdates and return last message id
int FlowBot::get_last_message_id () {
    auto updates = _bot->getApi ().getUpdates ();
    return updates.back ()->message->messageId;
}

bool FlowBot::isAuthenticated (const long long &user_id) {

    return _env_keeper.is_valid_user_id (user_id);

}

void FlowBot::send_wrong_auth_message (const long long &user_id) {
    _bot->getApi ().sendMessage (user_id, "You are not authenticated");
}



