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

#ifndef TBC_REPRESENTATION_OPERAND_HPP
#define TBC_REPRESENTATION_OPERAND_HPP

#include <cstdint>
#include <format>
#include <ostream>

namespace tbc {
struct Operand {
  uint16_t kind : 3;
  uint16_t data;

  enum class Kind : uint8_t {
    register_,
    stack,
    label,
    i16,
    u16,
  };

  static Operand register_(uint16_t index) {
    return {std::to_underlying(Kind::register_), index};
  }
  static Operand stack(uint16_t index) {
    return {std::to_underlying(Kind::stack), index};
  }
  static Operand label(uint16_t index) {
    return {std::to_underlying(Kind::label), index};
  }
  static Operand i16(uint16_t index) {
    return {std::to_underlying(Kind::i16), index};
  }
  static Operand u16(uint16_t index) {
    return {std::to_underlying(Kind::u16), index};
  }
};

} // namespace tbc

template <> struct std::formatter<tbc::Operand> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  constexpr auto format(tbc::Operand operand, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (static_cast<tbc::Operand::Kind>(operand.kind)) {
    case tbc::Operand::Kind::register_:
      return std::format_to(ctx.out(), "r{}", operand.data);
    case tbc::Operand::Kind::stack:
      return std::format_to(ctx.out(), "s{}", operand.data);
    case tbc::Operand::Kind::label:
      return std::format_to(ctx.out(), "l{}", operand.data);
    case tbc::Operand::Kind::i16:
      return std::format_to(ctx.out(), "#{}", operand.data);
    case tbc::Operand::Kind::u16:
      return std::format_to(ctx.out(), "#{}", operand.data);
    default:
      std::abort();
    }
  }
};

namespace tbc {
inline auto operator<<(std::ostream &out, Operand operand)
    -> std::ostream & {
  out << std::format("{}", operand);
  return out;
}

} // namespace tbc

#endif // !TBC_REPRESENTATION_OPERAND_HPP
