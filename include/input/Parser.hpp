
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

#ifndef TBC_INPUT_PARSER_HPP
#define TBC_INPUT_PARSER_HPP

#include <array>
#include <memory>
#include <utility>

#include <boost/leaf.hpp>

namespace leaf = boost::leaf;

#include "input/Lexer.hpp"
#include "representation/Context.hpp"

namespace tbc {
class Parser {
public:
  enum class Precedence {
    none,
    assignment,
    or_,
    and_,
    equality,
    comparison,
    term,
    factor,
    unary,
    call,
    primary,
    length
  };

  using PrefixFn = leaf::result<Operand> (Parser::*)();
  using InfixFn = leaf::result<Operand> (Parser::*)(Operand left);

  struct Rule {
    PrefixFn prefix;
    InfixFn infix;
    Precedence precedence;
  };

  Parser(Context::Ptr context) : m_context(context), m_current(Token::end) {}
  Parser(Context::Ptr context, std::string_view view)
      : m_context(context), m_lexer(view), m_current(Token::end)
  {
  }

  void set(std::string_view view) {
    reset();
    m_lexer.set(view);
  }

  void reset() {
    m_context->beginNewExpression();
    m_current = Token::end;
    m_lexer.reset();
  }

  leaf::result<void> pull();

private:
  bool peek(Token token) const { return m_current == token; }

  leaf::result<void> next() {
    BOOST_LEAF_AUTO(token, m_lexer.next());
    m_current = token;
    return {};
  }

  leaf::result<bool> expect(Token token) {
    if (peek(token)) {
      BOOST_LEAF_CHECK(next());
      return true;
    }

    return false;
  }

  const Rule &lookupRule(Token token) const {
    // This is safe because the lexer will never return a token that is out of
    // bounds of the rules array. and if it does, it's a bug in the lexer and we
    // want to crash anyway.
    return m_rules.at(std::to_underlying(token));
  }

  leaf::result<Operand> infix(Precedence precedence);
  leaf::result<Operand> parenthesis();
  leaf::result<Operand> unary();
  leaf::result<Operand> binary(Operand left);
  leaf::result<Operand> primary();

private:
  Context::Ptr m_context;
  Lexer m_lexer;
  Token m_current;
  const std::array<Rule, static_cast<size_t>(Token::length)> m_rules = {{
      {nullptr, nullptr, Precedence::none},               // Token::end
      {nullptr, nullptr, Precedence::none},               // Token::return_
      {nullptr, nullptr, Precedence::none},               // Token::let
      {nullptr, nullptr, Precedence::none},               // Token::fn
      {&Parser::primary, nullptr, Precedence::none},   // Token::nil
      {&Parser::primary, nullptr, Precedence::none},      // Token::true_
      {&Parser::primary, nullptr, Precedence::none},      // Token::false_
      {nullptr, nullptr, Precedence::none},               // Token::typeNil
      {nullptr, nullptr, Precedence::none},               // Token::typeBool
      {nullptr, nullptr, Precedence::none},               // Token::typeI64
      {nullptr, &Parser::binary, Precedence::assignment}, // Token::equals
      {nullptr, &Parser::binary, Precedence::equality},   // Token::equalsEquals
      {nullptr, &Parser::binary, Precedence::comparison}, // Token::less
      {nullptr, &Parser::binary, Precedence::comparison}, // Token::lessEquals
      {nullptr, &Parser::binary, Precedence::comparison}, // Token::greater
      {nullptr, &Parser::binary,
       Precedence::comparison},                     // Token::greaterEquals
      {nullptr, &Parser::binary, Precedence::term}, // Token::plus
      {&Parser::unary, &Parser::binary, Precedence::term}, // Token::minus
      {nullptr, &Parser::binary, Precedence::factor},      // Token::star
      {nullptr, &Parser::binary, Precedence::factor}, // Token::forwardSlash
      {nullptr, &Parser::binary, Precedence::factor}, // Token::percent
      {&Parser::parenthesis, nullptr,
       Precedence::call},                   // Token::beginParenthesis
      {nullptr, nullptr, Precedence::none}, // Token::endParenthesis
      {&Parser::primary, nullptr, Precedence::none}, // Token::integer
      {&Parser::primary, nullptr, Precedence::none}, // Token::label
  }};
};

inline Parser::Precedence operator+(const Parser::Precedence& precedence, int right) {
  auto value = std::to_underlying(precedence);
  auto result = value + right;

  if (result > std::to_underlying(Parser::Precedence::length)) {
    return Parser::Precedence::primary;
  }

  return static_cast<Parser::Precedence>(result);
}

inline bool operator>(Parser::Precedence left, Parser::Precedence right) {
  return std::to_underlying(left) > std::to_underlying(right);
}

} // namespace tbc

#endif // !TBC_INPUT_PARSER_HPP
