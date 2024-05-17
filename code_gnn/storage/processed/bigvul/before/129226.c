void BackFramebuffer::Create() {
  ScopedGLErrorSuppressor suppressor("BackFramebuffer::Create",
                                     decoder_->GetErrorState());
  Destroy();
  glGenFramebuffersEXT(1, &id_);
}
