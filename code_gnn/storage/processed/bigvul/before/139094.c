  void Close() {
    host_->OnCloseStream(kStreamId);
    SyncWithAudioThread();
  }
