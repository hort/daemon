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

## Monitoring Services

- Logging: Loki ([mircodezorzi/loki-cpp](http://github.com/mircodezorzi/loki-cpp))
- Monitoring: Promotheus ([jupp0r/prometheus-cpp](https://github.com/jupp0r/prometheus-cpp))

## Current Code Base

### STL 

- `hort::filesystem`: filesystem operations
- `hort::http`: libcurl wrapper
- `hort::regexp`: RE2 wrapper
- `hort::archive`: libzip wrapper
- `hort::print`: fmt wrapper
- `hort::string`, `hort::vector`: thin wrappers around std classes

### Infrastructure

- `hort::Registry`
- `hort::Interface`
- `hort::Index`

## Plans for the future

- Automated parallel pipelines
- Intelligent rate limiters
