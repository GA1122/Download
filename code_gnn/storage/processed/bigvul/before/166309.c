  void StopCapture() {
    base::RunLoop run_loop;

    EXPECT_CALL(*this, OnStateChanged(media::mojom::VideoCaptureState::STOPPED))
        .WillOnce(ExitMessageLoop(task_runner_, run_loop.QuitClosure()));
    host_->Stop(kDeviceId);

    run_loop.Run();

    EXPECT_TRUE(host_->controllers_.empty());
  }
