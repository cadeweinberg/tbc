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

#ifndef TBC_UTILITY_ERROR_HPP
#define TBC_UTILITY_ERROR_HPP

#include <format>
#include <ostream>
#include <source_location>
#include <string>

#include <boost/stacktrace.hpp>
namespace bst = boost::stacktrace;

namespace tbc {
struct Error {
  std::string message;
  std::source_location location;
  bst::stacktrace trace;

  static Error
  current(std::string message,
          std::source_location location = std::source_location::current(),
          bst::stacktrace trace = bst::stacktrace()) {
    return {std::move(message), std::move(location), std::move(trace)};
  }
};

inline auto operator<<(std::ostream &out, const std::source_location &location)
    -> std::ostream & {
  out << location.file_name() << ":" << location.line() << "."
      << location.column() << " in " << location.function_name();
  return out;
}

inline auto operator<<(std::ostream &out, const Error &error)
    -> std::ostream & {
  out << "Error: " << error.message << "\n"
      << "  at " << error.location << "\n"
      << "  during:\n"
      << error.trace;
  return out;
}
} // namespace tbc

template <> struct std::formatter<std::source_location> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator {
    return ctx.begin();
  }
  template <class FormatContext>
  auto format(const std::source_location &location,
              FormatContext &context) const -> FormatContext::iterator {
    return std::format_to(context.out(), "{}:{}.{} in {}", location.file_name(),
                          location.line(), location.column(),
                          location.function_name());
  }
};

template <> struct std::formatter<tbc::Error> {
  template <class ParseContext>
  constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const tbc::Error &error, FormatContext &context) const
      -> FormatContext::iterator {
    return std::format_to(context.out(), "Error: {}\n  at {}\n  during:\n{}",
                          error.message, error.location, error.trace);
  }
};

#endif // !TBC_UTILITY_ERROR_HPP
