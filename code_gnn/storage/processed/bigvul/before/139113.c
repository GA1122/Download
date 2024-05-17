  void SimulateError() {
    EXPECT_EQ(1u, host_->delegates_.size())
        << "Calls Create() before calling this method";

    EXPECT_CALL(*host_.get(), WasNotifiedOfError(kStreamId));

    host_->OnStreamError(kStreamId);
    SyncWithAudioThread();

    EXPECT_EQ(0u, host_->delegates_.size());
  }
