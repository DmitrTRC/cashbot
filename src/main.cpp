///3-d Party libs

#include <tgbot/tgbot.h>

#include "load_api_keys.hpp"

#include <iostream>


int main () {
    Tokens api_keys;

    api_keys = load_environment ("bot.env");

    if (api_keys.empty ()) {
        std::cerr << "No API keys found" << std::endl;
        return 1;
    }

    TgBot::Bot bot (api_keys["TELEGRAM_API"]);

    bot.getEvents ().onCommand ("start", [ &bot ] (TgBot::Message::Ptr message) {
        bot.getApi ().sendMessage (message->chat->id, "Hi!");
    });
    bot.getEvents ().onAnyMessage ([ &bot ] (TgBot::Message::Ptr message) {
        std::cout << "User : " << message->from->id << " wrote " << message->text << std::endl;
        if (StringTools::startsWith (message->text, "/start")) {
            return;
        }
        bot.getApi ().sendMessage (message->chat->id, "Your message is: " + message->text);
    });

    try {
        auto me = bot.getApi ().getMe ();

        std::cout << "Bot username: \n" << me->username << std::endl;
        std::cout << "Bot id: " << me->id << std::endl;
        std::cout << "Bot first name: " << me->firstName << std::endl;
        std::cout << "Bot last name: " << me->lastName << std::endl;
        std::cout << "Bot user name: " << me->username << std::endl;
        std::cout << "Bot language code: " << me->languageCode << std::endl;

        TgBot::TgLongPoll longPoll (bot);
        while (true) {
            std::cout << "Long poll started" << std::endl;
            longPoll.start ();
        }
    } catch (TgBot::TgException &e) {
        std::cout << "Bot error: " << e.what () << std::endl;
    }
    return 0;
}