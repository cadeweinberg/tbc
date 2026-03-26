
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

#include <iostream>
#include <print>
#include <string>

#include "utility/Error.hpp"
#include "input/Parser.hpp"
#include "representation/Context.hpp"

int main() {
  tbc::Context::Ptr context = tbc::Context::create();
  tbc::Parser parser{context};

  while (true) {
    std::cout << ">> ";
    std::string input{};
    std::getline(std::cin, input);

    parser.set(input);

    leaf::try_handle_all(
        [&]() -> leaf::result<void> { return parser.pull(); },
        [&](tbc::Error const &error) { std::print("{}", error); },
        [&]() { std::abort(); }
    );

    std::cout << "$ ";
    context->printExpression(std::cout);
  }
}
