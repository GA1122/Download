  void PauseResumeCapture() {
    InSequence s;
    base::RunLoop run_loop;

    EXPECT_CALL(*this, OnStateChanged(media::mojom::VideoCaptureState::PAUSED));
    host_->Pause(kDeviceId);

    media::VideoCaptureParams params;
    params.requested_format = media::VideoCaptureFormat(
        gfx::Size(352, 288), 30, media::PIXEL_FORMAT_I420);

    EXPECT_CALL(*this,
                OnStateChanged(media::mojom::VideoCaptureState::RESUMED));
    host_->Resume(kDeviceId, opened_session_id_, params);
    run_loop.RunUntilIdle();
  }
