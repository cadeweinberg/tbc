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

#ifndef TBC_IMR_OPERAND_HPP
#define TBC_IMR_OPERAND_HPP

#include <cstdint>
#include <format>
#include <ostream>

namespace tbc {
struct Operand {
  uint16_t kind : 3;
  uint16_t data;

  enum class Kind : uint8_t {
    Register,
    Stack,
    label,
    Immediate,
  };

  static Operand Register(uint16_t index) {
    return {std::to_underlying(Kind::Register), index};
  }
  static Operand Immediate(uint16_t index) {
    return {std::to_underlying(Kind::Immediate), index};
  }
  static Operand Stack(uint16_t index) {
    return {std::to_underlying(Kind::Stack), index};
  }
  static Operand label(uint16_t index) {
    return {std::to_underlying(Kind::label), index};
  }
};

} // namespace tbc

template <> struct std::formatter<tbc::Operand> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <class FormatContext>
  constexpr auto format(tbc::Operand operand, FormatContext &ctx)
      -> decltype(ctx.out()) {
    switch (static_cast<tbc::Operand::Kind>(operand.kind)) {
    case tbc::Operand::Kind::Register:
      return std::format_to(ctx.out(), "r{}", operand.data);
    case tbc::Operand::Kind::Stack:
      return std::format_to(ctx.out(), "s{}", operand.data);
    case tbc::Operand::Kind::label:
      return std::format_to(ctx.out(), "l{}", operand.data);
    case tbc::Operand::Kind::Immediate:
      return std::format_to(ctx.out(), "#{}", operand.data);
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

#endif // !TBC_IMR_OPERAND_HPP
