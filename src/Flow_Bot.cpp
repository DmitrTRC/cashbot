//
// Created by Dmitry Morozov on 24/9/22.
//

#include <tgbot/tgbot.h>

#include "Flow_Bot.hpp"


#include <iostream>


FlowBot::FlowBot () {
    _is_running = false;

    if (_env_keeper.is_Empty ()) {
        std::cout << "The environment is empty" << std::endl;
        exit (EXIT_FAILURE);
    }

    _bot = new TgBot::Bot (_env_keeper.get_Token ());

    _initHandlers ();

}

FlowBot::~FlowBot () {
    delete _bot;
}

void FlowBot::Start () {
    _is_running = true;
    getInfo ();
    try {
        TgBot::TgLongPoll longPoll (*_bot);
        while (_is_running) {
            std::cout << "Long poll started" << std::endl;
            longPoll.start ();
        }
    } catch (TgBot::TgException &e) {
        std::cout << "Bot error: " << e.what () << std::endl;
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

void FlowBot::_initHandlers () {
    _bot->getEvents ().onCommand ("start", [ & ] (TgBot::Message::Ptr message) {
        _bot->getApi ().sendMessage (message->chat->id, "Hi!");
    });

    _bot->getEvents ().onCommand ("emergency", [ & ] (TgBot::Message::Ptr message) {
        _bot->getApi ().sendMessage (message->chat->id, "Bye!");
        Stop ();
    });

    _bot->getEvents ().onAnyMessage ([ & ] (TgBot::Message::Ptr message) {
        std::cout << "User : " << message->from->id << " wrote " << message->text << std::endl;
        if (StringTools::startsWith (message->text, "/start")) {
            return;
        }
        _bot->getApi ().sendMessage (message->chat->id, "Your message is: " + message->text);
    });

}
