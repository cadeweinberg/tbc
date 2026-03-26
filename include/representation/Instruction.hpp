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

#ifndef TBC_REPRESENTATION_INSTRUCTION_HPP
#define TBC_REPRESENTATION_INSTRUCTION_HPP

#include <cstdint>
#include <format>
#include <ostream>

#include "representation/operand.hpp"

namespace tbc {
struct Instruction {
  uint16_t kind : 3;
  uint16_t code : 4;
  uint16_t a_kind : 3;
  uint16_t b_kind : 3;
  uint16_t c_kind : 3;
  uint16_t a_data;
  uint16_t b_data;
  uint16_t c_data;

  enum class Kind : uint8_t {
    control,
    memory,
    bitwise,
    comparison,
    arithmetic,
  };

  enum class Control : uint8_t {
    halt,
  };

  enum class Memory : uint8_t {
    move,
  };

  enum class Bitwise : uint8_t {
    and_,
    or_,
    xor_,
    not_,
  };

  enum class Comparison : uint8_t {
    equals,
    // != is implemented as not equals, so it doesn't need its own instruction.
    less,
    // <= is implemented as not greater, so it doesn't need its own instruction.
    greater,
    // >= is implemented as not less, so it doesn't need its own instruction.
  };

  enum class Arithmetic : uint8_t {
    negate,
    add,
    subtract,
    multiply,
    divide,
    modulo,
  };

  Instruction() = default;
  Instruction(Kind kind, Control code, Operand a = {}, Operand b = {},
              Operand c = {})
      : kind(std::to_underlying(kind)), code(std::to_underlying(code)),
        a_kind(a.kind), b_kind(b.kind), c_kind(c.kind), a_data(a.data),
        b_data(b.data), c_data(c.data) {}

  Instruction(Kind kind, Memory code, Operand a = {}, Operand b = {},
              Operand c = {})
      : kind(std::to_underlying(kind)), code(std::to_underlying(code)),
        a_kind(a.kind), b_kind(b.kind), c_kind(c.kind), a_data(a.data),
        b_data(b.data), c_data(c.data) {}

  Instruction(Kind kind, Bitwise code, Operand a = {}, Operand b = {},
              Operand c = {})
      : kind(std::to_underlying(kind)), code(std::to_underlying(code)),
        a_kind(a.kind), b_kind(b.kind), c_kind(c.kind), a_data(a.data),
        b_data(b.data), c_data(c.data) {}

  Instruction(Kind kind, Comparison code, Operand a = {}, Operand b = {},
              Operand c = {})
      : kind(std::to_underlying(kind)), code(std::to_underlying(code)),
        a_kind(a.kind), b_kind(b.kind), c_kind(c.kind), a_data(a.data),
        b_data(b.data), c_data(c.data) {}

  Instruction(Kind kind, Arithmetic code, Operand a = {}, Operand b = {},
              Operand c = {})
      : kind(std::to_underlying(kind)), code(std::to_underlying(code)),
        a_kind(a.kind), b_kind(b.kind), c_kind(c.kind), a_data(a.data),
        b_data(b.data), c_data(c.data) {}

  Operand a() const { return {a_kind, a_data}; }
  Operand b() const { return {b_kind, b_data}; }
  Operand c() const { return {c_kind, c_data}; }

  static Instruction halt() { return {Kind::control, Control::halt}; }

  static Instruction move(Operand destination, Operand source) {
    return {Kind::memory, Memory::move, destination, source};
  }

  static Instruction and_(Operand destination, Operand left, Operand right) {
    return {Kind::bitwise, Bitwise::and_, destination, left, right};
  }

  static Instruction or_(Operand destination, Operand left, Operand right) {
    return {Kind::bitwise, Bitwise::or_, destination, left, right};
  }

  static Instruction xor_(Operand destination, Operand left, Operand right) {
    return {Kind::bitwise, Bitwise::xor_, destination, left, right};
  }

  static Instruction not_(Operand destination, Operand source) {
    return {Kind::bitwise, Bitwise::not_, destination, source};
  }

  static Instruction equals(Operand destination, Operand left, Operand right) {
    return {Kind::comparison, Comparison::equals, destination, left, right};
  }

