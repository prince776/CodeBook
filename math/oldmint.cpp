template <uint32_t Modulus>
class Modular {
    using M = Modular;

   public:
    static_assert(int(Modulus) >= 1, "Modulus must be in the range [1, 2^31)");
    static constexpr int modulus() { return Modulus; }
    static M raw(uint32_t v) { return *reinterpret_cast<M*>(&v); }
    Modular() : v_(0) {}
    Modular(int64_t v) : v_((v %= Modulus) < 0 ? v + Modulus : v) {}
    template <class T>
    explicit operator T() const {
        return v_;
    }
    M& operator++() { return v_ = ++v_ == Modulus ? 0 : v_, *this; }
    M& operator--() { return --(v_ ? v_ : v_ = Modulus), *this; }
    M& operator*=(M o) { return v_ = uint64_t(v_) * o.v_ % Modulus, *this; }
    M& operator/=(M o) {
        auto [inv, gcd] = extgcd(o.v_, Modulus);
        assert(gcd == 1);
        return *this *= inv;
    }
    M& operator+=(M o) {
        return v_ = int(v_ += o.v_ - Modulus) < 0 ? v_ + Modulus : v_, *this;
    }
    M& operator-=(M o) {
        return v_ = int(v_ -= o.v_) < 0 ? v_ + Modulus : v_, *this;
    }
    friend M operator++(M& a, int) { return std::exchange(a, ++M(a)); }
    friend M operator--(M& a, int) { return std::exchange(a, --M(a)); }
    friend M operator+(M a) { return a; }
    friend M operator-(M a) { return a.v_ = a.v_ ? Modulus - a.v_ : 0, a; }
    friend M operator*(M a, M b) { return a *= b; }
    friend M operator/(M a, M b) { return a /= b; }
    friend M operator+(M a, M b) { return a += b; }
    friend M operator-(M a, M b) { return a -= b; }
    template <class T>
    friend T& operator>>(T& is, M& x) {
        int64_t v;
        return is >> v, x = v, is;
    }
    template <class T>
    friend T& operator<<(T& os, M x) {
        return os << x.v_;
    }
    friend bool operator==(M a, M b) { return a.v_ == b.v_; }
    friend bool operator!=(M a, M b) { return a.v_ != b.v_; }

   private:
    static std::pair<int, int> extgcd(int a, int b) {
        std::array<int, 2> x{1, 0};
        while (b) std::swap(x[0] -= a / b * x[1], x[1]), std::swap(a %= b, b);
        return {x[0], a};
    }
    uint32_t v_;
};

