#include <iostream>
#include <vector>

using namespace std;

const int MaxN = 1e5 + 10, MaxM = 31 * MaxN, MaxK = 18;

struct S {
  int v, w, id;
};

struct Node {
  int nxt[2], mx;

  Node() {
    nxt[0] = nxt[1] = mx = -1;
  }
} trie[MaxM];

int f[MaxN][MaxK],ans[MaxM], rt[MaxN], d[MaxN], dep[MaxN], tot, t, n, q, m, top;
vector<S> g[MaxN], qu[MaxN];

void insert(int now, int lst, int x, int w, int p = 0) {
  rt[now] = ++m, trie[m] = trie[rt[lst]], p = m;
  for (int i = 30 - 1; i >= 0; i--) {
    bool c = x & (1 << i);
    if (trie[p].nxt[c] == -1) trie[p].nxt[c] = ++m;
    else trie[++m] = trie[trie[p].nxt[c]], trie[p].nxt[c] = m;
    p = trie[p].nxt[c];
    trie[p].mx = w;
  }
}

int query(int u, int w, int x, int p = 0, int res = 0) {
  p = rt[u];
  for (int i = 30 - 1; i >= 0; i--) {
    bool c = w & (1 << i);
    if (~trie[p].nxt[!c] && trie[trie[p].nxt[!c]].mx >= x) p = trie[p].nxt[!c], res ^= (1 << i);
    else p = trie[p].nxt[c];
  }
  return res;
}

void DFS(int x, int fa = 0) {
  f[x][0] = max(1, fa);
  for (int i = 1; i < MaxK; i++) {
    f[x][i] = f[f[x][i - 1]][i - 1];
  }
  insert(x, fa, d[x], dep[x]);
  for (auto i : g[x]) {
    if (i.v == fa) continue;
    d[i.v] = d[x] ^ i.w, dep[i.v] = dep[x] + 1, DFS(i.v, x); 
  }
}

int GO(int x, int k) {
  for (int i = MaxK - 1; i >= 0; i--) {
    if (k & (1 << i)) x = f[x][i];
  }
  return x;
}

int LCA(int x, int y) {
  if (dep[x] < dep[y]) swap(x, y);
  x = GO(x, dep[x] - dep[y]);
  if (x == y) return x;
  for (int i = MaxK - 1; i >= 0; i--) {
    if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
  }
  return f[x][0];
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  for (cin >> t; t; t--) {
    cin >> n >> q, tot = m = 0;
    for (int i = 1; i <= n; i++) vector<S>().swap(g[i]);
    for (int i = 1, u, v, w; i < n; i++) {
      cin >> u >> v >> w;
      g[u].push_back({v, w, i}), g[v].push_back({u, w, i});
    }
    DFS(1);
    for (int i = 1, u, v, lca; i <= q; i++) {
      cin >> u >> v, lca = LCA(u, v);
      cout << max(query(u, d[u], dep[lca]), query(v, d[u], dep[lca])) << '\n';
    }
    for (int i = 1; i <= m; i++) trie[i] = Node();
  }
  return 0;
}