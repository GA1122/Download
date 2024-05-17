  void StartCapture(int client_id, const media::VideoCaptureParams& params) {
    const auto state_update_callback = base::Bind(
        &VideoCaptureImplTest::OnStateUpdate, base::Unretained(this));
    const auto frame_ready_callback =
        base::Bind(&VideoCaptureImplTest::OnFrameReady, base::Unretained(this));

    video_capture_impl_->StartCapture(client_id, params, state_update_callback,
                                      frame_ready_callback);
  }
