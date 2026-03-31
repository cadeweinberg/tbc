//
// Copyright 2026 Cade Weinberg
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#include <cstdint>

#include <boost/assert.hpp>
#include <boost/leaf.hpp>

using namespace boost::leaf;

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace tbc {
inline leaf::result<int64_t> add(int64_t a, int64_t b);
inline leaf::result<int64_t> sub(int64_t a, int64_t b);
inline leaf::result<int64_t> mul(int64_t a, int64_t b);
inline leaf::result<int64_t> div(int64_t a, int64_t b);
inline leaf::result<int64_t> mod(int64_t a, int64_t b);

#if defined(__GNUC__) || defined(__clang__)
inline leaf::result<int64_t> add(int64_t a, int64_t b) {
  int64_t result;
  if (__builtin_add_overflow(a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in add instruction"));
  }
  return result;
}
inline leaf::result<int64_t> sub(int64_t a, int64_t b) {
  int64_t result;
  if (__builtin_sub_overflow(a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in sub instruction"));
  }
  return result;
}
inline leaf::result<int64_t> mul(int64_t a, int64_t b) {
  int64_t result;
  if (__builtin_mul_overflow(a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in mul instruction"));
  }
  return result;
}
inline leaf::result<int64_t> div(int64_t a, int64_t b) {
  if (b == 0) {
    return leaf::new_error(std::format("Division by zero in div instruction"));
  }
  if (a == INT64_MIN && b == -1) {
    return leaf::new_error(std::format("Integer overflow in div instruction"));
  }
  return a / b;
}
inline leaf::result<int64_t> mod(int64_t a, int64_t b) {
  if (b == 0) {
    return leaf::new_error(std::format("Division by zero in mod instruction"));
  }
  if (a == INT64_MIN && b == -1) {
    return leaf::new_error(std::format("Integer overflow in mod instruction"));
  }
  return a % b;
}
#elif defined(_MSC_VER)
#pragma intrinsic(__ll_rshift)
#pragma intrinsic(__ull_rshift)
#pragma intrinsic(_add_overflow_i64)
#pragma intrinsic(_sub_overflow_i64)
#pragma intrinsic(_mul_overflow_i64)

inline leaf::result<int64_t> sra(int64_t value, uint8_t amount) {
  if (amount >= 64) {
    return leaf::new_error(
        std::format("Shift value too large in sra instruction: {}", amount));
  }
  return __ll_rshift(value, amount);
}

inline leaf::result<uint64_t> srl(uint64_t value, uint8_t amount) {
  if (amount >= 64) {
    return leaf::new_error(
        std::format("Shift value too large in srl instruction: {}", amount));
  }
  return __ull_rshift(value, amount);
}

inline leaf::result<int64_t> add(int64_t a, int64_t b) {
  uint8_t carry_in = 0, carry_out = 0;
  int64_t result;
  if (carry_out = _add_overflow_i64(carry_in, a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in add instruction"));
  }
  return result;
}

inline leaf::result<int64_t> sub(int64_t a, int64_t b) {
  uint8_t carry_in = 1, carry_out = 0;
  int64_t result;
  if (carry_out = _sub_overflow_i64(carry_in, a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in sub instruction"));
  }
  return result;
}

inline leaf::result<int64_t> mul(int64_t a, int64_t b) {
  int64_t result;
  if (_mul_overflow_i64(a, b, &result)) {
    return leaf::new_error(std::format("Integer overflow in mul instruction"));
  }
  return result;
}

inline leaf::result<int64_t> div(int64_t a, int64_t b) {
  if (b == 0) {
    return leaf::new_error(std::format("Division by zero in div instruction"));
  }
  if (a == INT64_MIN && b == -1) {
    return leaf::new_error(std::format("Integer overflow in div instruction"));
  }
  return a / b;
}

inline leaf::result<int64_t> mod(int64_t a, int64_t b) {
  if (b == 0) {
    return leaf::new_error(std::format("Division by zero in mod instruction"));
  }
  if (a == INT64_MIN && b == -1) {
    return leaf::new_error(std::format("Integer overflow in mod instruction"));
  }
  return a % b;
}

#endif
} // namespace tbc
