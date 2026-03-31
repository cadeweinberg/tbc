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

// since gcc's libstdc++ version 13 doesn't provide the overloads for 
// std::format for std::source_location and std::stacktrace, we need to
// define them ourselves.

#include <version>

#include <type_traits>

#include <ostream>
#include <string>

#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
// #TODO: if libstdc++ doesn't support std::stacktrace, we should
// include boost stacktrace instead. as boost stacktrace supports C++03.
#endif

#if defined(__cpp_lib_format) && __cpp_lib_format >= 202311L
#include <format>
#endif

#if defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
#include <source_location>
#endif

namespace tbc {
	struct Error {
		std::string message;
		std::source_location location;
		std::stacktrace trace;

		static Error
			current(std::string message,
				std::source_location location = std::source_location::current(),
				std::stacktrace trace = std::stacktrace::current()) {
			return { std::move(message), std::move(location), std::move(trace) };
		}
	};
} // namespace tbc

#if !defined(__cpp_lib_formatters) || __cpp_lib_formatters < 202302L
template <> struct std::formatter<std::source_location> {
	template <class ParseContext>
	constexpr auto parse(ParseContext& ctx) -> ParseContext::iterator {
		return ctx.begin();
	}
	template <class FormatContext>
	constexpr auto format(const std::source_location& location,
		FormatContext& context) const -> FormatContext::iterator {
		return std::format_to(context.out(), "{}:{}.{} in {}", location.file_name(),
			location.line(), location.column(),
			location.function_name());
	}
};

template <> struct std::formatter<std::stacktrace_entry>
{
	template <class ParseContext>
	constexpr auto parse(ParseContext & ctx) -> ParseContext::iterator {
		return ctx.begin();
	}

	template <class FormatContext>
	constexpr auto format(const std::stacktrace_entry & entry, FormatContext & context) const -> FormatContext::iterator {
		return std::format_to(context.out(), "{}", entry.description());
	}
};

template <> struct std::formatter<std::stacktrace>
{
	template <class ParseContext>
	constexpr auto parse(ParseContext & ctx) -> ParseContext::iterator {
		return ctx.begin();
	}

	template <class FormatContext>
	constexpr auto format(const std::stacktrace & trace, FormatContext & context) const -> FormatContext::iterator {
		for (const auto& frame : trace) {
			std::format_to(context.out(), "{}\n", frame);
		}
		return context.out();
	}
};

template <> struct std::formatter<tbc::Error> {
	template <class ParseContext>
	constexpr auto parse(ParseContext& ctx) -> ParseContext::iterator {
		return ctx.begin();
	}

	template <class FormatContext>
	constexpr auto format(const tbc::Error& error, FormatContext& context) const
		-> FormatContext::iterator {
		return std::format_to(context.out(), "Error: {}\n  at {}\n  during:\n{}",
			error.message, error.location, error.trace);
	}
};

namespace tbc {
	inline auto operator<<(std::ostream& out, const std::source_location& location)
		-> std::ostream& {
		out << std::format("{}", location);
		return out;
	}

	inline auto operator<<(std::ostream& out, const Error& error)
		-> std::ostream& {
		out << std::format("{}", error);
		return out;
	}
} // namespace tbc

#else 

namespace tbc {
	inline auto operator<<(std::ostream& out, const std::source_location& location)
		-> std::ostream& {
		out << location.file_name() << ":" << location.line() << "." << location.column()
			<< " in " << location.function_name();
		return out;
	}
	inline auto operator<<(std::ostream& out, const Error& error)
		-> std::ostream& {
		out << "Error: " << error.message << "\n  at " << error.location
			<< "\n  during:\n" << error.trace;
		return out;
	}
} // namespace tbc
#endif

#endif // !TBC_UTILITY_ERROR_HPP
