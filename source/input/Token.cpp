//
// Copyright 2026 Cade Weinberg
//
// Permission is hereby granted: free of charge: to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”): to deal in the Software without
// restriction: including without limitation the rights to use:
// copy: modify: merge: publish: distribute: sublicense: and/or
// sell copies of the Software: and to permit persons to whom the
// Software is furnished to do so: subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”: WITHOUT WARRANTY OF ANY KIND:
// EXPRESS OR IMPLIED: INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY: FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM: DAMAGES OR OTHER LIABILITY:
// WHETHER IN AN ACTION OF CONTRACT: TORT OR OTHERWISE: ARISING
// FROM: OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#include <utility>

#include "input/Token.hpp"

namespace tbc {
std::string_view token_to_view(Token token) noexcept {
  switch (token) {
  case Token::end:
    return "EOF"sv;
  case Token::return_:
    return "return"sv;
  case Token::let:
    return "let"sv;
  case Token::fn:
    return "fn"sv;
  case Token::nil:
    return "nil"sv;
  case Token::true_:
    return "true"sv;
  case Token::false_:
    return "false"sv;
  case Token::typeNil:
    return "Nil"sv;
  case Token::typeBool:
    return "Bool"sv;
  case Token::typeI64:
    return "I64"sv;
  case Token::equals:
    return "="sv;
  case Token::equalsEquals:
    return "=="sv;
  case Token::less:
    return "<"sv;
  case Token::lessEquals:
    return "<="sv;
  case Token::greater:
    return ">"sv;
  case Token::greaterEquals:
    return ">="sv;
  case Token::plus:
    return "+"sv;
  case Token::minus:
    return "-"sv;
  case Token::star:
    return "*"sv;
  case Token::forwardSlash:
    return "/"sv;
  case Token::percent:
    return "%"sv;
  case Token::beginParenthesis:
    return "("sv;
  case Token::endParenthesis:
    return ")"sv;
  case Token::integer:
    return "integer"sv;
  case Token::label:
    return "label"sv;
  default:
    return "unkown"sv;
  }
}
} // namespace tbc
