# hort: The Data Scraping Framework on Steroids

**For numerous reasons this project has been put on hold.**

**Whenever reading the source please consider that this project been started over a year ago and the code doesn't reflect my current style. There's a number of uncommited changes but I am going to wait to push them to github until I feel like continuing revamping the entire source code.**

**In the last months I have made a series of commints to restructure the framework but hey won't be pushed to github for some time.**

Things I'll have to fix before feeling comfortable on continuing hort's development:
 - Create Lua bindings and allow users to define interfaces throught scripts.
 - Clean up standard library wrappers as most of them aren't as safe as I would like them to be.
 - Remove `nhlomann/json` dependency as it bloats up compile times.

hort aims to be a high performance data mining and data scraping framework.

I'm planning to develop a scripting language around this framework aimed at
quick scraper development without any boilerplate code.

## Example

``` c++
#include "hort/interface.hpp"

// - Sets up Prometheus, Loki, and MongoDB drivers
// - Registers interface to horts' registry
HORT_INTERFACE(foo) {
  foo() : hort::Interface{"foo",
    { R"regex(foo (.*))regex", [this] (auto v) {
        hort::printf("matched first rule with {}", v[0]);
      }},
      { R"regex(bar (.*))regex", [this] (auto v) {
        hort::printf("matched second rule with {}", v[0]);
      }}
    }
  } { }

  void archive() override {
    using hort::filesystem::write;

    logger.Debugf("downloading and indexing webpage");
    auto r = session.get("http://example.com/page/{}", 5);
    index.insert({
      { "url", r.url },
      { "content", r.body },
      { "filepath", hortpath + "foo.txt" },
    });
    write(r.body, hortpath, "foo.txt");
  }
}

int main(int argv, char* argv[]) {
  hort::Registry::instance().parse(argc, argv);
  return 0;
}
```

```sh
$ hortd
Usage: hortd [options...]

  Data hoarding and indexing framework

Options:

  -h, --help          Show help
  -v, --version       Show version
  -i, --interactive   Run in interactive mode
  -r, --repl          Run REPL (experimental)
  -f, --forward       Forward
  -d, --dump          Dump an interface's index
  -a, --archive       Archive Interface

$ hortd -i
(hort) interfaces
- foo
(hort) use foo
(foo) archive
2020-04-26 17:30:59.489904894 [ INFO] started archiving with interface foo
2020-04-26 17:30:59.543762456 [DEBUG] downloading and indexing webpage
2020-04-26 17:34:28.553822604 [ INFO] done archiving with interface foo
```

## Monitoring Services

- Logging: Loki ([mircodezorzi/loki-cpp](http://github.com/mircodezorzi/loki-cpp))
- Monitoring: Promotheus ([jupp0r/prometheus-cpp](https://github.com/jupp0r/prometheus-cpp))

## Current Code Base

STD Wrappers: `hort::string`, `hort::vector`, `hort::filesystem`

### Library Wrappers
- `hort::archive`: libzip wrapper
- `hort::http`: libcurl wrapper
- `hort::print`: fmt wrapper
- `hort::regexp`: RE2 wrapper

### Infrastructure

- `hort::Registry`: Infrastructure orchestrator
- `hort::Interface`: User overridable API interfaces
- `hort::Args`: Command line argument parsing library
- `hort::Index`: High level MongoDB driver
- `hort::Session`: HTTP session management
- `hort::Worker`: Concurrency worker pools

## Dependencies

- curl
- fmt
- libzip
- md5
- mongocxx
- nhlomann/json
- rapidxml
- re2
- yaml-cpp

## ~~Plans for the future~~

- Automated parallel pipelines
- Intelligent rate limiters
- Enhanced code reflection
