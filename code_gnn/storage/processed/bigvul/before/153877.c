  GLfloat BackBufferAlphaClearColor() const {
    return offscreen_buffer_should_have_alpha_ ? 0.f : 1.f;
  }
