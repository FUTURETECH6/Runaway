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
#include <random>
#include <screen.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    using namespace std;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_x(0, SCREEN_X);  // distribution in range [x, y]
    std::uniform_int_distribution<std::mt19937::result_type> dist_y(0, SCREEN_Y);  // distribution in range [x, y]
    std::uniform_int_distribution<std::mt19937::result_type> dist_speed(1, 1);     // distribution in range [x, y]

    Screen screen;
    std::vector<Enemy *> enemies;
    auto myPlane = Plane(SCREEN_X, SCREEN_Y / 2);
    for (int i = 0; i < 3; i++) {
        enemies.push_back(new Enemy(dist_x(rng), dist_y(rng), dist_speed(rng)));
    }
    for (;;) {
        screen.flushScreen(myPlane, enemies);
        myPlane.loc.setX((SCREEN_X + myPlane.loc.getX() - 1) % SCREEN_X);
        for (auto &i : enemies) {
            if (i->loc.getX() > SCREEN_X) {
                i->loc.setX(0);
                i->loc.setY(dist_y(rng));
                i->setSpeed(dist_speed(rng));
            } else
                i->loc.setX(i->loc.getX() + i->getSpeed());
        }

        usleep(500 * 1e3);
    }
    return 0;
}
