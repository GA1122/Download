  void OnDeviceStoppedInternal(const std::string& label,
                               const MediaStreamDevice& device) {
    if (IsVideoInputMediaType(device.type))
      EXPECT_TRUE(device.IsSameDevice(video_devices_[0]));
    if (IsAudioInputMediaType(device.type))
      EXPECT_TRUE(device.IsSameDevice(audio_devices_[0]));

    OnDeviceStopSuccess();
  }
