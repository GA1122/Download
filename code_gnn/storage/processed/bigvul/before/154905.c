  ScopedPackStateRowLengthReset(gl::GLApi* api, bool enable) : api_(api) {
    if (!enable) {
      return;
    }

    api_->glGetIntegervFn(GL_PACK_ROW_LENGTH, &row_length_);
    api_->glPixelStoreiFn(GL_PACK_ROW_LENGTH, 0);
  }
