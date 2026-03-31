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

#ifndef TBC_REPRESENTATION_VALUE_HPP
#define TBC_REPRESENTATION_VALUE_HPP

#include <cstdint>
#include <variant>
#include <memory>

namespace tbc {
using Nil = std::monostate;

class Value {
public:
  using Variant = std::variant<Nil, bool, uint64_t, int64_t>;
  using Ptr = std::shared_ptr<Value>;

private:
  struct Private {};

  Variant m_variant;

public:
  Value(Private, Nil nil) : m_variant(nil) {}
  Value(Private, bool value) : m_variant(value) {}
  Value(Private, uint64_t value) : m_variant(value) {}
  Value(Private, int64_t value) : m_variant(value) {}

  template <class T> static Ptr create(T value) {
    return std::make_shared<Value>(Private{}, std::move(value));
  }

  template <class T> bool is() const {
    return std::holds_alternative<T>(m_variant);
  }

  template <class T> const T &as() const { return std::get<T>(m_variant); };
  template <class T> T &as() { return std::get<T>(m_variant); };

  Variant const &variant() const { return m_variant; }
  Variant &variant() { return m_variant; }
};
} // namespace tbc

template <> struct std::formatter<tbc::Nil> {
   template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator {
    return ctx.end();
  }
  template <class FormatContext>
  auto format(const tbc::Nil &, FormatContext &context) const
      -> FormatContext::iterator {
    return std::format_to(context.out(), "nil");
  }
};

template <>
struct std::formatter<tbc::Value> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator {
    return ctx.end();
  }
  template <class FormatContext>
  auto format(const tbc::Value &value, FormatContext &context) const
      -> FormatContext::iterator {
    return std::visit(
        [&](const auto &variant) {
          return std::format_to(context.out(), "{}", variant);
        },
        value.variant());
  }
};

namespace tbc {
inline std::ostream &operator<<(std::ostream &out, const Value &value) {
  return out << std::format("{}", value);
}
} // namespace tbc

#endif // !TBC_REPRESENTATION_VALUE_HPP
