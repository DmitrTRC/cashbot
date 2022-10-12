//
// Created by Dmitry Morozov on 12/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
#define CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP

#include <exception>

class WrongMsgException : public std::exception {
private:
    char *message;

public:
    explicit WrongMsgException(char *msg) : message(msg) {}

    [[nodiscard]] const char *what() const noexcept override {

        return message;
    }
};


#endif //CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
