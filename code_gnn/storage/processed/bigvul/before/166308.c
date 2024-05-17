  void StartCapture() {
    base::RunLoop run_loop;
    media::VideoCaptureParams params;
    params.requested_format = media::VideoCaptureFormat(
        gfx::Size(352, 288), 30, media::PIXEL_FORMAT_I420);

    EXPECT_CALL(*this,
                OnStateChanged(media::mojom::VideoCaptureState::STARTED));
    EXPECT_CALL(*this, DoOnNewBuffer(_))
        .Times(AnyNumber())
        .WillRepeatedly(Return());
    EXPECT_CALL(*this, DoOnBufferReady(_))
        .Times(AnyNumber())
        .WillRepeatedly(ExitMessageLoop(task_runner_, run_loop.QuitClosure()));

    media::mojom::VideoCaptureObserverPtr observer;
    observer_binding_.Bind(mojo::MakeRequest(&observer));
    host_->Start(kDeviceId, opened_session_id_, params, std::move(observer));

    run_loop.Run();
  }
