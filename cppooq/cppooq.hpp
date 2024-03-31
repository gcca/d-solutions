#pragma once

#define CPPOOQ_TRAIT(T)                                                        \
public:                                                                        \
  virtual ~T() = default;                                                      \
                                                                               \
protected:                                                                     \
  T() = default;                                                               \
                                                                               \
private:                                                                       \
  T(const T &) = delete;                                                       \
  T(T &&) = default;                                                           \
  T &operator=(const T &) = delete;                                            \
  T &operator=(T &&) = default
