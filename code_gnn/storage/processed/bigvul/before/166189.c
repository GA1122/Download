  void OpenVideoDeviceAndWaitForResult(int page_request_id,
                                       const std::string& device_id) {
    EXPECT_CALL(*host_, OnDeviceOpenSuccess());
    base::RunLoop run_loop;
    host_->OnOpenDevice(page_request_id, device_id, MEDIA_DEVICE_VIDEO_CAPTURE,
                        run_loop.QuitClosure());
    run_loop.Run();
    EXPECT_FALSE(DoesContainRawIds(host_->video_devices_));
    EXPECT_TRUE(DoesEveryDeviceMapToRawId(host_->video_devices_, origin_));
  }
