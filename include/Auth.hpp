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


//int add(int a, int b)
//{
//    return a+b;
//}
//
//void testing()
//{
//    Functor<int (*)(int, int)> f(add);
//    cout << f()(2,3);
//}


#endif //CASHBOT_DEV_AUTH_HPP
