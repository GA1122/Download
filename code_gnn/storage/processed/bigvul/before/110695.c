TextureManager::TextureInfo::~TextureInfo() {
  if (manager_) {
    if (owned_ && manager_->have_context_) {
      GLuint id = service_id();
      glDeleteTextures(1, &id);
    }
    MarkAsDeleted();
    manager_->StopTracking(this);
    manager_ = NULL;
  }
}
