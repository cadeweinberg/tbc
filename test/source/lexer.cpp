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

#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
namespace data = boost::unit_test::data;

#include "input/lexer.hpp"
#include "utility/error.hpp"

tbc::Token expected_tokens[] = {
    tbc::Token::End,          tbc::Token::Return,        tbc::Token::Let,
    tbc::Token::Fn,           tbc::Token::True,          tbc::Token::False,
    tbc::Token::TypeBool,     tbc::Token::Typei64,       tbc::Token::Equals,
    tbc::Token::EqualsEquals, tbc::Token::Less,          tbc::Token::LessEquals,
    tbc::Token::Greater,      tbc::Token::GreaterEquals, tbc::Token::Plus,
    tbc::Token::Minus,        tbc::Token::Star,          tbc::Token::FSlash,
    tbc::Token::Percent};

std::string_view expected_views[] = {
    ""sv,    "return"sv, "let"sv, "fn"sv, "true"sv, "false"sv, "bool"sv,
    "i64"sv, "="sv,      "=="sv,  "<"sv,  "<="sv,   ">"sv,     ">="sv,
    "+"sv,   "-"sv,      "*"sv,   "/"sv,  "%"sv};

BOOST_DATA_TEST_CASE(lexer_keywords,
                     data::make(expected_tokens) ^ data::make(expected_views),
                     expected, view) {
  tbc::Lexer lexer(view);

  tbc::Token actual = leaf::try_handle_all(
      [&]() -> leaf::result<tbc::Token> { return lexer.next(); },
      [&](tbc::Error const &error) -> tbc::Token {
        BOOST_ERROR(error);
        return tbc::Token::End;
      },
      [&]() -> tbc::Token {
        BOOST_ERROR(bst::stacktrace());
        std::abort();
      });

  BOOST_TEST(expected == actual);
}
