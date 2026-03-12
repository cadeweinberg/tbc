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

#include "input/token.hpp"

namespace tbc {
std::string_view token_to_view(Token token) noexcept {
  switch (token) {
  case Token::End:
    return "EOF"sv;
  case Token::Return:
    return "return"sv;
  case Token::Let:
    return "let"sv;
  case Token::Fn:
    return "fn"sv;
  case Token::True:
    return "true"sv;
  case Token::False:
    return "false"sv;
  case Token::TypeBool:
    return "bool"sv;
  case Token::Typei64:
    return "i64"sv;
  case Token::Equals:
    return "="sv;
  case Token::EqualsEquals:
    return "=="sv;
  case Token::Less:
    return "<"sv;
  case Token::LessEquals:
    return "<="sv;
  case Token::Greater:
    return ">"sv;
  case Token::GreaterEquals:
    return ">="sv;
  case Token::Plus:
    return "+"sv;
  case Token::Minus:
    return "-"sv;
  case Token::Star:
    return "*"sv;
  case Token::FSlash:
    return "/"sv;
  case Token::Percent:
    return "%"sv;
  case Token::Integer:
    return "integer"sv;
  case Token::Label:
    return "label"sv;
  default:
    std::unreachable();
  }
}
} // namespace tbc
