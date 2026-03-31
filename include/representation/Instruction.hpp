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

#include "representation/Operand.hpp"

namespace tbc {
struct Instruction {
  uint16_t m_kind : 3;
  uint16_t m_code : 4;
  uint16_t a_kind : 3;
  uint16_t b_kind : 3;
  uint16_t c_kind : 3;
  uint16_t a_data;
  uint16_t b_data;
  uint16_t c_data;

  enum class Kind : uint8_t {
    control = 0b000,
    memory = 0b001,
    bitwise = 0b010,
    comparison = 0b011,
    integral = 0b100,
  };

  enum class Control : uint8_t {
    ret,
    // jal,
    // b,
    // beq,
    // bne,
    // blt, reverse operands -> bgt
    // bge, reverse operands -> ble
    // bltu, reverse operands -> bgtu
    // bgeu, reverse operands -> bleu
  };

  enum class Memory : uint8_t {
    // given the permutations of operands, move can handle
    // register to register moves, register to memory moves, and memory to
    // register moves. immediate to register and immediate to memory.
    // since move is always two operands, we can assume that b and c can be
    // combined
    // into a single extended operand.
    mv,
    mvu, // move upper, the same as move except shifts the extended operand left
         // by 32.
  };

  enum class Bitwise : uint8_t {
    and_,
    or_,
    xor_,
    not_,
    sl,
    sr,
    // rol,
    // ror,
  };

  enum class Comparison : uint8_t {
    eq,
    neq,
    lt,   // reverse operands -> gt
    lte,  // reverse operands -> gte
    slt,  // reverse operands -> sgt
    slte, // reverse operands -> sgte
  };

  enum class Integral : uint8_t {
    neg,
    add,
    sub,
    mul,
    div,
    mod,
    // min
    // max
    // abs
    // clamp(a, b, c) = min(max(a, b), c)
  };

  // enum class Floating : uint8_t {
  //   fneg,
  //   fadd,
  //   fsub,
  //   fmul,
  //   fdiv,
  //   fmod,
  // };

