  void Play() {
    host_->OnPlayStream(kStreamId);
    SyncWithAudioThread();
  }
