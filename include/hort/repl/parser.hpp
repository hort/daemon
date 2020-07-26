#ifndef HORT_REPL_PARSER_HPP_
#define HORT_REPL_PARSER_HPP_

#include "hort/repl/lexer.hpp"

namespace hort::repl {

struct Parser {

  /*
  struct Cursor {
    std::vector<Token> source;
    decltype(source.cbegin()) iterator{source.cbegin()};

    Token consume() noexcept {
      return *(iterator++);
    }

    Token peek() const noexcept {
      return *(iterator + 1);
    }

    Token get() const noexcept {
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

    Token operator*() const noexcept {
      return *iterator;
    }
  } cursor{};

  explicit Parser(vector<Token> &source_) noexcept
    : cursor{source_} {}
  */

  explicit Parser() noexcept {}


};

} // hort::repl

#endif // HORT_REPL_PARSER_HPP_
