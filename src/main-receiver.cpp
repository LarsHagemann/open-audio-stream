#include <cstdio>
#include <openhd_global_constants.hpp>
#include <openhd_profile.h>
#include <ohd_interface.h>

int main() {
  const auto profile = DProfile::discover(false);
  write_profile_manifest(profile);
  
  auto ohdInterface = std::make_shared<OHDInterface>(profile);

  OHDInterface::generate_keys_from_pw_if_exists_and_delete();


}
