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
#include <cmath>
#include <iostream>
#include <random>
#include <screen.h>
#include <unistd.h>

std::vector<std::vector<char>> Screen::map = std::vector<std::vector<char>>(SCREEN_Y, std::vector<char>(SCREEN_X, ' '));

int genSpeed(std::uniform_int_distribution<std::mt19937::result_type> &dist_speed, std::mt19937 &rng) {
    int speed = 0;
    while (!speed)
        speed = dist_speed(rng);
    return speed;
}

int main(int argc, char const *argv[]) {
    using namespace std;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_x(0, SCREEN_X - 1);  // distribution in range [x, y]
    std::uniform_int_distribution<std::mt19937::result_type> dist_y(0, SCREEN_Y - 1);  // distribution in range [x, y]
    std::uniform_int_distribution<std::mt19937::result_type> dist_speed_b(-4, 4);      // distribution in range [x, y]
    std::uniform_int_distribution<std::mt19937::result_type> dist_speed_e(1, 2);       // distribution in range [x, y]

    Screen screen;
    auto myPlane = Plane(SCREEN_X, SCREEN_Y / 2);
    std::vector<Enemy> enemies;
    std::list<Bullet> bullets;
    for (int i = 0; i < 5; i++) {
        enemies.push_back(Enemy(dist_x(rng), dist_y(rng), genSpeed(dist_speed_e, rng)));
    }
    for (;;) {

        bullets.push_back(Bullet(dist_x(rng), dist_y(rng), genSpeed(dist_speed_b, rng)));
        // bullets.push_back(Bullet(dist_x(rng), dist_y(rng), genSpeed(dist_speed, rng)));

        screen.flushScreen(myPlane, enemies, bullets);

        myPlane.getLoc().setX((SCREEN_X + myPlane.getLoc().getX() - myPlane.getSpeed()) % SCREEN_X);

        for (auto &e : enemies) {
            if (e.getLoc().getX() >= SCREEN_X) {
                e.getLoc().setX(0);
                e.getLoc().setY(dist_y(rng));
                e.setSpeed(genSpeed(dist_speed_e, rng));
            } else
                e.getLoc().setX(e.getLoc().getX() + e.getSpeed());
        }

        for (auto b = bullets.begin(); b != bullets.end();) {
            b->getLoc().setX(b->getLoc().getX() + b->getSpeed());
            if (b->getLoc().getX() >= SCREEN_X || b->getLoc().getX() < 0) {
                bullets.erase(b++);
            } else
                b++;
        }

        usleep(100 * 1e3);
    }
    return 0;
}
