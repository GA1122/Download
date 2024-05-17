  void SimulateError() {
    EXPECT_CALL(*this, OnStateChanged(media::mojom::VideoCaptureState::FAILED));
    VideoCaptureControllerID id(kDeviceId);
    host_->OnError(id,
                   media::VideoCaptureError::kIntentionalErrorRaisedByUnitTest);
    base::RunLoop().RunUntilIdle();
  }
