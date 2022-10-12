//
// Created by Dmitry Morozov on 12/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
#define CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <utility>

class WrongMsgException : public std::exception {
private:
    std::string message;

public:
    explicit WrongMsgException(std::string msg) : message(std::move(msg)) {}

    [[nodiscard]] const char *what() const noexcept override {

        return message.c_str();
    }
};


#endif //CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
