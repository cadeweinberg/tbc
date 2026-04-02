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
static leaf::result<void> executeSl(Instruction instruction,
                                     Context::Ptr context);
static leaf::result<void> executeSr(Instruction instruction,
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
    return leaf::new_error(Error::create(instruction));
  }
}

static leaf::result<void> executeControl(Instruction instruction,
                                         Context::Ptr context) {
  switch (instruction.code<Instruction::Control>()) {
  case Instruction::Control::ret:
    return executeRet(instruction, context);
  default:
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
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
  case Instruction::Bitwise::sl:
    return executeSl(instruction, context);
  case Instruction::Bitwise::sr:
    return executeSr(instruction, context);
  default:
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeMv(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeMvu(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeAnd(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeOr(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeXor(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeNot(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
  }

  return {};
}

static leaf::result<void> executeSl(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  Operand shift = instruction.c();

  switch (shift.kind()) {
  case Operand::Kind::reg: {
    Register &shiftValue = context->registerAt(shift.as<Index>());
    if (shiftValue >= 64) {
      return leaf::new_error(Error::create(instruction));
    }

    switch (source.kind()) {
    case Operand::Kind::reg:
      destination = context->registerAt(source.as<Index>()) << shiftValue;
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = source.as<uint64_t>() << shiftValue;
      break;
    default:
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }

  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    uint64_t shiftValue = shift.as<uint64_t>();
    if (shiftValue >= 64) {
      return leaf::new_error(Error::create(instruction));
    }

    switch (source.kind()) {
    case Operand::Kind::reg:
      destination = context->registerAt(source.as<Index>()) << shiftValue;
      break;
    case Operand::Kind::i16:
    case Operand::Kind::u16:
      destination = source.as<uint64_t>() << shiftValue;
      break;
    default:
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }

   

  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeSr(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
  }
  Register &destination = context->registerAt(a.as<Index>());
  Operand source = instruction.b();
  Operand shift = instruction.c();

  switch (shift.kind()) {
  case Operand::Kind::reg: {
    Register &shiftValue = context->registerAt(shift.as<Index>());
    if (shiftValue >= 64) {
      return leaf::new_error(Error::create(instruction));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
		destination = context->registerAs<int64_t>(source.as<Index>()) >> shiftValue;
      break;
    }
    case Operand::Kind::i16:
    case Operand::Kind::u16: {
		destination = source.as<int64_t>() >> shiftValue;
      break;
    }
    default:
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }

  case Operand::Kind::i16:
  case Operand::Kind::u16: {
    int64_t shiftValue = shift.as<int64_t>();
    if (shiftValue < 0 || shiftValue >= 64) {
      return leaf::new_error(Error::create(instruction));
    }
    switch (source.kind()) {
    case Operand::Kind::reg: {
		destination = context->registerAs<int64_t>(source.as<Index>()) >> shiftValue;
      break;
    }
    case Operand::Kind::i16: 
    case Operand::Kind::u16: {
		destination = source.as<int64_t>() >> shiftValue;
      break;
    }
    default:
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }

  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeEq(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeLt(Instruction instruction,
                                    Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeLte(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeSlt(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeSlte(Instruction instruction,
                                      Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeNeg(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeAdd(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeSub(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeMul(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeDiv(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
    default:
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

static leaf::result<void> executeMod(Instruction instruction,
                                     Context::Ptr context) {
  Operand a = instruction.a();
  if (a.kind() != Operand::Kind::reg) {
    return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
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
      return leaf::new_error(Error::create(instruction));
    }
    break;
  }
  default:
    return leaf::new_error(Error::create(instruction));
  }
  return {};
}

} // namespace tbc
