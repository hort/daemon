#ifndef HORT_JIT_LEXER_HPP_
#define HORT_JIT_LEXER_HPP_

#include <unordered_map>

#include "hort/string.hpp"
#include "hort/vector.hpp"

struct Lexer {

  struct Token {

    enum class Kind
      { Symbol
      , Builtin
      , String
      , Number
      , Whitespace
      , None
      };

    struct Ln {
      size_t line{0};
      size_t column{0};
    };

    const Kind kind;
    const std::string value;
    const Ln ln;

    explicit Token(Kind kind_, const std::string &value_, Ln ln_)
      : kind{kind_}
      , value{value_}
      , ln{ln_} {}

    std::string colorize() const {
      switch (kind) {
        case Kind::Symbol:
          return "{}";
        case Kind::Builtin:
          return "\033[1;31m{}\033[0m";
        case Kind::String:
          return "\033[1;32m{}\033[0m";
        case Kind::Number:
          return "\033[1;33m{}\033[0m";
        case Kind::Whitespace:
          return "{}";
        case Kind::None:
          return "{}";
      }
      return "<none>";
    };

    std::string fancy_str() const {
      return fmt::format(colorize(), value);
    }

    std::string str() const {
      return value;
    }

  };

  std::vector<Token> tokens{};

  struct Cursor {
    std::string source;
    decltype(source.cbegin()) iterator{source.cbegin()};
    Token::Ln position{0, 0};

    char peek() const noexcept {
      return *(iterator + 1);
    }

    char consume() noexcept {
      return *(iterator++);
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

  static const std::unordered_map<std::string_view, Token::Kind> create_tokens() {
    return []() -> std::unordered_map<std::string_view, Token::Kind> {
      std::unordered_map<std::string_view, Token::Kind> result;

      result["interfaces"] = Token::Kind::Builtin;
      result["dump"] = Token::Kind::Builtin;
      result["archive"] = Token::Kind::Builtin;
      result["exit"] = Token::Kind::Builtin;

      return result;
    }();
  }

  static const std::unordered_map<std::string_view, Token::Kind> &build_tokens() {
    static const std::unordered_map<std::string_view, Token::Kind> result = create_tokens();
    return result;
  }

  std::unordered_map<std::string_view, Token::Kind> keywords = build_tokens();

  bool isdigit(char c) const noexcept {
    switch (c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return true;
        break;
      default:
        return false;
    }
  }

  bool isalpha(char c) const noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }

  Lexer(const std::string &source_) : cursor{source_} {
    while (cursor.has_next()) {
      auto start = cursor.position;
      std::string str;

      auto val = cursor.get();

      if (isdigit(val)) {
        while (isdigit(val = cursor.get())) {
          str += val;
          cursor.step();
        }
        tokens.emplace_back(Token(Token::Kind::Number, str, start));
        continue;
      }

      if (val == ' ') {
        while ((val = cursor.get()) == ' ') {
          str += val;
          cursor.step();
        }
        tokens.emplace_back(Token(Token::Kind::Whitespace, str, start));
        continue;
      }

      if (val == '\'') {
        str += val;
        cursor.step();
        val = cursor.get();
        while(val != '\'' && cursor.has_next()) {
          str += val;
          cursor.step();
          val = cursor.get();
        };
        str += val;
        if (cursor.has_next()) {
          cursor.step();
        }
        tokens.emplace_back(Token(Token::Kind::String, str, start));
        continue;
      }

      while(val != ' ' && cursor.has_next()) {
        str += val;
        cursor.step();
        val = cursor.get();
      };

      if (keywords.find(str) != keywords.end()) {
        tokens.emplace_back(Token(keywords[str.c_str()], str, start));
      } else {
        tokens.emplace_back(Token(Token::Kind::Symbol, str, start));
      }

    }
  }

  std::string fancy_str() {
    std::string str = "";
    for (const auto &tok : tokens) {
      str += tok.fancy_str();
    }
    return str;
  }

};

struct Parser {

  struct Cursor {
    std::vector<Lexer::Token> source;
    decltype(source.cbegin()) iterator{source.cbegin()};

    Lexer::Token peek() const noexcept {
      return *(iterator + 1);
    }

    Lexer::Token consume() noexcept {
      return *(iterator++);
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

  Parser(std::vector<Lexer::Token> &source_) : cursor{source_} {}

};

#endif // HORT_JIT_LEXER_HPP_
