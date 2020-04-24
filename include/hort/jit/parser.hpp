#ifndef HORT_JIT_PARSER_HPP_
#define HORT_JIT_PARSER_HPP_

#include <unordered_map>
#include <optional>

#include "hort/string.hpp"
#include "hort/vector.hpp"

#include "hort/jit/lexer.hpp"

namespace hort::jit {

struct Parser {

  struct Cursor {
    std::vector<Lexer::Token> source;
    decltype(source.cbegin()) iterator{source.cbegin()};

    Lexer::Token consume() noexcept {
      return *(iterator++);
    }

    Lexer::Token peek() const noexcept {
      return *(iterator + 1);
    }

    Lexer::Token get() const noexcept {
      return *iterator;
    }

    void step(int steps = 1) noexcept {
      for (; steps > 0; steps--) (*this)++;
    }

    bool has_next() const noexcept {
      return iterator != source.end();
    }

    void operator++([[maybe_unused]] int _) noexcept {
      iterator++;
    }

    Lexer::Token operator*() const noexcept {
      return *iterator;
    }
  } cursor{};

  explicit Parser(std::vector<Lexer::Token> &source_) noexcept
    : cursor{source_} {}

};

} // hort::jit


#endif // HORT_JIT_PARSER_HPP_
