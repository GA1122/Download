  ~ScopedPixelUnpackBufferOverride() {
    if (orig_binding_ != -1) {
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, static_cast<GLuint>(orig_binding_));
    }
  }
