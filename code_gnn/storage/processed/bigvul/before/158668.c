void BackFramebuffer::Destroy() {
  if (id_ != 0) {
    ScopedGLErrorSuppressor suppressor("BackFramebuffer::Destroy",
                                       decoder_->GetErrorState());
    api()->glDeleteFramebuffersEXTFn(1, &id_);
    id_ = 0;
  }
}
