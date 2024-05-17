void Texture::Destroy() {
  if (id_ != 0) {
    ScopedGLErrorSuppressor suppressor(decoder_);
    glDeleteTextures(1, &id_);
    id_ = 0;
    memory_tracker_.UpdateMemRepresented(0);
    TRACE_BACKBUFFER_MEMORY_TOTAL(decoder_);
  }
}
