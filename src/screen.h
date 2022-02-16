#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <term.h>
#include <unistd.h>
#include <utility>
#include <vector>

const int SCREEN_X = 80;
const int SCREEN_Y = 20;

class Loc {
  protected:
    int x, y;

  public:
    Loc(int x, int y) : x(x), y(y) {}

    int getX() { return x; }
    int getY() { return y; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    std::pair<int, int> getLocPair() { return std::make_pair(x, y); }

    bool operator<(const Loc &other) const { return y < other.y || (y == other.y && x < other.x); }
    bool operator>(const Loc &other) const { return y > other.y || (y == other.y && x > other.x); }
    bool operator==(const Loc &other) const { return y == other.y && x == other.x; }
};

class Base {
  protected:
    Loc loc;

  public:
    Base(int x, int y) : loc(x, y) {}
    bool operator<(const Base &other) const { return loc < other.loc; }
    bool operator==(const Base &other) const { return loc == other.loc; }

    Loc &getLoc() { return loc; }

    virtual std::string &getPic()    = 0;
    virtual int getSpeed()           = 0;
    virtual void setSpeed(int speed) = 0;

    static bool cmp(Base *x, Base *y) { return x->loc < y->loc; }
};

class Plane : public Base {
  private:
    std::string pic = "-<-`";

  public:
    Plane(int x, int y) : Base(x, y) {}

    std::string &getPic() override { return pic; }
    int getSpeed() override { return 1; }
    void setSpeed(int speed) override { throw std::runtime_error("Plane speed cannot be set"); }
};

class Bullet : public Base {
  private:
    std::string pic = "+";
    int speed;

  public:
    Bullet(int x, int y, int speed = 1) : Base(x, y), speed(speed) {}

    std::string &getPic() override { return pic; }
    int getSpeed() override { return speed; }
    void setSpeed(int speed) override { this->speed = speed; }
};

class Enemy : public Base {
  private:
    std::string pic = "X";
    int speed;

  public:
    Enemy(int x, int y, int speed) : Base(x, y), speed(speed) {}

    std::string &getPic() override { return pic; }
    int getSpeed() override { return speed; }
    void setSpeed(int speed) override { this->speed = speed; }
};

class Screen {
  private:
    inline static void addToMap(Loc &loc, std::string str) {
        for (auto i = 0; i < str.size(); i++)
            map[loc.getY()][(loc.getX() + i) % SCREEN_X] = str[i];
        // copy(str.begin(), str.end(), &map[loc.getY()][(loc.getX()) % SCREEN_X]);
    }
    inline static void printMap() {
        for (auto &i : map) {
            for (auto &j : i)
                std::cout << j;
            std::cout << std::endl;
        }
    }
    // inline static void rmFromMap(Loc &loc, int len) {
    //     for (auto i = 0; i < len; i++)
    //         map[loc.getY()][(loc.getX() + i) % SCREEN_X] = ' ';
    // }
    static std::vector<std::vector<char>> map;

  public:
    Screen() {}

    static void clearScreen() {
        if (!cur_term) {
            int result;
            setupterm(NULL, STDOUT_FILENO, &result);
            if (result <= 0)
                return;
        }
        putp(tigetstr("clear"));

        // for (auto &i : map)
        //     for (auto &j : i)
        //         j = ' ';
    }

    static void flushScreen(Plane &myPlane, std::vector<Enemy> &enemies, std::list<Bullet> &bullets) {
        using namespace std;

        for (auto &e : enemies)
            addToMap(e.getLoc(), e.getPic());
        addToMap(myPlane.getLoc(), myPlane.getPic());
        for (auto &b : bullets)
            addToMap(b.getLoc(), b.getPic());

        printMap();

        for (auto &e : enemies)
            addToMap(e.getLoc(), string(e.getPic().size(), ' '));
        addToMap(myPlane.getLoc(), string(myPlane.getPic().size(), ' '));
        for (auto &b : bullets)
            addToMap(b.getLoc(), string(b.getPic().size(), ' '));

        clearScreen();
    }
};

/**
 * @design

-<-`

*=>-

   ^
 -< >-
   V

   A
  < >
 / V \

   A
  d b
-+| |+-
  q p
   V

*/

/**
 * @about headTailX

1234
 1      dx=1, ht=dx-4=-3
  1     dx=1, ht=-2=dx+ht


1234
 1      dx=1, ht=-3
   1    dx=2, ht=-1=dx+ht
    1   dx=1, ht=dx+-1=0
*/