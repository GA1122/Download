  virtual void TearDown() {
    ::gfx::MockGLInterface::SetGLInterface(NULL);
    gl_.reset();
  }
