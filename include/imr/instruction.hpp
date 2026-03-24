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

#ifndef TBC_IMR_INSTRUCTION_HPP
#define TBC_IMR_INSTRUCTION_HPP

#include <cstdint>
#include <format>
#include <ostream>

#include "imr/operand.hpp"

namespace tbc {
struct Instruction {
  uint16_t kind : 3;
  uint16_t code : 4;
  uint16_t A_kind : 3;
  uint16_t B_kind : 3;
  uint16_t C_kind : 3;
  uint16_t A_data;
  uint16_t B_data;
  uint16_t C_data;

  enum class Kind : uint8_t {
    Control,
    Memory,
    Bitwise,
    Comparison,
    Arithmetic,
  };

  enum class Control : uint8_t {
    Halt,
  };

  enum class Memory : uint8_t {
    Move,
  };

  enum class Bitwise : uint8_t {
    And,
    Or,
    Xor,
    Not,
  };

  enum class Comparison : uint8_t {
    Equals,
    Less,
    Greater,
  };

  enum class Arithmetic : uint8_t {
    Negate,
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
  };

  Instruction() = default;
  Instruction(Kind kind, uint16_t code, Operand A = {}, Operand B = {},
              Operand C = {})
      : kind(std::to_underlying(kind)), code(code), A_kind(A.kind),
        B_kind(B.kind), C_kind(C.kind), A_data(A.data), B_data(B.data),
        C_data(C.data) {}

  Operand A() const { return {A_kind, A_data}; }
  Operand B() const { return {B_kind, B_data}; }
  Operand C() const { return {C_kind, C_data}; }

  static Instruction Halt() { return {Kind::Control, Control::Halt}; }

  static Instruction Move(Operand destination, Operand source) {
    return {Kind::Memory, Memory::Move, destination, source};
  }

  static Instruction And(Operand destination, Operand left, Operand right) {
    return {Kind::Bitwise, Bitwise::And, destination, left, right};
  }

  static Instruction Or(Operand destination, Operand left, Operand right) {
    return {Kind::Bitwise, Bitwise::Or, destination, left, right};
  }

  static Instruction Xor(Operand destination, Operand left, Operand right) {
    return {Kind::Bitwise, Bitwise::Xor, destination, left, right};
  }

  static Instruction Not(Operand destination, Operand source) {
    return {Kind::Bitwise, Bitwise::Not, destination, source};
  }

  static Instruction Equals(Operand destination, Operand left, Operand right) {
    return {Kind::Comparison, Comparison::Equals, destination, left, right};
  }

  static Instruction Less(Operand destination, Operand left, Operand right) {
    return {Kind::Comparison, Comparison::Less, destination, left, right};
  }

  static Instruction Greater(Operand destination, Operand left, Operand right) {
    return {Kind::Comparison, Comparison::Greater, destination, left, right};
  }

  static Instruction Negate(Operand destination, Operand source) {
    return {Kind::Arithmetic, Arithmetic::Negate, destination, source};
  }

  static Instruction Add(Operand destination, Operand left, Operand right) {
    return {Kind::Arithmetic, Arithmetic::Add, destination, left, right};
  }

  static Instruction Subtract(Operand destination, Operand left, Operand right) {
    return {Kind::Arithmetic, Arithmetic::Subtract, destination, left, right};
  }

  static Instruction Multiply(Operand destination, Operand left, Operand right) {
    return {Kind::Arithmetic, Arithmetic::Multiply, destination, left, right};
  }

  static Instruction Divide(Operand destination, Operand left, Operand right) {
    return {Kind::Arithmetic, Arithmetic::Divide, destination, left, right};
  }

  static Instruction Modulo(Operand destination, Operand left, Operand right) {
    return {Kind::Arithmetic, Arithmetic::Modulo, destination, left, right};
  }
};

} // namespace tbc


template <> struct std::formatter<tbc::Instruction> {
  template <class ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <class FormatContext>
  constexpr auto format(const FormatContext &ctx,
                        tbc::Instruction instruction) {
    if (!instruction.is_valid()) {
      return std::format_to(ctx.out(), "Invalid instruction");
    }

    switch (static_cast<tbc::Instruction::Kind>(instruction.kind)) {
    case tbc::Instruction::Kind::Control: {
      switch (static_cast<tbc::Instruction::Control>(instruction.code)) {
      case tbc::Instruction::Control::Return:
        return std::format_to(ctx.out(), "Return {}", instruction.A());
      case tbc::Instruction::Control::Jump:
        return std::format_to(ctx.out(), "Jump {}", instruction.A());
      }
    }

    case tbc::Instruction::Kind::Memory: {
      switch (static_cast<tbc::Instruction::Memory>(instruction.code)) {
      case tbc::Instruction::Memory::Move:
        return std::format_to(ctx.out(), "Move {}, {}", instruction.A(),
                              instruction.B());
      }
    }

    case tbc::Instruction::Kind::Bitwise: {
      switch (static_cast<tbc::Instruction::Bitwise>(instruction.code)) {
      case tbc::Instruction::Bitwise::And:
        return std::format_to(ctx.out(), "And {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());

      case tbc::Instruction::Bitwise::Or:
        return std::format_to(ctx.out(), "Or {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());

      case tbc::Instruction::Bitwise::Xor:
        return std::format_to(ctx.out(), "Xor {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());

      case tbc::Instruction::Bitwise::Not:
        return std::format_to(ctx.out(), "Not {}, {}", instruction.A(),
                              instruction.B());
      }
    }

    case tbc::Instruction::Kind::Comparison: {
      switch (static_cast<tbc::Instruction::Comparison>(instruction.code)) {
      case tbc::Instruction::Comparison::Equals:
        return std::format_to(ctx.out(), "Equals {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());

      case tbc::Instruction::Comparison::Less:
        return std::format_to(ctx.out(), "Less {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());

      case tbc::Instruction::Comparison::Greater:
        return std::format_to(ctx.out(), "Greater {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
      }
    }

    case tbc::Instruction::Kind::Arithmetic: {
      switch (static_cast<tbc::Instruction::Arithmetic>(instruction.code)) {
      case tbc::Instruction::Arithmetic::Negate:
        return std::format_to(ctx.out(), "Negate {}, {}", instruction.A(),
                              instruction.B());
      case tbc::Instruction::Arithmetic::Add:
        return std::format_to(ctx.out(), "Add {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
      case tbc::Instruction::Arithmetic::Subtract:
        return std::format_to(ctx.out(), "Subtract {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
      case tbc::Instruction::Arithmetic::Multiply:
        return std::format_to(ctx.out(), "Multiply {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
      case tbc::Instruction::Arithmetic::Divide:
        return std::format_to(ctx.out(), "Divide {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
      case tbc::Instruction::Arithmetic::Modulo:
        return std::format_to(ctx.out(), "Modulo {}, {}, {}", instruction.A(),
                              instruction.B(), instruction.C());
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

#endif // !TBC_IMR_INSTRUCTION_HPP
