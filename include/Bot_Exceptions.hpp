//
// Created by Dmitry Morozov on 12/10/22.
//

#ifndef CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
#define CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <utility>

/**
 * @brief Base class for all exceptions in the project
 */
class WrongMsgException : public std::exception {
private:
    std::string message;

public:
    /**
     * @brief Constructor
     * @param message Message that will be shown to the user
     */
    explicit WrongMsgException(std::string msg) : message(std::move(msg)) {}


    /**
     * @brief Get the message that will be shown to the user
     * @return Message that will be shown to the user as a C string
     */
    [[nodiscard]] const char *what() const noexcept override {

        return message.c_str();
    }
};


#endif //CASHBOT_FEATURE_EXPENSE_BOT_EXCEPTIONS_HPP
