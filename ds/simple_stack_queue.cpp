template <class T>
struct simple_queue {
    std::vector<T> payload;
    int pos = 0;
    void reserve(int n) { payload.reserve(n); }
    int size() const { return int(payload.size()) - pos; }
    bool empty() const { return pos == int(payload.size()); }
    void push(const T& t) { payload.push_back(t); }
    T& front() { return payload[pos]; }
    void clear() { payload.clear(), pos = 0; }
    void pop() { pos++; }
};

template <class T>
struct simple_stack {
    std::vector<T> payload;
    void reserve(int n) { payload.reserve(n); }
    int size() const { return int(payload.size()); }
    bool empty() const { return payload.empty(); }
    void push(const T& t) { payload.push_back(t); }
    T& top() { return payload.back(); }
    void clear() { payload.clear(); }
    void pop() { payload.pop_back(); }
};

