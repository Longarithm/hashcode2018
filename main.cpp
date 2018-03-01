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
  int ts, tf;
  int num;

  void read() {
    scanf("%d%d%d%d%d%d", &s.x, &s.y, &f.x, &f.y, &ts, &tf);
  }

  bool operator <(const Ride & r) const {
    return ts < r.ts;
  }
};

const int maxn = 10010;
const int maxx = 10010;
const int maxf = 1010;

int r, c, f, n, b, t;
Ride rides[maxn];
int inv[maxn];
bool used[maxn];

bool read() {
  if (scanf("%d%d%d%d%d%d", &r, &c, &f, &n, &b, &t) < 6) {
    return false;
  }
  for (int i = 0; i < n; ++i) {
    rides[i].read();
    rides[i].num = i;
  }
  return true;
}

vector<int> ans[maxf];
vector<int> score[maxf];
vector<Pnt> pos[maxf];
vector<int> tfree[maxf];

int gettget(int t, Pnt p, int num) {
  t = max(rides[num].ts, t + p.dist(rides[num].s));
  if (t + rides[num].s.dist(rides[num].f) > rides[num].tf) {
    return -1;
  }
  return t;
}

void solve() {
  for (int i = 0; i < f; ++i) {
    ans[i].clear();
    score[i].clear();
    pos[i].clear();
    tfree[i].clear();
    pos[i].pb(Pnt(0, 0));
    tfree[i].pb(0);
  }

  sort(rides, rides + n);
  for (int i = 0; i < n; ++i) {
    inv[rides[i].num] = i;
  }
  memset(used, 0, sizeof(used));

  for (int it = 0; it < 100; ++it) {
    for (int i = 0; i < n; ++i) {
      if (used[i]) {
        continue;
      }
      int ridelen = rides[i].s.dist(rides[i].f);

      int chosen = -1, chtget = -1, chscore = -1;
      for (int j = 0; j < f; ++j) {
        int ctfree = *tfree[j].rbegin();
        Pnt cpos = *pos[j].rbegin();
        int tget = gettget(ctfree, cpos, i);
        if (tget != -1) {
          int score = ridelen + (tget == rides[i].ts ? b : 0);
          //        if (chosen == -1 || score > chscore || (score == chscore && (ctfree > (*tfree[chosen].rbegin())))) {
          if (chosen == -1 || tget < chtget || (tget == chtget && (ctfree > (*tfree[chosen].rbegin())))) {
            chosen = j;
            chtget = tget;
            chscore = score;
          }
        }
        }

        if (chosen != -1) {
          ans[chosen].pb(rides[i].num);
          score[chosen].pb(ridelen + (chtget == rides[i].ts ? b : 0));
          tfree[chosen].pb(chtget + ridelen);
          pos[chosen].pb(rides[i].f);
          used[i] = true;
        } else {
          if (it == 0) {
            continue;
          }
          int chosen = -1, chtget = -1, chk = -1, chdiff = 0;
          for (int j = 0; j < f; ++j) {
            int diff = 0;
            for (int k = sz(tfree[j]) - 2; k >= 0; --k) {
              diff -= score[j][k];
              int ctfree = tfree[j][k];
              Pnt cpos = pos[j][k];
              int tget = gettget(ctfree, cpos, i);
              if (tget != -1) {
                int cscore = ridelen + (tget == rides[i].ts ? b : 0);

                int ntfree = tget + ridelen;
                Pnt npos = rides[i].f;
                for (int l = k; l < sz(ans[j]); ++l) {
                  int ci = inv[ans[j][l]];
                  int ntget = gettget(ntfree, npos, ci);
                  if (ntget != -1) {
                    int nridelen = rides[ci].s.dist(rides[ci].f);
                    cscore += nridelen + (ntget == rides[ci].ts ? b : 0);
                    ntfree = ntget + nridelen;
                    npos = rides[ci].f;
                  }
                }

                if (cscore + diff > chdiff) {
                  chosen = j;
                  chk = k;
                  chtget = tget;
                  chdiff = diff + cscore;
                }
                break;
              }
            }
          }

          if (chosen != -1) {
            vector<int> tmp;
            tmp.clear();
            while (sz(tfree[chosen]) - 1 > chk) {
              tfree[chosen].pop_back();
              pos[chosen].pop_back();
              score[chosen].pop_back();
              tmp.pb(*ans[chosen].rbegin());
              ans[chosen].pop_back();
            }
            reverse(tmp.begin(), tmp.end());

            ans[chosen].pb(rides[i].num);
            score[chosen].pb(ridelen + (chtget == rides[i].ts ? b : 0));
            tfree[chosen].pb(chtget + ridelen);
            pos[chosen].pb(rides[i].f);
            used[i] = true;

            for (int l = 0; l < sz(tmp); ++l) {
              int ctfree = *tfree[chosen].rbegin();
              Pnt cpos = *pos[chosen].rbegin();
              int ci = inv[tmp[l]];
              used[ci] = false;
              int ctget = gettget(ctfree, cpos, ci);
              if (ctget != -1) {
                int cridelen = rides[ci].s.dist(rides[ci].f);

                ans[chosen].pb(rides[ci].num);
                score[chosen].pb(cridelen + (ctget == rides[ci].ts ? b : 0));
                tfree[chosen].pb(ctget + cridelen);
                pos[chosen].pb(rides[ci].f);
                used[ci] = true;
              }
            }
          }
        }
      }
    }

    int sum = 0;

    for (int i = 0; i < f; ++i) {
      printf("%d", sz(ans[i]));
      for (int j = 0; j < sz(ans[i]); ++j) {
        sum += score[i][j];
        printf(" %d", ans[i][j]);
      }
      printf("\n");
    }

    eprintf("%d\n", sum);
  }

  int main() {
    srand(rdtsc());
    precalc();

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


