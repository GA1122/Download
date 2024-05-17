  void Pause() {
    host_->OnPauseStream(kStreamId);
    SyncWithAudioThread();
  }
