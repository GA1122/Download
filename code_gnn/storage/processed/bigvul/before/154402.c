  GLuint id() const {
    return texture_ref_ ? texture_ref_->service_id() : 0;
  }
