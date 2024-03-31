#include "dsolutions.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

class TimerImpl {
public:
  explicit TimerImpl(std::string &&name, std::string &&sig)
      : name_{std::move(name)}, sig_{std::move(sig)},
        start_{std::chrono::steady_clock::now()} {}

  ~TimerImpl() {
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::steady_clock::now() - start_)
                             .count();
    times.Add(std::move(name_), std::move(sig_), elapsed);
  }

private:
  std::string name_;
  std::string sig_;
  std::chrono::time_point<std::chrono::steady_clock> start_;
};

Timer::Timer(std::string &&name, std::string &&sig)
    : impl_{std::make_unique<TimerImpl>(std::move(name), std::move(sig))} {}

Timer::~Timer() = default;

class TimesImpl {
public:
  TimesImpl() : max_name_length_{0}, max_sig_length_{0} {
    records_.reserve(10);
  }

  void Add(std::string &&name, std::string &&sig, std::size_t elapsed) {
    const std::size_t name_length = name.size();
    const std::size_t sig_length = sig.size();
    if (max_name_length_ < name_length) max_name_length_ = name_length;
    if (max_sig_length_ < sig_length) max_sig_length_ = sig_length;
    records_.emplace_back(std::move(name), std::move(sig), elapsed);
  }

  void Show() {
    for (const auto &rec : records_) {
      std::cout << '[' << std::left << std::setw(max_name_length_)
                << std::get<0>(rec) << ' ' << std::setw(max_sig_length_)
                << std::get<1>(rec) << " : elapsed=" << std::setw(5)
                << (std::get<2>(rec) / 1000.0) << ']' << std::endl;
    }
    records_.clear();
    max_name_length_ = 0;
    max_sig_length_ = 0;
  }

private:
  std::size_t max_name_length_;
  std::size_t max_sig_length_;
  std::vector<std::tuple<std::string, std::string, std::size_t>> records_;
};

Times::Times() : impl_{std::make_unique<TimesImpl>()} {}

Times times;

void Times::Add(std::string &&name, std::string &&sig, std::size_t count) {
  impl_->Add(std::move(name), std::move(sig), count);
}

void Times::Show() { impl_->Show(); }

namespace dsol_1 {

long long sum_3_5_le(long long N) {
  long long total = 0;
  for (long long n = 3; n < N; n++) {
    if ((n % 3 == 0) or (n % 5 == 0)) { total += n; }
  }
  return total;
}

inline long long L(long long N, long long k) { return std::floor((N - 1) / k); }
inline long long Sn(long long n) { return (n * (n + 1)) / 2; }
inline long long ST(long long N, long long k) { return k * Sn(L(N, k)); }

long long sum_3_5_le_pro(long long N) {
  return ST(N, 3) + ST(N, 5) - ST(N, 15);
}

} // namespace dsol_1

namespace dsol_2 {

long long fib(long long n) {
  if (n <= 1) return 1;
  return fib(n - 1) + fib(n - 2);
}

long long sumevenfib(long long N) {
  long long total = 0;

  long long i = 2;
  long long n = fib(i);

  while (n <= N) {
    if (n % 2 == 0) { total += n; }
    n = fib(++i);
  }

  return total;
}

long long fibeven(long long n) {
  if (n == 0) return 0;
  if (n == 1) return 2;
  return (4 * fibeven(n - 1)) + fibeven(n - 2);
}

long long sumevenfib_step(long long N) {
  long long total = 0;

  long long i = 1;
  long long n = fibeven(i);

  while (n <= N) {
    total += n;
    n = fibeven(++i);
  }

  return total;
}

} // namespace dsol_2
