  void RequestDeviceAuthorizationWithBadOrigin(const std::string& device_id) {
    int session_id = 0;
    host_->OnRequestDeviceAuthorization(kStreamId, kRenderFrameId, session_id,
                                        device_id,
                                        url::Origin(GURL(kBadSecurityOrigin)));
    SyncWithAudioThread();
  }
