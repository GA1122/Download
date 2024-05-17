  ~ScopedPackStateRowLengthReset() {
    if (row_length_ != 0) {
      api_->glPixelStoreiFn(GL_PACK_ROW_LENGTH, row_length_);
    }
  }
