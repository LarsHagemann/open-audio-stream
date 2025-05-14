#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

#include <openhd_global_constants.hpp>
#include <openhd_profile.h>
#include <ohd_interface.h>
#include <gstaudiostream.h>

static auto quit = false;

int main() {
  const auto profile = DProfile::discover(true);
  write_profile_manifest(profile);
  
  auto ohdInterface = std::make_shared<OHDInterface>(profile);
  auto link_handle = ohdInterface->get_link_handle();
  if (!link_handle) {
    std::cerr << "Failed to get link handle" << std::endl;
    return -1;
  }

  OHDInterface::generate_keys_from_pw_if_exists_and_delete();

  auto gstAudioStream = std::make_shared<GstAudioStream>();
  gstAudioStream->set_link_cb(
      [&link_handle](const openhd::AudioPacket& audioPacket) {
        std::cout << "Got audio packet of size: " << audioPacket.data->size()
                  << '\n';
        link_handle->transmit_audio_data(audioPacket);
      });
  gstAudioStream->start_looping();

  signal(SIGTERM, [](int sig) {
    std::cerr << "Got SIGTERM, exiting\n";
    quit = true;
  });
  signal(SIGQUIT, [](int sig) {
    std::cerr << "Got SIGQUIT, exiting\n";
    quit = true;
  });


  while (!quit) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  gstAudioStream->stop_looping();
  gstAudioStream.reset();
  ohdInterface.reset();

  std::cout << "Exiting main loop\n";
}
