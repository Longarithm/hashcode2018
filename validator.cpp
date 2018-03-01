#include <bits/stdc++.h>

#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)
#define sz(c) int((c).size())
#define all(c) (c).begin(), (c).end()
#define pb push_back
#define mp(x, y) make_pair(x, y)
#define fst first
#define snd second

using namespace std;

using vi = vector<int>;
using ll = long long;
using pii = pair<int, int>;

#define FNAME "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stderr)
#else
#define eprintf(args...) ;
#endif

#define print_err(args...) fprintf(stdout, args), fflush(stdout)

struct Pnt {
  int x, y;

  Pnt(): x(), y() {}
  Pnt(int x, int y): x(x), y(y) {}

  int dist(const Pnt & p) const {
    return abs(x - p.x) + abs(y - p.y);
  }
};

int dist(const Pnt &p, const Pnt &q) {
    return p.dist(q);
}

struct Ride {
  Pnt s, f;
  int st, ft;

  void read() {
    assert(scanf("%d%d%d%d%d%d", &s.x, &s.y, &f.x, &f.y, &st, &ft));
  }

  bool operator <(const Ride & r) const {
    return st < r.st;
  }
};

const int maxn = 10010;
const int maxx = 10010;
const int maxf = 1010;

int r, c, f, n, b, t;
Ride rides[maxn];

bool read_test(string filename) {
    assert(freopen(filename.c_str(), "r", stdin));
    if (scanf("%d%d%d%d%d%d", &r, &c, &f, &n, &b, &t) < 6) {
        return false;
    }
    for (int i = 0; i < n; ++i) {
        rides[i].read();
    }
    fclose(stdin);
    return true;
}

vi taken[maxf];

bool read_answer(const string &filename) {
    assert(freopen(filename.c_str(), "r", stdin));
    forn(i, f) {
        taken[i].clear();
        int m;
        if (scanf("%d", &m) < 1) {
            return false;
        }
        taken[i].resize(m);
        forn(j, m) {
            if (scanf("%d", &taken[i][j]) < 1) {
                return false;
            }
        }
    }
    fclose(stdin);
    return true;
}
    
vi who;

bool all_used_once() {
    who.clear();
    who.resize(n, -1);
    forn(i, f) {
        for (auto id: taken[i]) {
            if (who[id] != -1) {
                eprintf("Error: cars %d and %d are pretending on the same ride %d\n", who[id], i, id);
                return false;
            }
            who[id] = i;
        }
    }
    return true;
}

bool print_output(const string &filename) {
    assert(filename == "");
    //assert(freopen(filename.c_str(), "r", stdout));
    if (!all_used_once()) {
        return false;
    }

    int bonuses = 0;
    int failed_rides = 0;
    ll failed_dists = 0;
    forn(id, n) {
        if (who[id] == -1) {
            failed_rides++;
            failed_dists += dist(rides[id].s, rides[id].f);
        }
    } 

    ll points = 0;
    forn(i, f) {
        Pnt cur_pos(0, 0);
        ll cur_time = 0;
        for (auto id: taken[i]) {
            cur_time += dist(cur_pos, rides[id].s);
            if (cur_time < rides[id].st) {
                cur_time = rides[id].st;
            }

            if (cur_time == rides[id].st) {
                bonuses++;
                points += b;
            }
            
            cur_time += dist(rides[id].s, rides[id].f);
            if (cur_time > rides[id].ft) {
                eprintf("Error: car %d arrived to the final point of ride %d at time %lld, want %d\n", i, id, cur_time, rides[id].ft);
                return false;
            }

            points += dist(rides[id].s, rides[id].f);

            cur_pos = rides[id].f;
        }
    }

    eprintf("GOT %lld points!\n", points);
    eprintf("STATS:\n");
    eprintf("used %lld bonuses, failed %lld rides, failed %lld dists\n", (ll)bonuses, (ll)failed_rides, failed_dists);
    
    return true;
}

int main(int argc, char **argv) {
    eprintf("Started validating\n");
    assert(argc == 3);
    string test_file(argv[1]), input_file(argv[2]), output_file("");

    if (!read_test(test_file)) {
        eprintf("Error during reading testdata!");
        return 1; 
    }

    if (!read_answer(input_file)) {
        eprintf("Error during reading answer!\n");
        return 1;
    }

    if (!print_output(output_file)) {
        eprintf("Error during logging output!\n");
        return 1;
    }
    
    eprintf("Succeed\n");

    return 0;
}
