#include "hort/registry.hpp"

int main(int argc, char* argv[]) {

  hort::Registry::instance()->parse(argc, argv);

  return 0;
}