  Instruction() = default;
  Instruction(Control code, Operand a, Operand b, Operand c)
      : m_kind(std::to_underlying(Kind::control)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(c.m_kind), a_data(a.m_data), b_data(b.m_data), c_data(c.m_data) {
  }

  Instruction(Control code, Operand a, Operand b)
      : m_kind(std::to_underlying(Kind::control)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(0), a_data(a.m_data), b_data(b.m_data), c_data(0) {}

  Instruction(Control code, Operand a)
      : m_kind(std::to_underlying(Kind::control)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(0),
        c_kind(0), a_data(a.m_data), b_data(0), c_data(0) {}

  // Instruction(Control code, Operand a, OperandExtended bc)
  //     : m_kind(std::to_underlying(Kind::control)),
  //       m_code(std::to_underlying(code)), a_kind(a.m_kind),
  //       b_kind(bc.m_kind), c_kind(0), a_data(a.m_data), b_data(bc.upper()),
  //       c_data(bc.lower()) {}

  // Instruction(Control code, OperandExtended ab)
  //     : m_kind(std::to_underlying(Kind::control)),
  //       m_code(std::to_underlying(code)), a_kind(ab.m_kind), b_kind(0),
  //       c_kind(0), a_data(ab.upper()), b_data(ab.lower()), c_data(0) {}

  Instruction(Memory code, Operand a, Operand b, Operand c)
      : m_kind(std::to_underlying(Kind::memory)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(c.m_kind), a_data(a.m_data), b_data(b.m_data), c_data(c.m_data) {
  }

  Instruction(Memory code, Operand a, Operand b)
      : m_kind(std::to_underlying(Kind::memory)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(0), a_data(a.m_data), b_data(b.m_data), c_data(0) {}

  Instruction(Memory code, Operand a)
      : m_kind(std::to_underlying(Kind::memory)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(0),
        c_kind(0), a_data(a.m_data), b_data(0), c_data(0) {}

  // Instruction(Memory code, Operand destination, OperandExtended source)
  //     : m_kind(std::to_underlying(Kind::memory)),
  //       m_code(std::to_underlying(code)), a_kind(destination.m_kind),
  //       b_kind(source.m_kind), c_kind(0), a_data(destination.m_data),
  //       b_data(source.upper()), c_data(source.lower()) {}

  Instruction(Bitwise code, Operand a, Operand b, Operand c)
      : m_kind(std::to_underlying(Kind::bitwise)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(c.m_kind), a_data(a.m_data), b_data(b.m_data), c_data(c.m_data) {
  }

  Instruction(Bitwise code, Operand a, Operand b)
      : m_kind(std::to_underlying(Kind::bitwise)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(0), a_data(a.m_data), b_data(b.m_data), c_data(0) {}

  // Instruction(Bitwise code, Operand a, OperandExtended bc)
  //     : m_kind(std::to_underlying(Kind::bitwise)),
  //       m_code(std::to_underlying(code)), a_kind(a.m_kind),
  //       b_kind(bc.m_kind), c_kind(0), a_data(a.m_data), b_data(bc.upper()),
  //       c_data(bc.lower()) {}

  Instruction(Comparison code, Operand a, Operand b, Operand c)
      : m_kind(std::to_underlying(Kind::comparison)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(c.m_kind), a_data(a.m_data), b_data(b.m_data), c_data(c.m_data) {
  }

  Instruction(Comparison code, Operand a, Operand b)
      : m_kind(std::to_underlying(Kind::comparison)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(0), a_data(a.m_data), b_data(b.m_data), c_data(0) {}

  Instruction(Integral code, Operand a, Operand b, Operand c)
      : m_kind(std::to_underlying(Kind::integral)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(c.m_kind), a_data(a.m_data), b_data(b.m_data), c_data(c.m_data) {
  }

  Instruction(Integral code, Operand a, Operand b)
      : m_kind(std::to_underlying(Kind::integral)),
        m_code(std::to_underlying(code)), a_kind(a.m_kind), b_kind(b.m_kind),
        c_kind(0), a_data(a.m_data), b_data(b.m_data), c_data(0) {}

  // Instruction(Integral code, Operand a, OperandExtended bc)
  //     : m_kind(std::to_underlying(Kind::integral)),
  //       m_code(std::to_underlying(code)), a_kind(a.m_kind),
  //       b_kind(bc.m_kind), c_kind(0), a_data(a.m_data), b_data(bc.upper()),
  //       c_data(bc.lower()) {}

  Kind kind() const { return static_cast<Kind>(m_kind); }
  template <class T> T code() const { return static_cast<T>(m_code); }
  Operand a() const { return {a_kind, a_data}; }
  Operand b() const { return {b_kind, b_data}; }
  Operand c() const { return {c_kind, c_data}; }
  // OperandExtended bc() const {
  //   return OperandExtended{b_kind,
  //           static_cast<uint32_t>(b_data) << 16 |
  //           static_cast<uint32_t>(c_data)};
  // }

  static Instruction ret(Operand value) { return {Control::ret, value}; }

  static Instruction mv(Operand destination, Operand source) {
    return {Memory::mv, destination, source};
  }

  static Instruction mvu(Operand destination, Operand source) {
    return {Memory::mvu, destination, source};
  }

  static Instruction and_(Operand destination, Operand left, Operand right) {
    return {Bitwise::and_, destination, left, right};
  }

  static Instruction or_(Operand destination, Operand left, Operand right) {
    return {Bitwise::or_, destination, left, right};
  }

  static Instruction xor_(Operand destination, Operand left, Operand right) {
    return {Bitwise::xor_, destination, left, right};
  }

  static Instruction not_(Operand destination, Operand source) {
    return {Bitwise::not_, destination, source};
  }

  static Instruction sl(Operand destination, Operand left, Operand right) {
    return {Bitwise::sl, destination, left, right};
  }

  static Instruction sr(Operand destination, Operand left, Operand right) {
    return {Bitwise::sr, destination, left, right};
  }

  static Instruction eq(Operand destination, Operand left, Operand right) {
    return {Comparison::eq, destination, left, right};
  }

  static Instruction neq(Operand destination, Operand left, Operand right) {
    return {Comparison::neq, destination, left, right};
  }

  static Instruction lt(Operand destination, Operand left, Operand right) {
    return {Comparison::lt, destination, left, right};
  }

  static Instruction lte(Operand destination, Operand left, Operand right) {
    return {Comparison::lte, destination, left, right};
  }

  static Instruction gt(Operand destination, Operand left, Operand right) {
    return {Comparison::lt, destination, right, left};
  }

  static Instruction gte(Operand destination, Operand left, Operand right) {
    return {Comparison::lte, destination, right, left};
  }

  static Instruction slt(Operand destination, Operand left, Operand right) {
    return {Comparison::slt, destination, left, right};
  }

  static Instruction slte(Operand destination, Operand left, Operand right) {
    return {Comparison::slte, destination, left, right};
  }

  static Instruction sgt(Operand destination, Operand left, Operand right) {
    return {Comparison::slt, destination, right, left};
  }

  static Instruction sgte(Operand destination, Operand left, Operand right) {
    return {Comparison::slte, destination, right, left};
  }

  static Instruction neg(Operand destination, Operand source) {
    return {Integral::neg, destination, source};
  }

  static Instruction add(Operand destination, Operand left, Operand right) {
    return {Integral::add, destination, left, right};
  }

  static Instruction sub(Operand destination, Operand left, Operand right) {
    return {Integral::sub, destination, left, right};
  }

  static Instruction mul(Operand destination, Operand left, Operand right) {
    return {Integral::mul, destination, left, right};
  }

  static Instruction div(Operand destination, Operand left, Operand right) {
    return {Integral::div, destination, left, right};
  }

  static Instruction mod(Operand destination, Operand left, Operand right) {
    return {Integral::mod, destination, left, right};
  }
};

} // namespace tbc

template <> struct std::formatter<tbc::Instruction::Kind> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Kind m_kind, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (m_kind) {
    case tbc::Instruction::Kind::control:
      return std::format_to(ctx.out(), "control");
    case tbc::Instruction::Kind::memory:
      return std::format_to(ctx.out(), "memory");
    case tbc::Instruction::Kind::bitwise:
      return std::format_to(ctx.out(), "bitwise");
    case tbc::Instruction::Kind::comparison:
      return std::format_to(ctx.out(), "comparison");
    case tbc::Instruction::Kind::integral:
      return std::format_to(ctx.out(), "integral");
    default:
      return std::format_to(ctx.out(), "unknown: {}",
                            std::to_underlying(m_kind));
    }
  }
};

template <> struct std::formatter<tbc::Instruction::Control> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Control control, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (control) {
    case tbc::Instruction::Control::ret:
      return std::format_to(ctx.out(), "ret");
    default:
      return std::format_to(ctx.out(), "unknown: {}",
                            std::to_underlying(control));
    }
  }
};

template <> struct std::formatter<tbc::Instruction::Memory> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Memory memory, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (memory) {
    case tbc::Instruction::Memory::mv:
      return std::format_to(ctx.out(), "mv");
    case tbc::Instruction::Memory::mvu:
      return std::format_to(ctx.out(), "mvu");
    default:
      return std::format_to(ctx.out(), "unknown: {}",
                            std::to_underlying(memory));
    }
  }
};

template <> struct std::formatter<tbc::Instruction::Bitwise> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Bitwise bitwise, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (bitwise) {
    case tbc::Instruction::Bitwise::and_:
      return std::format_to(ctx.out(), "and");
    case tbc::Instruction::Bitwise::or_:
      return std::format_to(ctx.out(), "or");
    case tbc::Instruction::Bitwise::xor_:
      return std::format_to(ctx.out(), "xor");
    case tbc::Instruction::Bitwise::not_:
      return std::format_to(ctx.out(), "not");
    case tbc::Instruction::Bitwise::sl:
      return std::format_to(ctx.out(), "sl");
    case tbc::Instruction::Bitwise::sr:
      return std::format_to(ctx.out(), "sr");
    default:
      return std::format_to(ctx.out(), "unknown: {}",
                            std::to_underlying(bitwise));
    }
  }
};

template <> struct std::formatter<tbc::Instruction::Comparison> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Comparison comparison, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (comparison) {
    case tbc::Instruction::Comparison::eq:
      return std::format_to(ctx.out(), "eq");
    case tbc::Instruction::Comparison::lt:
      return std::format_to(ctx.out(), "lt");
    case tbc::Instruction::Comparison::lte:
      return std::format_to(ctx.out(), "lte");
    case tbc::Instruction::Comparison::slt:
      return std::format_to(ctx.out(), "slt");
    case tbc::Instruction::Comparison::slte:
      return std::format_to(ctx.out(), "slte");
    default:
      return std::format_to(ctx.out(), "unknown: {}",
                            std::to_underlying(comparison));
    }
  }
};

template <> struct std::formatter<tbc::Instruction::Integral> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(tbc::Instruction::Integral code, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (code) {
    case tbc::Instruction::Integral::neg:
      return std::format_to(ctx.out(), "neg");
    case tbc::Instruction::Integral::add:
      return std::format_to(ctx.out(), "add");
    case tbc::Instruction::Integral::sub:
      return std::format_to(ctx.out(), "sub");
    case tbc::Instruction::Integral::mul:
      return std::format_to(ctx.out(), "mul");
    case tbc::Instruction::Integral::div:
      return std::format_to(ctx.out(), "div");
    case tbc::Instruction::Integral::mod:
      return std::format_to(ctx.out(), "mod");
    default:
      return std::format_to(ctx.out(), "unknown: {}", std::to_underlying(code));
    }
  }
};

