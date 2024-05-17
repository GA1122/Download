  VideoCaptureImplTest()
      : video_capture_impl_(new VideoCaptureImpl(kSessionId)) {
    params_small_.requested_format = media::VideoCaptureFormat(
        gfx::Size(176, 144), 30, media::PIXEL_FORMAT_I420);
    params_large_.requested_format = media::VideoCaptureFormat(
        gfx::Size(320, 240), 30, media::PIXEL_FORMAT_I420);

    video_capture_impl_->SetVideoCaptureHostForTesting(
        &mock_video_capture_host_);

    ON_CALL(mock_video_capture_host_, DoStart(_, _, _))
        .WillByDefault(InvokeWithoutArgs([this]() {
          video_capture_impl_->OnStateChanged(
              mojom::VideoCaptureState::STARTED);
        }));
  }
