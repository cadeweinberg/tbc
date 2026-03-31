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

#include <utility>

#include "algorithm/Execute.hpp"
#include "utility/Arithmetic.hpp"

namespace tbc {
static leaf::result<void> executeExpression(Expression::Ptr expression,
                                            Context::Ptr context);
static leaf::result<void> executeInstruction(Instruction instruction,
                                             Context::Ptr context);
static leaf::result<void> executeControl(Instruction instruction,
                                         Context::Ptr context);
static leaf::result<void> executeMemory(Instruction instruction,
                                        Context::Ptr context);
static leaf::result<void> executeBitwise(Instruction instruction,
                                         Context::Ptr context);
static leaf::result<void> executeComparison(Instruction instruction,
                                            Context::Ptr context);
static leaf::result<void> executeIntegral(Instruction instruction,
                                          Context::Ptr context);

static leaf::result<void> executeRet(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeMv(Instruction instruction,
                                    Context::Ptr context);
static leaf::result<void> executeMvu(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeAnd(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeOr(Instruction instruction,
                                    Context::Ptr context);
static leaf::result<void> executeXor(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeNot(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSll(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSrl(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSra(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeEq(Instruction instruction,
                                    Context::Ptr context);
static leaf::result<void> executeNeq(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeLt(Instruction instruction,
                                    Context::Ptr context);
static leaf::result<void> executeLte(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSlt(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSlte(Instruction instruction,
                                      Context::Ptr context);
static leaf::result<void> executeNeg(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeAdd(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSub(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeMul(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeDiv(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeMod(Instruction instruction,
                                     Context::Ptr context);

leaf::result<void> execute(Context::Ptr context) {
  return executeExpression(context->getExpression(), context);
}

static leaf::result<void> executeExpression(Expression::Ptr expression,
                                            Context::Ptr context) {
  for (Instruction instruction : *expression) {
    BOOST_LEAF_CHECK(executeInstruction(instruction, context));
  }
  return {};
}

static leaf::result<void> executeInstruction(Instruction instruction,
                                             Context::Ptr context) {
  switch (instruction.kind()) {
  case Instruction::Kind::control:
    return executeControl(instruction, context);
  case Instruction::Kind::memory:
    return executeMemory(instruction, context);
  case Instruction::Kind::bitwise:
    return executeBitwise(instruction, context);
  case Instruction::Kind::comparison:
    return executeComparison(instruction, context);
  case Instruction::Kind::integral:
    return executeIntegral(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid instruction kind: {}", instruction.kind()));
  }
}

static leaf::result<void> executeControl(Instruction instruction,
                                         Context::Ptr context) {
  switch (instruction.code<Instruction::Control>()) {
  case Instruction::Control::ret:
    return executeRet(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid control instruction code: {}",
                    instruction.code<Instruction::Control>()));
  }
}

static leaf::result<void> executeMemory(Instruction instruction,
                                        Context::Ptr context) {
  switch (instruction.code<Instruction::Memory>()) {
  case Instruction::Memory::mv:
    return executeMv(instruction, context);
  case Instruction::Memory::mvu:
    return executeMvu(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid memory instruction code: {}",
                    instruction.code<Instruction::Memory>()));
  }
}

static leaf::result<void> executeBitwise(Instruction instruction,
                                         Context::Ptr context) {
  switch (instruction.code<Instruction::Bitwise>()) {
  case Instruction::Bitwise::and_:
    return executeAnd(instruction, context);
  case Instruction::Bitwise::or_:
    return executeOr(instruction, context);
  case Instruction::Bitwise::xor_:
    return executeXor(instruction, context);
  case Instruction::Bitwise::not_:
    return executeNot(instruction, context);
  case Instruction::Bitwise::sll:
    return executeSll(instruction, context);
  case Instruction::Bitwise::srl:
    return executeSrl(instruction, context);
  case Instruction::Bitwise::sra:
    return executeSra(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid bitwise instruction code: {}",
                    instruction.code<Instruction::Bitwise>()));
  }
}

static leaf::result<void> executeComparison(Instruction instruction,
                                            Context::Ptr context) {
  switch (instruction.code<Instruction::Comparison>()) {
  case Instruction::Comparison::eq:
    return executeEq(instruction, context);
  case Instruction::Comparison::neq:
    return executeNeq(instruction, context);
  case Instruction::Comparison::lt:
    return executeLt(instruction, context);
  case Instruction::Comparison::lte:
    return executeLte(instruction, context);
  case Instruction::Comparison::slt:
    return executeSlt(instruction, context);
  case Instruction::Comparison::slte:
    return executeSlte(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid comparison instruction code: {}",
                    instruction.code<Instruction::Comparison>()));
  }
}

static leaf::result<void> executeIntegral(Instruction instruction,
                                          Context::Ptr context) {
  switch (instruction.code<Instruction::Integral>()) {
  case Instruction::Integral::neg:
    return executeNeg(instruction, context);
  case Instruction::Integral::add:
    return executeAdd(instruction, context);
  case Instruction::Integral::sub:
    return executeSub(instruction, context);
  case Instruction::Integral::mul:
    return executeMul(instruction, context);
  case Instruction::Integral::div:
    return executeDiv(instruction, context);
  case Instruction::Integral::mod:
    return executeMod(instruction, context);
  default:
    return leaf::new_error(
        std::format("Invalid arithmetic instruction code: {}",
                    instruction.code<Instruction::Integral>()));
  }
}

static leaf::result<void> executeRet(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  switch (a.kind()) {
  case Operand::Kind::reg:
    context->setResult(Value::create(context->registerAs<int64_t>(a.as<Index>())));
    break;
  case Operand::Kind::i16:
    context->setResult(Value::create(a.as<int64_t>()));
    break;
  case Operand::Kind::u16:
    context->setResult(Value::create(a.as<uint64_t>()));
    break;
  default:
    return leaf::new_error(std::format(
        "Invalid operand kind in return instruction: {}", a.kind()));
  }
  return {};
}

static leaf::result<void> executeMv(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in move instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();

  switch (source.kind()) {
  case Operand::Kind::reg:
    destination = context->registerAt(source.as<Index>());
    break;
  case Operand::Kind::i16:
    destination = source.as<int64_t>();
    break;
  case Operand::Kind::u16:
    destination = source.as<uint64_t>();
    break;
  default:
    return leaf::new_error(std::format(
        "Invalid source operand kind in move instruction: {}", source.kind()));
  }
  return {};
}

static leaf::result<void> executeMvu(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in move upper instruction: {}",
        a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  switch (source.kind()) {
  case Operand::Kind::reg:
    destination = context->registerAt(source.as<Index>()) << 32;
    break;
  case Operand::Kind::i16:
    destination = source.as<int64_t>() << 32;
    break;
  case Operand::Kind::u16:
    destination = source.as<uint64_t>() << 32;
    break;
  default:
    return leaf::new_error(
        std::format("Invalid source operand kind in move upper instruction: {}",
                    source.kind()));
  }
  return {};
}

static leaf::result<void> executeAnd(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in and instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();

  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left & context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left & c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left & c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in and instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left & context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left & c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left & c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in and instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left & context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left & c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left & c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in and instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in and instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeOr(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in or instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();

  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left | context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left | c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left | c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in or instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left | context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left | c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left | c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in or instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left | context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left | c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left | c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in or instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in or instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeXor(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in xor instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();

  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left ^ context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left ^ c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left ^ c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in xor instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left ^ context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left ^ c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left ^ c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in xor instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = left ^ context->registerAt(c.as<Index>());
      break;
    case Operand::Kind::i16:
      destination = left ^ c.as<int64_t>();
      break;
    case Operand::Kind::u16:
      destination = left ^ c.as<uint64_t>();
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in xor instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in xor instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeNot(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in xor instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());

  Operand source = instruction.b();
  switch (source.kind()) {
  case Operand::Kind::reg:
    destination = ~(context->registerAt(source.as<Index>()));
    break;
  case Operand::Kind::i16:
    destination = ~(source.as<int64_t>());
    break;
  case Operand::Kind::u16:
    destination = ~(source.as<uint64_t>());
    break;
  default:
    return leaf::new_error(std::format(
        "Invalid source operand kind in not instruction: {}", source.kind()));
  }

  return {};
}

static leaf::result<void> executeSll(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in sll instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  Operand shift = instruction.c();

  switch (shift.kind()) {
  case Operand::Kind::reg: {
    Register &shiftValue = context->registerAt(shift.as<Index>());
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in sll instruction: {}", shiftValue));
    }

    switch (source.kind()) {
    case Operand::Kind::reg:
      destination = context->registerAt(source.as<Index>()) << shiftValue;
      break;
    case Operand::Kind::i16:
      destination = source.as<int64_t>() << shiftValue;
      break;
    case Operand::Kind::u16:
      destination = source.as<uint64_t>() << shiftValue;
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sll instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t shiftValue = shift.as<int64_t>();
    if (shiftValue < 0 || shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Invalid shift value in sll instruction: {}", shiftValue));
    }

    switch (source.kind()) {
    case Operand::Kind::reg:
      destination = context->registerAt(source.as<Index>()) << shiftValue;
      break;
    case Operand::Kind::i16:
      destination = source.as<int64_t>() << shiftValue;
      break;
    case Operand::Kind::u16:
      destination = source.as<uint64_t>() << shiftValue;
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sll instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::u16: {
    uint64_t shiftValue = shift.as<uint64_t>();
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in sll instruction: {}", shiftValue));
    }

    switch (source.kind()) {
    case Operand::Kind::reg:
      destination = context->registerAt(source.as<Index>()) << shiftValue;
      break;
    case Operand::Kind::i16:
      destination = source.as<int64_t>() << shiftValue;
      break;
    case Operand::Kind::u16:
      destination = source.as<uint64_t>() << shiftValue;
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sll instruction: {}", source.kind()));
    }
    break;
  }

  default:
    return leaf::new_error(std::format(
        "Invalid shift operand kind in sll instruction: {}", shift.kind()));
  }
  return {};
}

static leaf::result<void> executeSrl(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in srl instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  Operand shift = instruction.c();

  switch (shift.kind()) {
  case Operand::Kind::reg: {
    Register &shiftValue = context->registerAt(shift.as<Index>());
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in srl instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(shifted,
                      srl(context->registerAt(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in srl instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t shiftValue = shift.as<int64_t>();
    if (shiftValue < 0 || shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Invalid shift value in srl instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(shifted,
                      srl(context->registerAt(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in srl instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::u16: {
    uint64_t shiftValue = shift.as<uint64_t>();
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in srl instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(shifted,
                      srl(context->registerAt(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<uint64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in srl instruction: {}", source.kind()));
    }
    break;
  }

  default:
    return leaf::new_error(std::format(
        "Invalid shift operand kind in srl instruction: {}", shift.kind()));
  }
  return {};
}

static leaf::result<void> executeSra(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in sra instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  Operand shift = instruction.c();

  switch (shift.kind()) {
  case Operand::Kind::reg: {
    Register &shiftValue = context->registerAt(shift.as<Index>());
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in sra instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(
          shifted,
          srl(context->registerAs<int64_t>(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, srl(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sra instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t shiftValue = shift.as<int64_t>();
    if (shiftValue < 0 || shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Invalid shift value in sra instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(
          shifted,
          sra(context->registerAs<int64_t>(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, sra(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, sra(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sra instruction: {}", source.kind()));
    }
    break;
  }

  case Operand::Kind::u16: {
    uint64_t shiftValue = shift.as<uint64_t>();
    if (shiftValue >= 64) {
      return leaf::new_error(std::format(
          "Shift value too large in sra instruction: {}", shiftValue));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
      BOOST_LEAF_AUTO(
          shifted,
          sra(context->registerAs<int64_t>(source.as<Index>()), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::i16: {
      BOOST_LEAF_AUTO(shifted, sra(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    case Operand::Kind::u16: {
      BOOST_LEAF_AUTO(shifted, sra(source.as<int64_t>(), shiftValue));
      destination = shifted;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid source operand kind in sra instruction: {}", source.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid shift operand kind in sra instruction: {}", shift.kind()));
  }
  return {};
}

static leaf::result<void> executeEq(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in xor instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());

  Operand b = instruction.b();
  Operand c = instruction.c();

  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left == context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left == c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left == c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in equals instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left == context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left == c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left == c.as<uint64_t>());
      break;
    }
    break;
  }

  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left == context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left == c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left == c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in equals instruction: {}", c.kind()));
    }
    break;
  }
  default:
    break;
  }
  return {};
}

static leaf::result<void> executeNeq(Instruction instruction,
                                     Context::Ptr context) {
  // see comments in executeEq, the same reasoning applies here, except that
  // we return 0 for true and 1 for false since this is not equals.
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in neq instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left != context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left != c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left != c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in neq instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left != context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left != c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left != c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in neq instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left != context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
      destination = (left != c.as<int64_t>());
      break;
    case Operand::Kind::u16:
      destination = (left != c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in neq instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in neq instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeLt(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in lt instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());

  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left < context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left < c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in lt instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left < context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left < c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in lt instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in lt instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeLte(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in lte instruction: {}", a.kind()));
  }
  Register &destination = context->registerAt(a.as<Index>());

  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    Register &left = context->registerAt(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left <= context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left <= c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in lte instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    uint64_t left = b.as<uint64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left <= context->registerAt(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left <= c.as<uint64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in lte instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in lte instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeSlt(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in slt instruction: {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());

  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left < context->registerAs<int64_t>(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left < c.as<int64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in slt instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left < context->registerAs<int64_t>(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left < c.as<int64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in slt instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in slt instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeSlte(Instruction instruction,
                                      Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(std::format(
        "Invalid destination operand kind in slte instruction: {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());

  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left <= context->registerAs<int64_t>(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left <= c.as<int64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in slte instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg:
      destination = (left <= context->registerAs<int64_t>(c.as<Index>()));
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = (left <= c.as<int64_t>());
      break;
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in slte instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in slte instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeNeg(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in neg instruction", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  Operand source = instruction.b();
  switch (source.kind()) {
  case Operand::Kind::reg:
    destination = -(context->registerAs<int64_t>(source.as<Index>()));
    break;
  case Operand::Kind::i16:
  case Operand::Kind::u16:
    destination = -(source.as<int64_t>());
    break;
  default:
    return leaf::new_error(std::format(
        "Invalid source operand kind in neg instruction: {}", source.kind()));
  }
  return {};
}

static leaf::result<void> executeAdd(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in add instruction {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  // two's compliment means that we can use the same add instruction
  // for signed and unsigned values, and the results will be correct.
  // we still need to account for overflow, this instruction is implementing
  // addition arithmetically, so we want to error on overflow.
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, add(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, add(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in add instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, add(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, add(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in add instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in add instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeSub(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in sub instruction {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, sub(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, sub(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in sub instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, sub(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, sub(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in sub instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format(
        "Invalid left operand kind in sub instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeMul(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in mul instruction {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, mul(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, mul(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in mul instruction: {}", c.kind()));
    }
    break;
  }

  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, mul(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, mul(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format("Invalid right operand kind in "
                                         "mul instruction: {}",
                                         c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(std::format("Invalid left operand kind in mul "
                                       "instruction: {}",
                                       b.kind()));
  }
  return {};
}

static leaf::result<void> executeDiv(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in div instruction {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, div(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, div(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in div instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, div(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, div(left, right));
      destination = result;
      break;
    }
    default: return leaf::new_error(std::format(
          "Invalid right operand kind in div instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(
        std::format("Invalid left operand in div instruction: {}", b.kind()));
  }
  return {};
}

static leaf::result<void> executeMod(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(
        std::format("Invalid destination in mod instruction {}", a.kind()));
  }
  int64_t &destination = context->registerAs<int64_t>(a.as<Index>());
  Operand b = instruction.b();
  Operand c = instruction.c();
  switch (b.kind()) {
  case Operand::Kind::reg: {
    int64_t &left = context->registerAs<int64_t>(b.as<Index>());
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, mod(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, mod(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in mod instruction: {}", c.kind()));
    }
    break;
  }
  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t left = b.as<int64_t>();
    switch (c.kind()) {
    case Operand::Kind::reg: {
      int64_t &right = context->registerAs<int64_t>(c.as<Index>());
      BOOST_LEAF_AUTO(result, mod(left, right));
      destination = result;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
      int64_t right = c.as<int64_t>();
      BOOST_LEAF_AUTO(result, mod(left, right));
      destination = result;
      break;
    }
    default:
      return leaf::new_error(std::format(
          "Invalid right operand kind in mod instruction: {}", c.kind()));
    }
    break;
  }
  default:
    return leaf::new_error(
        std::format("Invalid left operand in mod instruction: {}", b.kind()));
  }
  return {};
}

} // namespace tbc
