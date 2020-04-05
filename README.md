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
// interfaces/foo.hpp
#include "hort/interface.hpp"

HORT_INTERFACE(foo) {
  void archive(); 
}
```

```
$ hortd -r
(hort) list
foo
(hort) foo
(foo) archive
archiving...
```

## ~~Monitoring Services~~

I'll put this on hold for some time...

- ~~Logging: Loki~~ ([~~mircodezorzi/loki-cpp~~](http://github.com/mircodezorzi/loki-cpp))
- ~~Monitoring: Promotheus~~ ([~~jupp0r/prometheus-cpp~~](https://github.com/jupp0r/prometheus-cpp))

## Current Code Base

### STL 

- `hort::archive`: libzip wrapper
- `hort::filesystem`: filesystem operations
- `hort::http`: libcurl wrapper
- `hort::print`: fmt wrapper
- `hort::regexp`: RE2 wrapper
- `hort::string`: thin std::string wrapper
- `hort::vector`: thin std::vector wrapper

### Infrastructure

- `hort::Args`: argument parsing
- `hort::Index`: MongoDB interface
- `hort::Interface`: user defined API interfaces
- `hort::Registry`: handles the entire infrastructure
- `hort::Session`: HTTP session management

## Plans for the future

- Automated parallel pipelines
- Intelligent rate limiters
- Code reflection (?)
- Dockerize the entire infrastructure
