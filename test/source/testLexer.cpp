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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <charconv>
#include <limits>

#include "input/Lexer.hpp"
#include "utility/Error.hpp"

tbc::Token expected_tokens[] = {
    tbc::Token::end,           tbc::Token::return_,
    tbc::Token::let,           tbc::Token::fn,
    tbc::Token::nil,           tbc::Token::true_,
    tbc::Token::false_,        tbc::Token::typeNil,
    tbc::Token::typeBool,      tbc::Token::typeI64,
    tbc::Token::equals,        tbc::Token::equalsEquals,
    tbc::Token::less,          tbc::Token::lessEquals,
    tbc::Token::greater,       tbc::Token::greaterEquals,
    tbc::Token::plus,          tbc::Token::minus,
    tbc::Token::star,          tbc::Token::forwardSlash,
    tbc::Token::percent,       tbc::Token::beginParenthesis,
    tbc::Token::endParenthesis};

std::string_view expected_views[] = {
    ""sv,      "return"sv, "let"sv,  "fn"sv,  "nil"sv, "true"sv,
    "false"sv, "Nil"sv,    "Bool"sv, "I64"sv, "="sv,   "=="sv,
    "<"sv,     "<="sv,     ">"sv,    ">="sv,  "+"sv,   "-"sv,
    "*"sv,     "/"sv,      "%"sv,    "("sv,   ")"sv};

BOOST_DATA_TEST_CASE(lexer_keywords,
                     data::make(expected_tokens) ^ data::make(expected_views),
                     expected, view) {
  tbc::Lexer lexer(view);

  tbc::Token actual = leaf::try_handle_all(
      [&]() -> leaf::result<tbc::Token> { return lexer.next(); },
      [&](tbc::Error const &error) -> tbc::Token {
        BOOST_ERROR(error);
        return tbc::Token::end;
      },
      [&]() -> tbc::Token {
        //BOOST_ERROR(std::stacktrace::current());
        std::abort();
      });

  BOOST_TEST(expected == actual);
}

class LexerIntegerDataset {
public:
  static const int arity = 1;

  struct iterator {
    boost::mt19937_64 generator;
    uint64_t number;

    uint64_t operator*() const { return number; }
    void operator++() { number = generator(); }
  };

  data::size_t size() const { return data::BOOST_TEST_DS_INFINITE_SIZE; }
  iterator begin() const { return iterator{}; }
};

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {
template <> struct is_dataset<LexerIntegerDataset> : boost::mpl::true_ {};
} // namespace monomorphic
} // namespace data
} // namespace unit_test
} // namespace boost

BOOST_DATA_TEST_CASE(lexer_integer, LexerIntegerDataset{} ^ data::xrange(20),
                     integer, index) {
  static const uint32_t length = std::numeric_limits<uint64_t>::digits10 + 1;
  char buffer[length + 1] = {};
  {
    auto [ptr, ec] = std::to_chars(buffer, buffer + length, integer);
    if (ec != std::errc{}) {
      BOOST_ERROR(std::make_error_code(ec));
      return;
    }
  }

  tbc::Lexer lexer({buffer, buffer + length});

  tbc::Token actual = leaf::try_handle_all(
      [&]() -> leaf::result<tbc::Token> { return lexer.next(); },
      [&](tbc::Error const &error) -> tbc::Token {
        BOOST_ERROR(error);
        return tbc::Token::end;
      },
      [&]() -> tbc::Token {
        //BOOST_ERROR(std::stacktrace::current());
        std::abort();
      });

  BOOST_TEST(actual == tbc::Token::integer);

  uint64_t parsed = 0;
  std::string_view view = lexer.current();
  {
    auto [ptr, ec] =
        std::from_chars(view.data(), view.data() + view.length(), parsed);
    if (ec != std::errc{}) {
      BOOST_ERROR(std::make_error_code(ec));
      return;
    }
  }

  BOOST_TEST(parsed == integer);
}

static const std::string_view label_begin =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"sv;
static const std::string_view label_continue =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789"sv;

class LexerLabelDataset {
public:
  static const int arity = 1;
  struct iterator {
    using Generator = boost::random::mt19937;
    using Distribution = boost::random::uniform_int_distribution<uint32_t>;
    Generator generator;
    std::string label;

    iterator() { generate(); }

    void generate() {
      Distribution distribution{10, 100};
      uint32_t length = distribution(generator);
      label = choose(label_begin);

      for (int i = 0; i < length; ++i) {
        label += choose(label_continue);
      }
    }

    char choose(std::string_view view) {
      assert(std::in_range<uint32_t>(view.length()));
      Distribution distribution{0, static_cast<uint32_t>(view.length()) - 1};
      uint32_t index = distribution(generator);
      return view.at(index);
    }

    std::string operator*() const { return label; }
    void operator++() { generate(); }
  };

  data::size_t size() const { return data::BOOST_TEST_DS_INFINITE_SIZE; }
  iterator begin() const { return iterator{}; }
};

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {
template <> struct is_dataset<LexerLabelDataset> : boost::mpl::true_ {};
} // namespace monomorphic
} // namespace data
} // namespace unit_test
} // namespace boost

BOOST_DATA_TEST_CASE(lexer_label, LexerLabelDataset{} ^ data::xrange(20), label,
                     index) {
  tbc::Lexer lexer{label};

  tbc::Token actual = leaf::try_handle_all(
      [&]() -> leaf::result<tbc::Token> { return lexer.next(); },
      [&](tbc::Error const &error) -> tbc::Token {
        BOOST_ERROR(error);
        return tbc::Token::end;
      },
      [&]() -> tbc::Token {
       // BOOST_ERROR(std::stacktrace::current());
        std::abort();
      });

  BOOST_TEST(actual == tbc::Token::label);

  std::string_view view = lexer.current();

  BOOST_TEST(view == label);
}
