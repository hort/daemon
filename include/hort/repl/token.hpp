#ifndef HORT_REPL_TOKEN_HPP_
#define HORT_REPL_TOKEN_HPP_

#include "hort/string.hpp"

namespace hort::repl {

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
    { Kind::Symbol,     "Symbol"     },
    { Kind::Builtin,    "Builtin"    },
    { Kind::String,     "String"     },
    { Kind::Number,     "Number"     },
    { Kind::Whitespace, "Whitespace" },
    { Kind::True,       "True"       },
    { Kind::False,      "False"      },
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

  string colorized() const {
    return format(colorize.find(kind)->second, value);
  }

  string str() const noexcept {
    return sprintf("{} = '{}'", kinds.find(kind)->second, value);
  }

};

} // hort::repl

#endif // HORT_REPL_TOKEN_HPP_
