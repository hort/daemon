#ifndef HORT_REPL_LEXER_HPP_
#define HORT_REPL_LEXER_HPP_

#include <unordered_map>
#include <optional>

#include "hort/string.hpp"
#include "hort/vector.hpp"

#include "hort/repl/token.hpp"

namespace hort::repl {

struct Lexer {

  using CharType = char;

  struct Error {
    std::string_view message;
    Token::Ln ln{0, 1};

    explicit operator bool() const noexcept {
      return ln.column != 0;
    }
  };

  struct Cursor {
    string source;
    decltype(source.cbegin()) iterator{source.cbegin()};
    Token::Ln position{0, 0};

    CharType consume() noexcept {
      return *(iterator++);
    }

    CharType peek() const noexcept {
      return *(iterator + 1);
    }

    CharType get() const noexcept {
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
      switch (*iterator) {
        case '\n':
          position.column = 0;
          position.line += 1;
          break;
        default:
          position.column += 1;
          break;
      }
    }

    CharType operator*() const noexcept {
      return *iterator;
    }
  } cursor{};

  vector<Error> errors;
  vector<Token> tokens{};

  const std::unordered_map<std::string_view, Token::Kind> keywords{
    { "archive",    Token::Kind::Builtin },
    { "dump",       Token::Kind::Builtin },
    { "exit",       Token::Kind::Builtin },
    { "interfaces", Token::Kind::Builtin },
    { "use",        Token::Kind::Builtin },
    { "true",       Token::Kind::True    },
    { "false",      Token::Kind::False   },
  };

  explicit Lexer(const std::string &source_)
    : cursor{source_} {}

  std::optional<Token> consume() {
    if (!cursor.has_next()) {
      return std::nullopt;
    }

    auto start = cursor.position;
    string str = "";

    auto val = cursor.get();

    if (isdigit(val)) {
      while ((val = cursor.get()) != ' ' && cursor.has_next()) {
        if (!isdigit(val)) {
          errors.emplace_back(Error{"invalid symbol", start});
        }
        str += val;
        cursor.step();
      }
      return new_token(Token::Kind::Number, str, start);
    }

    if (val == ' ') {
      while ((val = cursor.get()) == ' ') {
        str += val;
        cursor.step();
      }
      return new_token(Token::Kind::Whitespace, str, start);
    }

    if (val == '\'') {
      str += val;
      cursor.step();
      val = cursor.get();
      while (val != '\'' && cursor.has_next()) {
        str += val;
        cursor.step();
        val = cursor.get();
      };
      str += val;
      if (cursor.has_next()) {
        cursor.step();
      } else {
        errors.emplace_back(Error{"unmatched '", start});
      }
      return new_token(Token::Kind::String, str, start);
    }

    while (val != ' ' && cursor.has_next()) {
      str += val;
      cursor.step();
      val = cursor.get();
    };

    if (keywords.find(str) != keywords.end()) {
      return new_token(keywords.find(str.c_str())->second, str, start);
    } else {
      return new_token(Token::Kind::Symbol, str, start);
    }

  };

  template <typename ... Args>
  Token new_token(const Args& ... args) {
    auto token = Token{args...};
    tokens.push_back(token);
    return token;
  }

  string colorized() {
    string str = "";
    Error error;
    while (consume());
    for (int i = 0; const auto &tok : tokens) {
      if (errors.size()) {
        error = errors[i];
      }
      if (tok.ln == error.ln) {
        str += "\033[4:3m" + tok.colorized() + "\033[4:0m";
        i++;
      } else {
        str += tok.colorized();
      }
    }
    return str;
  }

  string str() {
    string str = "";
    for (const auto& tok : tokens) {
      str += tok.str() + "\n";
    }
    return str;
  }

};

} // hort::repl

#endif // HORT_REPL_LEXER_HPP_
