  void StartAndImmediateStopCapture() {
    InSequence s;
    base::RunLoop run_loop;

    media::VideoCaptureParams params;
    params.requested_format = media::VideoCaptureFormat(
        gfx::Size(352, 288), 30, media::PIXEL_FORMAT_I420);

    EXPECT_CALL(*this, OnStateChanged(media::mojom::VideoCaptureState::STARTED))
        .Times(AtMost(1));
    media::mojom::VideoCaptureObserverPtr observer;
    observer_binding_.Bind(mojo::MakeRequest(&observer));
    host_->Start(kDeviceId, opened_session_id_, params, std::move(observer));

    EXPECT_CALL(*this,
                OnStateChanged(media::mojom::VideoCaptureState::STOPPED));
    host_->Stop(kDeviceId);
    run_loop.RunUntilIdle();
  }
