#ifndef HORT_JIT_LEXER_HPP_
#define HORT_JIT_LEXER_HPP_

#include <unordered_map>
#include <optional>

#include "hort/string.hpp"
#include "hort/vector.hpp"

namespace hort::jit {

struct Lexer {

  struct Token {

    enum class Kind
      { Symbol
      , Builtin
      , String
      , Number
      , Whitespace
      , True
      , False
      };

    const std::unordered_map<Kind, std::string_view> kinds{
      { Kind::Symbol, "Symbol" },
      { Kind::Builtin, "Builtin" },
      { Kind::String, "String" },
      { Kind::Number, "Number" },
      { Kind::Whitespace, "Whitespace" },
      { Kind::True, "True" },
      { Kind::False, "False" },
    };

    struct Ln {
      size_t line{0};
      size_t column{0};

      bool operator==(const Ln &other) const noexcept {
        return line == other.line && column == other.column;
      }
    };

    const Kind kind;
    const std::string value;
    const Ln ln;

    explicit Token(Kind kind_, const std::string &value_, Ln ln_) noexcept
      : kind{kind_}
      , value{value_}
      , ln{ln_} {}

    const std::unordered_map<Kind, std::string_view> colorize{
      { Kind::Symbol,     "{}"                  },
      { Kind::Builtin,    "\033[1;31m{}\033[0m" },
      { Kind::String,     "\033[1;32m{}\033[0m" },
      { Kind::Number,     "\033[1;33m{}\033[0m" },
      { Kind::Whitespace, "{}"                  },
      { Kind::True,       "\033[1m{}\033[0m"    },
      { Kind::False,      "\033[1m{}\033[0m"    },
    };

    std::string colorized() const {
      return fmt::format(colorize.find(kind)->second, value);
    }

    std::string str() const noexcept {
      return fmt::format("{} = '{}'", kinds.find(kind)->second, value);
    }

  };

  std::vector<Token> tokens{};

  struct Error {
    std::string_view message;
    Lexer::Token::Ln ln{0, 1};

    explicit operator bool() const noexcept {
      return ln.column != 0;
    }
  };

  std::vector<Error> errors;

  struct Cursor {
    std::string source;
    decltype(source.cbegin()) iterator{source.cbegin()};
    Token::Ln position{0, 0};

    char consume() noexcept {
      return *(iterator++);
    }

    char peek() const noexcept {
      return *(iterator + 1);
    }

    char get() const noexcept {
      return *iterator;
    }

    void step(int steps = 1) noexcept {
      for (; steps > 0; steps--) (*this)++;
    }

    bool has_next() const noexcept {
      return iterator != source.end();
    }

    void operator++(int _) noexcept {
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

    char operator*() const noexcept {
      return *iterator;
    }
  } cursor{};

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
    std::string str = "";

    auto val = cursor.get();

    if (isdigit(val)) {
      while ((val = cursor.get()) != ' ' && cursor.has_next()) {
        if (!isdigit(val)) {
          errors.emplace_back(Error{"invalid symbol", start});
        }
        str += val;
        cursor.step();
      }
      auto token = Token{Token::Kind::Number, str, start};
      tokens.push_back(token);
      return token;
    }

    if (val == ' ') {
      while ((val = cursor.get()) == ' ') {
        str += val;
        cursor.step();
      }
      auto token = Token{Token::Kind::Whitespace, str, start};
      tokens.push_back(token);
      return token;
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
      auto token = Token{Token::Kind::String, str, start};
      tokens.push_back(token);
      return token;
    }

    while (val != ' ' && cursor.has_next()) {
      str += val;
      cursor.step();
      val = cursor.get();
    };

    if (keywords.find(str) != keywords.end()) {
      auto token = Token{keywords.find(str.c_str())->second, str, start};
      tokens.push_back(token);
      return token;
    } else {
      auto token = Token{Token::Kind::Symbol, str, start};
      tokens.push_back(token);
      return token;
    }

  };

  std::string colorized() {
    std::string str = "";
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

  std::string str() {
    std::string str = "";
    for (const auto& tok : tokens) {
      str += tok.str() + "\n";
    }
    return str;
  }

};

} // hort::jit

#endif // HORT_JIT_LEXER_HPP_
