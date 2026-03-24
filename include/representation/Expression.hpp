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
#include <ostream>
#include <vector>

#include "representation/Instruction.hpp"

namespace tbc {
class Expression : public std::vector<Instruction> {};
}

template <> struct std::formatter<tbc::Expression> {
  template <class ParseContext> constexpr auto parse(ParseContext &ctx) {
	return ctx.begin();
  }
  template <class FormatContext>
  constexpr auto format(const tbc::Expression &expression,
						FormatContext &ctx) -> decltype(ctx.out()) {
	for (const auto &instruction : expression) {
	  std::format_to(ctx.out(), "{}\n", instruction);
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

#endif // !TBC_IMR_EXPRESSION_HPP
