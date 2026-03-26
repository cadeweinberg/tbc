
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

#include "input/Parser.hpp"

namespace tbc {
leaf::result<Operand> Parser::infix(Precedence precedence) {
  const Rule &rule = lookupRule(m_current);
  if (rule.prefix == nullptr) {
    return leaf::new_error(std::format("Expected expression"));
  }

  BOOST_LEAF_AUTO(destination, (this->*rule.prefix)());

  while (true) {
    const Rule &rule = lookupRule(m_current);
    if (precedence > rule.precedence) {
      break;
    }

    BOOST_LEAF_AUTO(left, (this->*rule.infix)(destination));
    destination = left;
  }

  return destination;
}

leaf::result<Operand> Parser::parenthesis() {
  BOOST_LEAF_AUTO(found_begin_parenthesis, expect(Token::beginParenthesis));
  if (!found_begin_parenthesis) {
    return leaf::new_error(std::format("Expected '('"));
  }

  BOOST_LEAF_AUTO(destination, infix(Precedence::assignment));

  BOOST_LEAF_AUTO(found_end_parenthesis, expect(Token::endParenthesis));
  if (!found_end_parenthesis) {
    return leaf::new_error(std::format("Expected ')'"));
  }

  return destination;
}

leaf::result<Operand> Parser::unary() {
  Token unop = m_current;
  BOOST_LEAF_CHECK(next());

  BOOST_LEAF_AUTO(right, infix(Precedence::unary));

  switch (m_current) {
  case Token::minus:
    return m_context->emitNegate(right);

  default:
    return leaf::new_error(std::format("{} is not a unary operator.", unop));
  }
}

leaf::result<Operand> Parser::binary(Operand left) {
  Token binop = m_current;
  BOOST_LEAF_CHECK(next());

  const Rule &rule = lookupRule(binop);
  BOOST_LEAF_AUTO(right, infix(rule.precedence + 1));

  switch (binop) {
  case Token::plus:
    return m_context->emitAdd(left, right);
  case Token::minus:
    return m_context->emitSubtract(left, right);
  case Token::star:
    return m_context->emitMultiply(left, right);
  case Token::forwardSlash:
    return m_context->emitDivide(left, right);
  case Token::percent:
    return m_context->emitModulo(left, right);

  default:
    return leaf::new_error(std::format("{} is not a binary operator", binop));
  }
}

leaf::result<Operand> Parser::primary() {
  switch (m_current) {
  case Token::nil:
    BOOST_LEAF_CHECK(next());
    return Operand::u16(0);
  case Token::true_:
    BOOST_LEAF_CHECK(next());
    return Operand::u16(1);
  case Token::false_:
    BOOST_LEAF_CHECK(next());
    return Operand::u16(0);

  case Token::integer: {
    int16_t value = 0;
    auto view = m_lexer.current();
    auto [ptr, ec] =
        std::from_chars(view.data(), view.data() + view.length(), value);
    if (ec != std::errc{}) {
      return leaf::new_error(
          std::format("{}", std::make_error_code(ec).message()));
    }
    BOOST_LEAF_CHECK(next());
    return Operand::i16(value);
  }

  default:
    return leaf::new_error(std::format("{} is not a primary token", m_current));
  }
}

leaf::result<void> Parser::pull() {
  BOOST_LEAF_CHECK(next());

  while (!peek(Token::end)) {
    BOOST_LEAF_AUTO(destination, infix(Precedence::assignment));
  }

  return {};
}
} // namespace tbc