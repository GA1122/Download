  bool DoesContainRawIds(const MediaStreamDevices& devices) {
    for (size_t i = 0; i < devices.size(); ++i) {
      if (devices[i].id != media::AudioDeviceDescription::kDefaultDeviceId &&
          devices[i].id !=
              media::AudioDeviceDescription::kCommunicationsDeviceId) {
        for (const auto& audio_device : audio_device_descriptions_) {
          if (audio_device.unique_id == devices[i].id)
            return true;
        }
      }
      for (const std::string& device_id : stub_video_device_ids_) {
        if (device_id == devices[i].id)
          return true;
      }
    }
    return false;
  }
