# hort: The Data Scraping Framework on Steroids

hort aims to be a high performance data mining and data scraping framework.

I'm planning to develop a scripting language around this framework aimed at
quick scraper development **without any boilerplate** code.

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

## Example

``` c++
#include "hort/interface.hpp"
#include "hort/registry.hpp"

/// creates a logger agent to Loki & Prometheus
/// creates a database agent to MongoDB
/// creates a HTTP session handler (cookies are automatically saved & loaded)
HORT_INTERFACE(foo) {
  foo() : hort::Interface{"foo", 
    { HORT_RULE_CALLBACK(R"regex(foo (.*))regex", { hort::print(v[0]); } )
    }
  } {}
  
  void archive() override {
    logger.Warnf("bar");
    auto r = session.get("http://example.com/page/{}", 5);
    index.insert({
      { "url", r.url },
      { "content", r.body },
    });
    hort::filesystem::write(r.body, hortpath, "foo.txt");
  }
}

int main(int argv, char* argv[]) {
  hort::Registry::instance().parse(argc, argv);
  return 0;
}
```

### Forwarding
```
$ hortd -r
(hort) foo bar
2020-04-26 17:30:45.8309840654 [ INFO] matched foo bar with interface foo
bar
2020-04-26 17:30:47.8564894651 [ INFO] finished match callback with interface foo
```

### Archiving
``` c++
$ hortd -r
(hort) list
foo
(hort) foo
(foo) archive
2020-04-26 17:30:59.489904894 [ INFO] started archiving with interface foo
2020-04-26 17:30:59.543762456 [ WARN] bar
2020-04-26 17:34:28.553822604 [ INFO] done archiving with interface foo
```

## Monitoring Services

- Logging: Loki ([mircodezorzi/loki-cpp](http://github.com/mircodezorzi/loki-cpp))
- ~~Monitoring: Promotheus~~ ([~~jupp0r/prometheus-cpp~~](https://github.com/jupp0r/prometheus-cpp))

## Current Code Base

### STL 

#### STD enhancements
- `hort::string`: thin std::string wrapper
- `hort::vector`: thin std::vector wrapper
- `hort::filesystem`: filesystem operations

#### Wrappers
- `hort::archive`: libzip wrapper
- `hort::http`: libcurl wrapper
- `hort::print`: fmt wrapper
- `hort::regexp`: RE2 wrapper

### Infrastructure

- `hort::Args`: Argument parsing
- `hort::Index`: MongoDB interface
- `hort::Interface`: User defined API interfaces
- `hort::Registry`: Handles the entire infrastructure
- `hort::Session`: HTTP session management
- `hort::Worker`: Parallel worker pools

## Plans for the future

- Automated parallel pipelines
- Intelligent rate limiters
- Code reflection (?)
- Dockerize the entire infrastructure
