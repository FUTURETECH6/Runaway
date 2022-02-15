/**
 * @file main.cpp
 * @author xiyao (futuretech6@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 * @ref [Clear the screen - C++ Articles](https://www.cplusplus.com/articles/4z18T05o/)
 *
 */
#include <iostream>
#include <screen.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    using namespace std;
    Screen screen;
    for (int i = 0; i < 10; i++) {
        screen.addLoc(Loc(30 - i, 10, Type::plane));
        screen.flush();
        usleep(300 * 1e3);
    }
    screen.clear();
    return 0;
}
