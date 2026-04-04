
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
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "algorithm/Execute.hpp"
#include "input/Parser.hpp"
#include "representation/Context.hpp"
#include "utility/Error.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

tbc::Context::Ptr context;

EMSCRIPTEN_KEEPALIVE
int main() {
  context = tbc::Context::create();
  return EXIT_SUCCESS;
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void readEvaluatePrint(char *input) {
  tbc::Parser parser{context};
  bool hasError = false;

  parser.set(input);

  leaf::try_handle_all([&]() -> leaf::result<void> { return parser.pull(); },
                       [&](tbc::Error const &error) {
                         std::cerr << error << "\n";
                         hasError = true;
                       },
                       [&]() { std::abort(); });

  if (hasError) {
    return;
  }

  leaf::try_handle_all(
      [&]() -> leaf::result<void> { return tbc::execute(context); },
      [&](tbc::Error const &error) {
        std::cerr << error << "\n";
        hasError = true;
      },
      [&]() { std::abort(); });

  if (hasError) {
    return;
  }

  //std::cout << "Expression compiled to:\n";
  //std::cout << *context->getExpression();
  //std::cout << "Expression evaluated to:\n";
  std::cout << "$ " << *context->getResult() << "\n";
  

  free(input);
}
}

#else
static void readEvaluatePrint(tbc::Context::Ptr context);

int main(int argc, char **argv) {
  po::options_description desc("tbc [options]");
  desc.add_options()("help,h", "Print this help message")(
      "version,v", "Print program version");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (vm.count("version")) {
    std::cout << "tbc version 0.1.0\n";
    return EXIT_SUCCESS;
  }

  tbc::Context::Ptr context = tbc::Context::create();

  while (true) {
    readEvaluatePrint(context);
  }

  return EXIT_SUCCESS;
}

static void readEvaluatePrint(tbc::Context::Ptr context) {
  tbc::Parser parser{context};
  bool hasError = false;

  std::cout << ">> ";
  std::string input{};
  std::getline(std::cin, input);

  parser.set(input);

  leaf::try_handle_all([&]() -> leaf::result<void> { return parser.pull(); },
                       [&](tbc::Error const &error) {
                         std::cerr << error << "\n";
                         hasError = true;
                        },
      [&]() { std::abort(); });

  if (hasError) {
    return;
  }

  leaf::try_handle_all(
      [&]() -> leaf::result<void> { return tbc::execute(context); },
      [&](tbc::Error const &error) {
        std::cerr << error << "\n";
        hasError = true;
       },
      [&]() { std::abort(); });

  if (hasError) {
    return;
  }

  //std::cout << "Compiled expression:\n";
  //std::cout << *context->getExpression();
  //std::cout << "Expression evaluated to:\n";
  std::cout << "$ " << *context->getResult() << "\n";
  
}

#endif
