  bool DoesEveryDeviceMapToRawId(const MediaStreamDevices& devices,
                                 const url::Origin& origin) {
    for (size_t i = 0; i < devices.size(); ++i) {
      bool found_match = false;
      media::AudioDeviceDescriptions::const_iterator audio_it =
          audio_device_descriptions_.begin();
      for (; audio_it != audio_device_descriptions_.end(); ++audio_it) {
        if (DoesMediaDeviceIDMatchHMAC(browser_context_->GetMediaDeviceIDSalt(),
                                       origin, devices[i].id,
                                       audio_it->unique_id)) {
          EXPECT_FALSE(found_match);
          found_match = true;
        }
      }
      for (const std::string& device_id : stub_video_device_ids_) {
        if (DoesMediaDeviceIDMatchHMAC(browser_context_->GetMediaDeviceIDSalt(),
                                       origin, devices[i].id, device_id)) {
          EXPECT_FALSE(found_match);
          found_match = true;
        }
      }
      if (!found_match)
        return false;
    }
    return true;
  }
