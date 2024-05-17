  void SetVolume(double volume) {
    host_->OnSetVolume(kStreamId, volume);
    SyncWithAudioThread();
  }
