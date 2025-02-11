template <int MAXN, class T = int>
struct HLPP {
    const T INF = numeric_limits<T>::max();

    struct edge {
        int to, rev;
        T f;
    };

    int s = MAXN - 1, t = MAXN - 2;

    vector<edge> adj[MAXN];
    vector<int> lst[MAXN], gap[MAXN];
    T excess[MAXN];

    int highest, height[MAXN], cnt[MAXN], work;

    void addEdge(int from, int to, int f, bool isDirected = true) {
        adj[from].push_back({to, adj[to].size(), f});
        adj[to].push_back({from, adj[from].size() - 1, isDirected ? 0 : f});
    }

    void updHeight(int v, int nh) {
        work++;
        if (height[v] != MAXN) cnt[height[v]]--;
        height[v] = nh;
        if (nh == MAXN) return;
        cnt[nh]++, highest = nh;
        gap[nh].push_back(v);
        if (excess[v] > 0) lst[nh].push_back(v);
    }

    void globalRelabel() {
        work = 0;
        fill(all(height), MAXN);
        fill(all(cnt), 0);
        for (int i = 0; i < highest; i++) lst[i].clear(), gap[i].clear();
        height[t] = 0;
        queue<int> q({t});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& e : adj[v])
                if (height[e.to] == MAXN && adj[e.to][e.rev].f > 0)
                    q.push(e.to), updHeight(e.to, height[v] + 1);
            highest = height[v];
        }
    }

    void push(int v, edge& e) {
        if (excess[e.to] == 0) lst[height[e.to]].push_back(e.to);
        T df = min(excess[v], e.f);
        e.f -= df, adj[e.to][e.rev].f += df;
        excess[v] -= df, excess[e.to] += df;
    }

    void discharge(int v) {
        int nh = MAXN;
        for (auto& e : adj[v]) {
            if (e.f > 0) {
                if (height[v] == height[e.to] + 1) {
                    push(v, e);
                    if (excess[v] <= 0) return;
                } else
                    nh = min(nh, height[e.to] + 1);
            }
        }
        if (cnt[height[v]] > 1)
            updHeight(v, nh);
        else {
            for (int i = height[v]; i <= highest; i++) {
                for (auto j : gap[i]) updHeight(j, MAXN);
                gap[i].clear();
            }
        }
    }

    T calc(int heur_n = MAXN) {
        fill(all(excess), 0);
        excess[s] = INF, excess[t] = -INF;
        globalRelabel();
        for (auto& e : adj[s]) push(s, e);
        for (; highest >= 0; highest--) {
            while (!lst[highest].empty()) {
                int v = lst[highest].back();
                lst[highest].pop_back();
                discharge(v);
                if (work > 4 * heur_n) globalRelabel();
            }
        }
        return excess[t] + INF;
    }
};
