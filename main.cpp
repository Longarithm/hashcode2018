#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef long long ll;

long long rdtsc() {
  long long tmp;
  asm("rdtsc" : "=A"(tmp));
  return tmp;
}

inline int myrand() {
#ifdef _WIN32
  return abs((rand() << 15) ^ rand());
#else
  return rand();
#endif
}

inline int rnd(int x) {
  return myrand() % x;
}

#ifdef LOCAL
#define LLD "%lld"
#else
#ifdef _WIN32
#define LLD "%I64d"
#else
#define LLD "%lld"
#endif
#endif

#ifdef DEBUG
#define eprintf(...) fprintf(stderr, __VA_ARGS__), fflush(stdout)
#else
#define eprintf(...) ;
#endif

#define pb push_back
#define mp make_pair
#define sz(x) ((int)(x).size())
#define TASK "text"

void precalc() {
}

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

bool read() {
  if (scanf("%d%d%d%d%d%d", &r, &c, &f, &b, &n, &t) < 6) {
    return false;
  }
  for (int i = 0; i < n; ++i) {
    rides[i].read();
  }
  return true;
}

vector<int> 

void solve() {
}

int main() {
  srand(rdtsc());
  precalc();
#ifdef LOCAL 
  assert(freopen(TASK".out", "w", stdout));
  assert(freopen(TASK".in", "r", stdin));
#endif

  while (true) {
    if (!read()) {
      break;
    }
    solve();
#ifdef DEBUG
    eprintf("Time %.2f\n", (double) clock() / CLOCKS_PER_SEC);
#endif
  }
  return 0;
}


