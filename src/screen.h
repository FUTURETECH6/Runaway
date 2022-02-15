#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <term.h>
#include <unistd.h>
#include <utility>
#include <vector>

enum Type { plane, bullet, enemy };
std::map<Type, const char *> type2Pic{{plane, "-<-`"}, {bullet, "~"}, {enemy, "x"}};

class Loc {
  private:
    int x, y;
    Type type;

  public:
    Loc(int x, int y, Type type) : x(x), y(y), type(type) {}

    int getX() { return x; }
    int getY() { return y; }
    Type getType() { return type; }
    std::pair<int, int> getLocPair() { return std::make_pair(x, y); }

    bool operator<(const Loc &other) const { return y < other.y || (y == other.y && x < other.x); }
};

class Screen {
  private:
    std::vector<Loc> locs;

  public:
    void addLoc(Loc l) { locs.push_back(l); }

    static void clear() {
        if (!cur_term) {
            int result;
            setupterm(NULL, STDOUT_FILENO, &result);
            if (result <= 0)
                return;
        }
        putp(tigetstr("clear"));
    }

    void flush() {
        using namespace std;
        sort(locs.begin(), locs.end());
        clear();
        for (auto itor = locs.begin(); itor != locs.end(); itor++) {
            if (itor == locs.begin()) {
                cout << string(itor->getY(), '\n') << string(itor->getX(), ' ') << type2Pic[itor->getType()] << endl;
            } else {
                int deltaY = itor->getY() - (itor - 1)->getY();
                if (deltaY == 0)
                    cout << string(itor->getX() - (itor - 1)->getX(), ' ') << type2Pic[itor->getType()] << endl;
                else {
                    cout << string(deltaY, '\n') << string(itor->getX(), ' ') << type2Pic[itor->getType()] << endl;
                }
            }
        }
        locs.clear();
    }
};