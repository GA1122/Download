  virtual ~FramebufferManagerTest() {
    manager_.Destroy(false);
    texture_manager_.Destroy(false);
    renderbuffer_manager_.Destroy(false);
  }
