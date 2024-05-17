  virtual void SetUp() {
    gl_.reset(new ::testing::StrictMock< ::gfx::MockGLInterface>());
    ::gfx::MockGLInterface::SetGLInterface(gl_.get());
  }
