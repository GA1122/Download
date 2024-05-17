  void OnNotifyDeviceAuthorized(int stream_id,
                                media::OutputDeviceStatus device_status,
                                const media::AudioParameters& output_params,
                                const std::string& matched_device_id) {
    EXPECT_TRUE(IsValidDeviceId(matched_device_id));

    OnDeviceAuthorized(stream_id, device_status, output_params,
                       matched_device_id);
    auth_run_loop_->Quit();
  }
