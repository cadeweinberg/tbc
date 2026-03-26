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

#ifndef TBC_REPRESENTATION_CONTEXT_HPP
#define TBC_REPRESENTATION_CONTEXT_HPP

#include <memory>

#include "representation/Expression.hpp"
#include "representation/Registers.hpp"

namespace tbc {
class Context : public std::enable_shared_from_this<Context> {
  struct Private {};

public:
  using Ptr = std::shared_ptr<Context>;

  Context(Private)
      : m_expression(),
        m_registers(Registers::create()), m_used(0) {}

  static Ptr create() { return std::make_shared<Context>(Private{}); }

  void beginNewExpression() {
    m_used = 0;
    m_expression = Expression::create();
  }

  void emitHalt() { m_expression->push_back(Instruction::halt()); }

  void emitMove(Operand destination, Operand source) {
    m_expression->push_back(Instruction::move(destination, source));
  }

  Operand emitAnd(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(
        Instruction::and_(destination, left, right));
    return destination;
  }

  Operand emitOr(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::or_(destination, left, right));
    return destination;
  }

  Operand emitXor(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::xor_(destination, left, right));
    return destination;
  }

  Operand emitNot(Operand source) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::not_(destination, source));
    return destination;
  }

  Operand emitEquals(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::equals(destination, left, right));
    return destination;
  }

  Operand emitLess(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::less(destination, left, right));
    return destination;
  }

  Operand emitGreater(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::greater(destination, left, right));
    return destination;
  }

  Operand emitNegate(Operand source) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::negate(destination, source));
    return destination;
  }

  Operand emitAdd(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::add(destination, left, right));
    return destination;
  }

  Operand emitSubtract(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::subtract(destination, left, right));
    return destination;
  }

  Operand emitMultiply(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::multiply(destination, left, right));
    return destination;
  }

  Operand emitDivide(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::divide(destination, left, right));
    return destination;
  }

  Operand emitModulo(Operand left, Operand right) {
    Operand destination = Operand::register_(m_used++);
    m_expression->push_back(Instruction::modulo(destination, left, right));
    return destination;
  }

  void printExpression(std::ostream &out) const { out << *m_expression << "\n"; }

private:
  Expression::Ptr m_expression;
  Registers::Ptr m_registers;
  uint16_t m_used;
};
} // namespace tbc

#endif // !TBC_REPRESENTATION_CONTEXT_HPP