template <> struct std::formatter<tbc::Instruction> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) const -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(tbc::Instruction instruction, FormatContext &ctx) const
      -> FormatContext::iterator {
    switch (static_cast<tbc::Instruction::Kind>(instruction.m_kind)) {
    case tbc::Instruction::Kind::control: {
      switch (static_cast<tbc::Instruction::Control>(instruction.m_code)) {
      case tbc::Instruction::Control::ret:
        return std::format_to(ctx.out(), "ret {}", instruction.a());
      default:
        return std::format_to(ctx.out(), "unknown: {}",
                              static_cast<uint16_t>(instruction.m_code));
      }
    }

    case tbc::Instruction::Kind::memory: {
      switch (static_cast<tbc::Instruction::Memory>(instruction.m_code)) {
      case tbc::Instruction::Memory::mv:
        return std::format_to(ctx.out(), "mv {}, {}", instruction.a(),
                              instruction.b());
      case tbc::Instruction::Memory::mvu:
        return std::format_to(ctx.out(), "mvu {}, {}", instruction.a(),
                              instruction.b());
      default:
        return std::format_to(ctx.out(), "unknown: {}",
                              static_cast<uint16_t>(instruction.m_code));
      }
    }

    case tbc::Instruction::Kind::bitwise: {
      switch (static_cast<tbc::Instruction::Bitwise>(instruction.m_code)) {
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

      case tbc::Instruction::Bitwise::sl:
        return std::format_to(ctx.out(), "sl {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Bitwise::sr:
        return std::format_to(ctx.out(), "sr {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      default:
        return std::format_to(ctx.out(), "unknown: {}",
                              static_cast<uint16_t>(instruction.m_code));
      }
    }

    case tbc::Instruction::Kind::comparison: {
      switch (static_cast<tbc::Instruction::Comparison>(instruction.m_code)) {
      case tbc::Instruction::Comparison::eq:
        return std::format_to(ctx.out(), "eq {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::neq:
        return std::format_to(ctx.out(), "neq {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::lt:
        return std::format_to(ctx.out(), "lt {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::lte:
        return std::format_to(ctx.out(), "lte {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::slt:
        return std::format_to(ctx.out(), "slt {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      case tbc::Instruction::Comparison::slte:
        return std::format_to(ctx.out(), "slte {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      default:
        return std::format_to(ctx.out(), "unknown: {}",
                              static_cast<uint16_t>(instruction.m_code));
      }
    }

    case tbc::Instruction::Kind::integral: {
      switch (static_cast<tbc::Instruction::Integral>(instruction.m_code)) {
      case tbc::Instruction::Integral::neg:
        return std::format_to(ctx.out(), "neg {}, {}", instruction.a(),
                              instruction.b());
      case tbc::Instruction::Integral::add:
        return std::format_to(ctx.out(), "add {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Integral::sub:
        return std::format_to(ctx.out(), "sub {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Integral::mul:
        return std::format_to(ctx.out(), "mul {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Integral::div:
        return std::format_to(ctx.out(), "div {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());
      case tbc::Instruction::Integral::mod:
        return std::format_to(ctx.out(), "mod {}, {}, {}", instruction.a(),
                              instruction.b(), instruction.c());

      default:
        return std::format_to(ctx.out(), "unknown: {}",
                              static_cast<uint16_t>(instruction.m_code));
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