  static Instruction less(Operand destination, Operand left, Operand right) {
    return {Kind::comparison, Comparison::less, destination, left, right};
  }

  static Instruction greater(Operand destination, Operand left, Operand right) {
    return {Kind::comparison, Comparison::greater, destination, left, right};
  }

  static Instruction negate(Operand destination, Operand source) {
    return {Kind::arithmetic, Arithmetic::negate, destination, source};
  }

  static Instruction add(Operand destination, Operand left, Operand right) {
    return {Kind::arithmetic, Arithmetic::add, destination, left, right};
  }

  static Instruction subtract(Operand destination, Operand left,
                              Operand right) {
    return {Kind::arithmetic, Arithmetic::subtract, destination, left, right};
  }

  static Instruction multiply(Operand destination, Operand left,
                              Operand right) {
    return {Kind::arithmetic, Arithmetic::multiply, destination, left, right};
  }

  static Instruction divide(Operand destination, Operand left, Operand right) {
    return {Kind::arithmetic, Arithmetic::divide, destination, left, right};
  }

  static Instruction modulo(Operand destination, Operand left, Operand right) {
    return {Kind::arithmetic, Arithmetic::modulo, destination, left, right};
  }
};

} // namespace tbc

template <> struct std::formatter<tbc::Instruction> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(tbc::Instruction instruction, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (static_cast<tbc::Instruction::Kind>(instruction.kind)) {
    case tbc::Instruction::Kind::control: {
      switch (static_cast<tbc::Instruction::Control>(instruction.code)) {
      case tbc::Instruction::Control::halt:
        return std::format_to(ctx.out(), "halt {}", instruction.a());
      default:
        std::abort();
      }
    }

    case tbc::Instruction::Kind::memory: {
      switch (static_cast<tbc::Instruction::Memory>(instruction.code)) {
      case tbc::Instruction::Memory::move:
        return std::format_to(ctx.out(), "move {}, {}", instruction.a(),
                              instruction.b());
      default:
        std::abort();
      }
    }

    case tbc::Instruction::Kind::bitwise: {
      switch (static_cast<tbc::Instruction::Bitwise>(instruction.code)) {
      case tbc::Instruction::Bitwise::and_:
        return std::format_to(ctx.out(), "and {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Bitwise::or_:
        return std::format_to(ctx.out(), "or {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Bitwise::xor_:
        return std::format_to(ctx.out(), "xor {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Bitwise::not_:
        return std::format_to(ctx.out(), "not {}, {}", instruction.a(),
                              instruction.b());
      default:
        std::abort();
      }
    }

    case tbc::Instruction::Kind::comparison: {
      switch (static_cast<tbc::Instruction::Comparison>(instruction.code)) {
      case tbc::Instruction::Comparison::equals:
        return std::format_to(ctx.out(), "equals {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::less:
        return std::format_to(ctx.out(), "less {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::greater:
        return std::format_to(ctx.out(), "greater {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      default:
        std::abort();
      }
    }

    case tbc::Instruction::Kind::arithmetic: {
      switch (static_cast<tbc::Instruction::Arithmetic>(instruction.code)) {
      case tbc::Instruction::Arithmetic::negate:
        return std::format_to(ctx.out(), "negate {}, {}", instruction.a(),
                              instruction.b());
      case tbc::Instruction::Arithmetic::add:
        return std::format_to(ctx.out(), "add {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Arithmetic::subtract:
        return std::format_to(ctx.out(), "subtract {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Arithmetic::multiply:
        return std::format_to(ctx.out(), "multiply {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Arithmetic::divide:
        return std::format_to(ctx.out(), "divide {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Arithmetic::modulo:
        return std::format_to(ctx.out(), "modulo {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      default:
        std::abort();
      }
    }
    }
    return ctx.out();
  }
};

namespace tbc {
inline auto operator<<(std::ostream &out, Instruction instruction)
    -> std::ostream & {
  out << std::format("{}", instruction);
  return out;
}
} // namespace tbc

#endif // !TBC_REPRESENTATION_INSTRUCTION_HPP
