#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

#include <csignal>

#include <openhd_global_constants.hpp>
#include <openhd_profile.h>
#include <ohd_interface.h>
#include <openhd_config.h>
#include <gstaudiostream.h>

static auto quit = false;

/*

CONFIG FILE FOR OPEN AUDIO STREAM:
{
  "inputs": [
    {
      interface: {
        "config": "/boot/openhd/hardware_0.config"
      },
      device: "hw:2,0"
    },
    {
      interface: {
        "config": "/boot/openhd/hardware_1.config"
      },
      device: "hw:3,0"
    }
  ]
}

*/

struct InterfaceConfig {
  std::string config;
};

struct AudioConfig {
  InterfaceConfig interface;
  std::string device;
};

int main() {
  const auto profile = DProfile::discover(true);
  write_profile_manifest(profile);

  // TODO: Read from file
  std::vector<AudioConfig> config = {
    {
      .interface = {
        .config = "/boot/openhd/hardware_1.config"
      },
      .device = "audiotestsrc", //"hw:3,0"
    }
  };

  std::vector<std::unique_ptr<OHDInterface>> interfaces;
  std::vector<std::unique_ptr<GstAudioStream>> streams;

  for (const auto& device : config) {
    openhd::set_config_file(device.interface.config);
    interfaces.emplace_back(std::make_unique<OHDInterface>(profile));
    auto link_handle = interfaces.back()->get_link_handle();
    
    if (!link_handle) {
      std::cerr << "Failed to get link handle for device " << device.device << '\n';
      return -1;
    }

    // Bit hacky, but this is how we can set the audio source
    // for the transmitter. Alternatively, we could have out
    // own GstAudioStream implementation.
    {
      std::ofstream audioFile("/boot/openhd/audio_source.txt");
      audioFile << device.device;
      audioFile.close();
    }

    streams.emplace_back(std::make_unique<GstAudioStream>());
    streams.back()->set_link_cb(
      [&link_handle, &device](const openhd::AudioPacket& audioPacket) {
        link_handle->transmit_audio_data(audioPacket);
      }
    );
    streams.back()->start_looping();
  }

  // OHDInterface::generate_keys_from_pw_if_exists_and_delete();

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

  for (auto& stream : streams) {
    stream->stop_looping();
  }

  std::cout << "Exiting main loop\n";
}
