void GLManager::SetSurface(gl::GLSurface* surface) {
  decoder_->SetSurface(surface);
  MakeCurrent();
}
