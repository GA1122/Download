  void GenerateStreamAndWaitForResult(int page_request_id,
                                      const StreamControls& controls) {
    base::RunLoop run_loop;
    int expected_audio_array_size =
        (controls.audio.requested && !audio_device_descriptions_.empty()) ? 1
                                                                          : 0;
    int expected_video_array_size =
        (controls.video.requested && !stub_video_device_ids_.empty()) ? 1 : 0;
    EXPECT_CALL(*host_, OnStreamGenerationSuccess(page_request_id,
                                                  expected_audio_array_size,
                                                  expected_video_array_size));
    host_->OnGenerateStream(page_request_id, controls, run_loop.QuitClosure());
    run_loop.Run();
    EXPECT_FALSE(DoesContainRawIds(host_->audio_devices_));
    EXPECT_FALSE(DoesContainRawIds(host_->video_devices_));
    EXPECT_TRUE(DoesEveryDeviceMapToRawId(host_->audio_devices_, origin_));
    EXPECT_TRUE(DoesEveryDeviceMapToRawId(host_->video_devices_, origin_));
  }
