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
#ifndef TBC_INPUT_TOKEN_HPP
#define TBC_INPUT_TOKEN_HPP

#include <cstdint>
#include <format>
#include <ostream>
#include <string_view>
using namespace std::literals::string_view_literals;

namespace tbc {
enum class Token : uint8_t {
  End,

  Return,
  Let,
  Fn,
  True,
  False,
  TypeBool,
  Typei64,

  Equals,
  EqualsEquals,
  Less,
  LessEquals,
  Greater,
  GreaterEquals,

  Plus,
  Minus,
  Star,
  FSlash,
  Percent,

  Integer,
  Label,

};

std::string_view token_to_view(Token token) noexcept;

inline std::ostream &operator<<(std::ostream &out, Token token) {
  out << token_to_view(token);
  return out;
}
} // namespace tbc

template <> struct std::formatter<tbc::Token> {
  template <class ParseContext>
  auto parse(ParseContext &ctx) -> ParseContext::iterator {
    return ctx.end();
  }

  template <class FormatContext>
  auto format(tbc::Token token, FormatContext &ctx) const
      -> FormatContext::iterator {
    return std::format_to(ctx.begin(), "{}", token_to_view(token));
  }
};

#endif // !TBC_INPUT_TOKEN_HPP
