#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <term.h>
#include <unistd.h>
#include <utility>
#include <vector>

const int SCREEN_X = 40;
const int SCREEN_Y = 0;

enum Type { plane, bullet, enemy, undefined };

class Loc {
  protected:
    int x, y;

  public:
    Type id = undefined;
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
  public:
    Loc loc;

    Base(int x, int y) : loc(x, y) {}
    bool operator<(const Base &other) const { return loc < other.loc; }
    bool operator==(const Base &other) const { return loc == other.loc; }

    virtual std::string getPic()     = 0;
    virtual int getSpeed()           = 0;
    virtual void setSpeed(int speed) = 0;

    static bool cmp(Base *x, Base *y) { return x->loc < y->loc; }
};

class Plane : public Base {
  private:
  public:
    Plane(int x, int y) : Base(x, y) {}

    std::string getPic() override { return "-<-`"; }
    int getSpeed() override { return 1; }
    void setSpeed(int speed) override {}
};

class Bullet : public Base {
  private:
    int speed;

  public:
    Bullet(int x, int y, int speed = 1) : Base(x, y), speed(speed) {}

    std::string getPic() override { return "~"; }
    int getSpeed() override { return speed; }
    void setSpeed(int speed) override { this->speed = speed; }
};

class Enemy : public Base {
  private:
    int speed;

  public:
    Enemy(int x, int y, int speed) : Base(x, y), speed(speed) {}

    std::string getPic() override { return "X"; }
    int getSpeed() override { return speed; }
    void setSpeed(int speed) override { this->speed = speed; }
};

class Screen {
  private:
  public:
    static void clearScreen() {
        if (!cur_term) {
            int result;
            setupterm(NULL, STDOUT_FILENO, &result);
            if (result <= 0)
                return;
        }
        putp(tigetstr("clear"));
    }

    void flushScreen(Plane &myPlane, std::vector<Enemy *> &enemies) {
        using namespace std;
        std::vector<Base *> elems(enemies.begin(), enemies.end());
        elems.push_back(&myPlane);

        sort(elems.begin(), elems.end(), Base::cmp);
        clearScreen();
        bool lastShadow = false;
        for (auto itor = elems.begin(); itor != elems.end(); itor++) {
            Loc &loc = (*itor)->loc;
            if (itor == elems.begin()) {
                cout << string(loc.getY(), '\n') << string(loc.getX(), ' ') << (*itor)->getPic();
            } else {
                auto &prevLoc = (*(itor - 1))->loc;
                if (loc.getY() == myPlane.loc.getY() && loc.getX() > myPlane.loc.getX() && loc.getX() - myPlane.loc.getX() < 4) {
                    lastShadow = true;
                    continue;
                } else
                    lastShadow = false;

                int deltaY = loc.getY() - prevLoc.getY();
                int deltaX = loc.getX() - prevLoc.getX();  // delta of first pixel, not (next.head - prev.tail)
                // int headTailX = deltaX - int((*itor)->getPic().size());  // used for graph shadow  // !! otherwise dX-size is int and this expr is tautology

                if (deltaY == 0) {
                    if (!dynamic_cast<Plane *>(*(itor - 1)) || deltaX >= myPlane.getPic().size()) {
                        cout << string(deltaX - (int)((*(itor - 1))->getPic().size()), ' ') << (*itor)->getPic();
                        // headTailX = deltaX + headTailX;
                    }
                } else if (deltaY > 0) {
                    cout << string(deltaY, '\n') << string(loc.getX(), ' ') << (*itor)->getPic();
                } else {
                    throw std::runtime_error("deltaY < 0");
                }
            }
        }
        cout << string(SCREEN_Y - elems.back()->loc.getY(), '\n') << endl;
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