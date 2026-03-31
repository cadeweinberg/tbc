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

#include "representation/Index.hpp"

namespace tbc {
struct Operand {
  uint16_t m_kind : 3;
  uint16_t m_data;

  enum class Kind : uint8_t {
    reg,
    // freg,
    i16,
    u16,
  };

  static Operand reg(uint16_t value) {
    return Operand{static_cast<uint16_t>(Kind::reg), value};
  }

  static Operand i16(int16_t value) {
    return Operand{static_cast<uint16_t>(Kind::i16),
                   static_cast<uint16_t>(value)};
  }

  static Operand u16(uint16_t value) {
    return Operand{static_cast<uint16_t>(Kind::u16), value};
  }

  Kind kind() const { return static_cast<Kind>(m_kind); }

  template <class T> T as() const { return static_cast<T>(m_data); }
};

template <> inline Index Operand::as<Index>() const { return Index{m_data}; }

//struct OperandExtended {
//  uint32_t m_kind : 3;
//  uint32_t m_data;
//
//  enum class Kind : uint8_t {
//    reg,
//    // freg,
//    i32,
//    u32,
//  };
//
//  OperandExtended() = default;
//  OperandExtended(Kind kind, uint32_t data)
//      : m_kind(std::to_underlying(kind)), m_data(data) {}
//  OperandExtended(uint16_t kind, uint32_t data)
//      : m_kind(kind), m_data(data) {}
//  OperandExtended(Operand operand)
//      : m_kind(operand.m_kind), m_data(operand.m_data) {}
//
//  static OperandExtended reg(uint32_t value) { return {Kind::reg, value}; }
//
//  static OperandExtended i32(int32_t value) {
//    return {Kind::i32, static_cast<uint32_t>(value)};
//  }
//
//  static OperandExtended u32(uint32_t value) { return {Kind::u32, value}; }
//
//  Kind kind() const { return static_cast<Kind>(m_kind); }
//
//  template <class T> T as() const { return static_cast<T>(m_data); }
//
//  uint16_t lower() const { return static_cast<uint16_t>(m_data & 0xFFFF); }
//  uint16_t upper() const {
//    return static_cast<uint16_t>((m_data >> 16) & 0xFFFF);
//  }
//};
//
//template <> Index OperandExtended::as<Index>() const { return Index{m_data}; }

} // namespace tbc

template <> struct std::formatter<tbc::Operand::Kind> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  constexpr auto format(tbc::Operand::Kind kind, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (kind) {
    case tbc::Operand::Kind::reg:
      return std::format_to(ctx.out(), "reg");
      
    case tbc::Operand::Kind::i16:
      return std::format_to(ctx.out(), "i16");
      
    case tbc::Operand::Kind::u16:
      return std::format_to(ctx.out(), "u16");
      
    default:
      return std::format_to(ctx.out(), "unknown: {}", kind);
    }
  }
};

template <> struct std::formatter<tbc::Operand> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  constexpr auto format(tbc::Operand operand, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (operand.kind()) {
    case tbc::Operand::Kind::reg:
      return std::format_to(ctx.out(), "r{}", operand.as<uint16_t>());
    case tbc::Operand::Kind::i16:
      return std::format_to(ctx.out(), "{}", operand.as<int16_t>());
    case tbc::Operand::Kind::u16:
      return std::format_to(ctx.out(), "{}", operand.as<uint16_t>());
    default:
      return std::format_to(ctx.out(), "<invalid operand>");
    }
  }
};

namespace tbc {
inline auto operator<<(std::ostream &out, Operand operand) -> std::ostream & {
  out << std::format("{}", operand);
  return out;
}

} // namespace tbc

#endif // !TBC_REPRESENTATION_OPERAND_HPP
