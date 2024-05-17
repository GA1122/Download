void ContextState::RestoreActiveTexture() const {
  api()->glActiveTextureFn(GL_TEXTURE0 + active_texture_unit);
}
