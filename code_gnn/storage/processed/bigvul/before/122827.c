  virtual GLHelper* GetGLHelper() {
    if (!gl_helper_.get()) {
      CreateSharedContextLazy();
      WebKit::WebGraphicsContext3D* context_for_thread =
          CreateOffscreenContext();
      if (!context_for_thread)
        return NULL;
      gl_helper_.reset(new GLHelper(shared_context_.get(),
                                    context_for_thread));
    }
    return gl_helper_.get();
  }
