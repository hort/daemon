# hort: The Data Scraping Framework on Steroids

hort aims to be a high performance data mining and data scraping framework.

## Used Services 
- Logging: Loki ([mircodezorzi/loki-cpp](http://github.com/mircodezorzi/loki-cpp))
- Monitoring: Promotheus
- Indexing: MongoDB

## Current Code Base

- Filesystem
- HTTP (libcurl wrapper)
- Regex (RE2 wrapper)
- Archive (libzip wrapper)

## Plans for the future

- Writing a solid STL aimed at scraping
	- Automated monitoring
	- High level data processing 
	- Intelligent rate limiters
- Developing a scripting language aimed at quick scraper development without **any** boilerplate code
