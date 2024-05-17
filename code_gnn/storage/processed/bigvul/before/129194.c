  void InitializeContext(const char* gl_version, const char* extensions) {
    gl_.reset(new ::testing::StrictMock< ::gfx::MockGLInterface>());
    ::gfx::MockGLInterface::SetGLInterface(gl_.get());
    TestHelper::SetupFeatureInfoInitExpectationsWithGLVersion(gl_.get(),
        extensions, "", gl_version);
    feature_info_->Initialize();
    manager_.CreateFramebuffer(kClient1Id, kService1Id);
    error_state_.reset(new ::testing::StrictMock<gles2::MockErrorState>());
    framebuffer_ = manager_.GetFramebuffer(kClient1Id);
    ASSERT_TRUE(framebuffer_ != NULL);
  }
