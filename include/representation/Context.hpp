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
#include "representation/Value.hpp"

namespace tbc {
class Context : public std::enable_shared_from_this<Context> {
  struct Private {};

public:
  using Ptr = std::shared_ptr<Context>;

  Context(Private)
      : m_expression(), m_registers(Registers::create()), m_used(0) {}

  static Ptr create() { return std::make_shared<Context>(Private{}); }

  void beginNewExpression() {
    m_used = 0;
    m_expression = Expression::create();
  }

  void emitRet(Operand value) {
    m_expression->push_back(Instruction::ret(value));
  }

  void emitMv(Operand destination, Operand source) {
    m_expression->push_back(Instruction::mv(destination, source));
  }

  Operand emitAnd(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::and_(destination, left, right));
    return destination;
  }

  Operand emitOr(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::or_(destination, left, right));
    return destination;
  }

  Operand emitXor(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::xor_(destination, left, right));
    return destination;
  }

  Operand emitNot(Operand source) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::not_(destination, source));
    return destination;
  }

  Operand emitSl(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::sl(destination, left, right));
    return destination;
  }

  Operand emitSr(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::sr(destination, left, right));
    return destination;
  }

  Operand emitEq(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::eq(destination, left, right));
    return destination;
  }

  Operand emitNeq(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::neq(destination, left, right));
    return destination;
  }

  Operand emitLt(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::lt(destination, left, right));
    return destination;
  }

  Operand emitLte(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::lte(destination, left, right));
    return destination;
  }

  Operand emitGt(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::lt(destination, right, left));
    return destination;
  }

  Operand emitGte(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::lte(destination, right, left));
    return destination;
  }

  Operand emitSlt(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::slt(destination, left, right));
    return destination;
  }

  Operand emitSlte(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::slte(destination, left, right));
    return destination;
  }

  Operand emitSgt(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::slt(destination, right, left));
    return destination;
  }

  Operand emitSgte(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::slte(destination, right, left));
    return destination;
  }

  Operand emitNeg(Operand source) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::neg(destination, source));
    return destination;
  }

  Operand emitAdd(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::add(destination, left, right));
    return destination;
  }

  Operand emitSub(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::sub(destination, left, right));
    return destination;
  }

  Operand emitMul(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::mul(destination, left, right));
    return destination;
  }

  Operand emitDiv(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::div(destination, left, right));
    return destination;
  }

  Operand emitMod(Operand left, Operand right) {
    Operand destination = Operand::reg(m_used++);
    m_expression->push_back(Instruction::mod(destination, left, right));
    return destination;
  }

  Expression::Ptr getExpression() const { return m_expression; }

  Register &registerAt(Index index) const { return m_registers->at(index); }

  template <class T> T &registerAs(Index index) {
    return reinterpret_cast<T &>(m_registers->at(index));
  }

  void setResult(Value::Ptr result) { m_result = std::move(result); }
  Value::Ptr getResult() const { return m_result; }

private:
  Expression::Ptr m_expression;
  Registers::Ptr m_registers;
  uint16_t m_used;
  Value::Ptr m_result;
};
} // namespace tbc

#endif // !TBC_REPRESENTATION_CONTEXT_HPP
