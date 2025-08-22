#include <iostream>
#include <vector>

using namespace std;

const int MaxN = 1e5 + 10, MaxM = 31 * MaxN, MaxK = 18;

struct S {
  int v, w, id;
};

struct Stack {
  int* p;
  int w;
} st[MaxN];

int f[MaxN][MaxK], nxt[MaxM][2], dm[MaxM], ans[MaxM], d[MaxN], dep[MaxN], tot, t, n, q, m, top;
vector<S> g[MaxN], qu[MaxN];

void DFS(int x, int fa = 1) {
  f[x][0] = fa;
  for (int i = 1; i < MaxK; i++) {
    f[x][i] = f[f[x][i - 1]][i - 1];
  }
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

void insert(int x, int w, int p = 0) {
  for (int i = 30 - 1; i >= 0; i--) {
    bool c = x & (1 << i);
    st[++top].p = &dm[p], st[top].w = dm[p], dm[p] = w;
    if (!nxt[p][c]) nxt[p][c] = ++m;
    p = nxt[p][c];
  }
  st[++top].p = &dm[p], st[top].w = dm[p], dm[p] = w;
}

int query(int w, int x, int p = 0, int res = 0) {
  for (int i = 30 - 1; i >= 0; i--) {
    bool c = w & (1 << i);
    if (nxt[p][!c] && dm[nxt[p][!c]] >= x) p = nxt[p][!c], res ^= (1 << i);
    else p = nxt[p][c];
  }
  return res;
}

void dfs(int x, int fa = 0) {
  int lst = top;
  insert(d[x], dep[x]);
  for (auto q : qu[x]) ans[q.id] = max(ans[q.id], query(q.w, q.v));
  for (auto i : g[x]) {
    if (i.v == fa) continue;
    dfs(i.v, x);
  }
  while (top > lst) *st[top].p = st[top].w, top--;
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  for (cin >> t; t; t--) {
    cin >> n >> q, tot = m = 0;
    for (int i = 1; i <= n; i++) vector<S>().swap(qu[i]), vector<S>().swap(g[i]);
    for (int i = 1, u, v, w; i < n; i++) {
      cin >> u >> v >> w;
      g[u].push_back({v, w, i}), g[v].push_back({u, w, i});
    }
    DFS(1);
    for (int i = 1, u, v, lca; i <= q; i++) {
      cin >> u >> v, lca = LCA(u, v), ans[i] = 0;
      qu[u].push_back({dep[lca], d[u], i}), qu[v].push_back({dep[lca], d[u], i});
    }
    fill(dm, dm + MaxM - 1, -1);
    dfs(1);
    for (int i = 1; i <= q; i++) {
      cout << ans[i] << '\n';
    }
    fill(nxt[0], nxt[m + 2], 0);
  }
  return 0;
}