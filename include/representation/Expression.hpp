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

#ifndef TBC_IMR_EXPRESSION_HPP
#define TBC_IMR_EXPRESSION_HPP

#include <cstdint>
#include <format>
#include <memory>
#include <ostream>
#include <vector>

#include "representation/Instruction.hpp"

namespace tbc {
class Expression : public std::vector<Instruction>,
                   public std::enable_shared_from_this<Expression> {
  struct Private {};

public:
  using Ptr = std::shared_ptr<Expression>;

  Expression(Private) {}

  static Ptr create() { return std::make_shared<Expression>(Private{}); }
};
}

template <> struct std::formatter<tbc::Expression> {
  template <class ParseContext> constexpr auto parse(ParseContext &ctx) const {
    return ctx.begin();
  }
  template <class FormatContext>
  constexpr auto format(const tbc::Expression &expression, FormatContext &ctx) const
      -> decltype(ctx.out()) {
    // size_t index = 0;
    for (const auto &instruction : expression) {
      std::format_to(ctx.out(), "{}\n", instruction);

      //if (index < (expression.size() - 1)) {
      //  std::format_to(ctx.out(), "\n");
      //}

      //index += 1;
    }
    return ctx.out();
  }
};

namespace tbc {
inline auto operator<<(std::ostream &out, const Expression &expression)
    -> std::ostream & {
  out << std::format("{}", expression);
  return out;
}
} // namespace tbc

#endif // !TBC_IMR_EXPRESSION_HPP
