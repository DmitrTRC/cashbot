///3-d Party libs

#include <tgbot/tgbot.h>

#include "load_api_keys.hpp"


#include <stdio.h>


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
        printf ("User wrote %s\n", message->text.c_str ());
        if (StringTools::startsWith (message->text, "/start")) {
            return;
        }
        bot.getApi ().sendMessage (message->chat->id, "Your message is: " + message->text);
    });
    try {
        printf ("Bot username: %s\n", bot.getApi ().getMe ()->username.c_str ());
        TgBot::TgLongPoll longPoll (bot);
        while (true) {
            printf ("Long poll started\n");
            longPoll.start ();
        }
    } catch (TgBot::TgException &e) {
        printf ("error: %s\n", e.what ());
    }
    return 0;
}