   void DeleteTexture() {
     if (texture_id_) {
       host_context_->deleteTexture(texture_id_);
      texture_id_ = 0;
    }
  }
