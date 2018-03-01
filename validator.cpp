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

struct Ride {
  Pnt s, f;
  int st, ft;

  void read() {
    scanf("%d%d%d%d%d%d", &s.x, &s.y, &f.x, &f.y, &st, &ft);
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

bool all_used_once() {
    vi who(n, -1);
    forn(i, f) {
        for (auto id: taken[i]) {
            if (who[id] != -1) {
                eprintf("Error: cars %d and %d are pretending on the same path %d\n", who[id], i, id);
                return false;
            }
            who[id] = i;
        }
    }
    return true;
}

bool print_output(const string &filename) {
    assert(freopen(filename.c_str(), "r", stdout));
    if (!all_used_once()) {
        return false;
    }
    return true;
}

int main(int argc, char **argv) {
    string test_file(argv[1]), input_file(argv[2]), output_file(argv[3]);

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
