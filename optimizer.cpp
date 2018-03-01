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

bool compareByCost(const int a, const int b) {
    return rides[a].cost < rides[b].cost;
}


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

        //assert(rem.size() == 0);
    }

    vector<int> constructByIndices(const vector<int>& rideIndices) {
        /*
        cerr << "ConstructByIndices ";
        for (auto x : rideIndices) {
            cerr << x << ' ';
        }
        cerr << '\n';*/

        Pnt pos(0, 0);
        int time = 0;
        cost = 0;
        orders.clear();

        forn(i, rideIndices.size()) {
            const Ride& ride = rides[rideIndices[i]];
            int timeBefore = time;
            time += ride.s.dist(pos);
            int timeStart = max(time, ride.st);
            time = timeStart;
            time += ride.cost;
            /*
            if (ride.id == 3696) {
                cerr << "Ride3696 " << timeBefore << ' ' << time << '\n';
                cerr << ride.st << ' ' << ride.ft << ' ' << ride.cost << '\n';
            }*/

            if (time > ride.ft) {
                time = timeBefore;
                continue;
            }

            pos = ride.f;
            if (time > steps) {
                vector<int> res;
                for(int j = i; j < rideIndices.size(); ++j) {
                    res.push_back(rideIndices[j]);
                }

                return res;
            }
//            cerr << i << ' ' << timeStart << ' ' << time << '\n';

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

    int getCost() {
        int res = 0;
        for (auto& x : routes) {
            res += x.cost;
        }
        return res;
    }

    Route tryAllInsertions(const Route& route, const int rideId) {
        vector<int> rideIndices = route.getIndices();
        rideIndices.push_back(rideId);
        Route best;
        for (int i = rideIndices.size() - 1; i >= 0; --i) {
            Route cur;
            cur.constructByIndices(rideIndices);
            if (cur.cost > best.cost) {
                best = std::move(cur);
            }
            if (i > 0) {
                swap(rideIndices[i], rideIndices[i - 1]);
            }
        }

        return best;
    }

    bool insertLongest() {
        int found = -1;
        vector<int> unassignedLongest(unassigned.begin(), unassigned.end());
        sort(unassignedLongest.begin(), unassignedLongest.end(), compareByCost);
        for (auto x : unassignedLongest) {
//            cerr << "Watching " << x << '\n';
            bool succ = false;
            for (int i = 0; i < routes.size(); ++i) {
                Route newRoute = tryAllInsertions(routes[i], x);
                if (newRoute.cost > routes[i].cost) {
                    cerr << "Found " << newRoute.cost - routes[i].cost << '\n';
                    for (auto y : routes[i].getIndices()) {
                        unassigned.insert(y);
                    }

                    for (auto y : newRoute.getIndices()) {
                        unassigned.erase(y);
                    }

                    routes[i] = newRoute;
                    succ = true;
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
    void write(ostream& ans) {
        for (const auto& route : routes) {
            const auto& indices = route.getIndices();
            ans << indices.size() << ' ';
            for (auto x : indices) {
                ans << x << ' ';
            }
            ans << '\n';
        }
    }
};

void solve(string nameIn, string nameOut, string nameAns) {
    ifstream in(nameIn);
    ifstream out(nameOut);
    in >> rows >> columns >> vehiclesCount >> ridesCount >> bonus >> steps;
    rides.resize(ridesCount);
    forn(i, ridesCount) {
        rides[i].read(i, in);
        unassigned.insert(i);
    }
    cerr << "Task read\n";

    Solution solution;
    solution.read(out);
    cerr << "Solution read\n";
    cerr << "Total cost " << solution.getCost() << '\n';

    int cnt = 0;
    while (solution.insertLongest()) {
        cnt++;
        if (cnt % 100 == 0) {
            ofstream ans(nameAns);
            solution.write(ans);
            ans.close();
        }
    }

}

int main(int argc, char** argv) {
#ifdef LOCAL
    //freopen("", "r", stdin);
    //freopen("", "w", stdout);
    //freopen("", "w", stderr);
#endif
    if (argc != 4) {
        cerr << "Format: optimizer <in> <out> <ans>\n";
        return -1;
    }

    string fileIn(argv[1]);
    string fileOut(argv[2]);
    string fileAns(argv[3]);
    solve(fileIn, fileOut, fileAns);
    
    return 0;
}

