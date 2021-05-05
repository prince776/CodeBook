template <typename C = std::chrono::steady_clock,
          typename T = std::chrono::duration<int_fast64_t, std::nano>>
struct Stopwatch {
    std::string name;
    std::chrono::time_point<C> last_played;
    T elapsed_time;
    bool running;
    Stopwatch(const std::string &s) : name(s), running(true) { reset(); }
    Stopwatch() : Stopwatch("Time") {}
    void reset() {
        last_played = C::now();
        elapsed_time = T(0);
    }
    void pause() {
        if (!running) return;
        running = false;
        elapsed_time += C::now() - last_played;
    }
    void play() {
        if (running) return;
        running = true;
        last_played = C::now();
    }
    int_fast64_t elapsed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   elapsed_time + (running ? (C::now() - last_played) : T(0)))
            .count();
    }
    void print() const {
#ifdef TIMING
        std::cerr << name << ": " << elapsed() << " ms\n";
#endif
    }
    ~Stopwatch() { print(); }
};

