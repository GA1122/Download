void BackFramebuffer::Create() {
  ScopedGLErrorSuppressor suppressor("BackFramebuffer::Create",
                                     decoder_->GetErrorState());
  Destroy();
  api()->glGenFramebuffersEXTFn(1, &id_);
}
