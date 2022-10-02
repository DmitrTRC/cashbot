//
// Created by Dmitry Morozov on 30/9/22.
//

#ifndef CASHBOT_DEV_AUTH_HPP
#define CASHBOT_DEV_AUTH_HPP


template<typename T>
class Auth {
    T f;
public:
    explicit Auth (T t) {
        f = t;
    }

    T &operator() () {
        return f;
    }
};

///Use class Auth to wrap function
void ifAuth () {
    Auth auth = Auth ([] () {
        return 1;
    });
    auth ();
}

#endif //CASHBOT_DEV_AUTH_HPP
