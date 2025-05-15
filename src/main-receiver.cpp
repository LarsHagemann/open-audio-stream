#include <iostream>
#include <thread>
#include <chrono>

#include <csignal>

#include <openhd_global_constants.hpp>
#include <openhd_profile.h>
#include <ohd_interface.h>
#include <openhd_udp.h>

static auto quit = false;

int main() {
  const auto profile = DProfile::discover(false);
  write_profile_manifest(profile);
  
  auto ohdInterface = std::make_shared<OHDInterface>(profile);
  auto link_handle = ohdInterface->get_link_handle();
  if (!link_handle) {
    std::cerr << "Failed to get link handle" << std::endl;
    return -1;
  }

  OHDInterface::generate_keys_from_pw_if_exists_and_delete();

  link_handle->m_audio_data_rx_cb = [](const uint8_t* data, int data_len) {
    /* TODO: create audio source from incoming data */
  };

  signal(SIGTERM, [](int sig) {
    std::cerr << "Got SIGTERM, exiting\n";
    quit = true;
  });
  signal(SIGQUIT, [](int sig) {
    std::cerr << "Got SIGQUIT, exiting\n";
    quit = true;
  });
  signal(SIGINT, [](int sig) {
    std::cerr << "Got SIGINT, exiting\n";
    quit = true;
  });

  while (!quit) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cout << "Exiting main loop\n";
}
