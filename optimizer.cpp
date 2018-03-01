#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <queue>
#include <bitset>
#include <fstream>

using namespace std;

#define FOR(a, b) for (int a = 0; a < (b); ++a)
#define clr(a) memset(a, 0, sizeof(a))
#define pb push_back
#define forab(i, a, b) for(int i = int(a); i < int(b); ++i)
#define forba(i, b, a) for(int i = int(b) - 1; i >= int(a); --i)
#define forn(i, n) forab(i, 0, n)
#ifdef LOCAL
#define debug(a) cerr << #a << ": " << a << '\n';
#else
#define debug(a)
#endif

typedef long long ll;
typedef long double ld;

const int INF = 1e9;
const ld EPS = 1e-8;
const ld PI = acos(-1.0L);
const int MAXN = 1e5;



set<int> unassigned;
struct Ride;
vector<Ride> rides;

struct Pnt {
    int x, y;

    Pnt(): x(), y() {}
    Pnt(int x, int y): x(x), y(y) {}

    int dist(const Pnt & p) const {
        return abs(x - p.x) + abs(y - p.y);
    }

    bool operator < (const Pnt & other) const {
        if (x != other.x) {
            return x < other.x;
        } else {
            return y < other.y;
        }
    }

    bool operator != (const Pnt & other) const {
        return x != other.x && y != other.y;
    }
};

int rows, columns, vehiclesCount, ridesCount, bonus, steps;

struct Ride {
    Pnt s, f;
    int st, ft;
    int id;
    int cost;

    void read(int id, istream& in) {
        in >> s.x >> s.y >> f.x >> f.y >> st >> ft;
        this->id = id;
        cost = f.dist(s);
    }

    bool operator <(const Ride & r) const {
        if (st != r.st) {
            return st < r.st;
        } else if (ft != r.ft) {
            return ft < r.ft;
        } else if (s != r.s) {
            return s < r.s;
        } else if (f != r.f) {
            return f < r.f;
        } else {
            return id < r.id;
        }
    }
};

struct Order {
    int startTime;
//        int endTime;
    const Ride* ride;

    Order(int time, const Ride* ride) : startTime(time), ride(ride) {}
    
    bool operator < (const Order & other) const {
        /*
        if (endTime != other.endTime) {
            return endTime < other.endTime;
        }*/

        if (startTime != other.startTime) {
            return startTime < other.startTime;
        }

        return *ride < *other.ride;
    }
};

//always assuming that the route is valid and left-normalized
struct Route {
    vector<Order> orders;
    int cost;

    Route() : cost(0), orders() {}

    void read(istream& in) {
        int length;
        in >> length;
        vector<int> rideIndices;
        forn(i, length) {
            int index;
            in >> index;
            rideIndices.push_back(index);
            unassigned.erase(index);
        }

        auto rem = constructByIndices(rideIndices);

        assert(rem.size() == 0);
    }

    vector<int> constructByIndices(const vector<int>& rideIndices) {
        Pnt pos(0, 0);
        int time = 0;
        cost = 0;

        forn(i, rideIndices.size()) {
            const Ride& ride = rides[rideIndices[i]];
            time += ride.s.dist(pos);
            int timeStart = time;
            time += ride.cost;
            pos = ride.f;
            if (time > steps) {
                vector<int> res;
                for(int j = i; j < rideIndices.size(); ++j) {
                    res.push_back(rideIndices[j]);
                }

                return res;
            }

            if (timeStart == ride.st) {
                cost += bonus;
            }

            cost += ride.cost;

            orders.emplace_back(timeStart, &ride);
        }

        return vector<int>();
    }

    vector<int> getIndices() const {
        vector<int> res;
        res.reserve(orders.size());
        for (auto order: orders) {
            res.push_back(order.ride->id);
        }
        return res;
    }

};



struct Solution {
    vector<Route> routes;
    Solution() {
        routes.resize(vehiclesCount);
    }

    void read(istream& in) {
        forn(vehicle, vehiclesCount) {
            routes[vehicle].read(in);
        }
    }

    Route tryAllInsertions(const Route& route, const int rideId) {
    }

    bool insertLongest() {
        int found = -1;
        for (auto x : unassigned) {
            bool succ = false;
            for (int i = 0; i < routes.size(); ++i) {
                Route newRoute = tryAllInsertions(routes[i], x);
                if (newRoute.cost > routes[i].cost) {
                    for (auto y : routes[i].getIndices()) {
                        unassigned.insert(y);
                    }

                    for (auto y : newRoute.getIndices()) {
                        unassigned.erase(y);
                    }

                    routes[i] = newRoute;
                    succ = true;
                    cerr << "Found " << newRoute.cost - routes[i].cost << '\n';
                    break;
                }
            }

            if (succ) {
                found = x;
                break;
            }
        }

        return found != -1;
    }
};

void readTask(string nameIn, string nameOut) {
    ifstream in(nameIn);
    ifstream out(nameOut);
    in >> rows >> columns >> vehiclesCount >> ridesCount >> bonus >> steps;
    rides.resize(ridesCount);
    forn(i, ridesCount) {
        rides[i].read(i, in);
        unassigned.insert(i);
    }

    Solution solution;
    solution.read(out);

    while (solution.insertLongest()) {
    }
}

int main(int argc, char** argv) {
#ifdef LOCAL
    //freopen("", "r", stdin);
    //freopen("", "w", stdout);
    //freopen("", "w", stderr);
#endif
    if (argc != 3) {
        cerr << "Format: opimizer <in> <out>\n";
        return -1;
    }

    string fileIn(argv[1]);
    string fileOut(argv[2]);
    readTask(fileIn, fileOut);
    
    return 0;
}

