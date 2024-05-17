  AudioSourceProviderClientLockScope(HTMLMediaElement& element)
      : client_(element.AudioSourceNode()) {
    if (client_)
      client_->lock();
  }
