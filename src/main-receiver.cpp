#include <cstdio>
#include <openhd_global_constants.hpp>

int main() {
  std::printf("OpenHD Version: %s!\n", openhd::get_ohd_version_as_string().c_str());
  std::printf("Hello, World from receiver!\n");
}
