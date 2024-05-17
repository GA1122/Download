void BackTexture::Copy() {
  DCHECK_NE(id(), 0u);
  ScopedGLErrorSuppressor suppressor("BackTexture::Copy",
                                     decoder_->error_state_.get());
  ScopedTextureBinder binder(&decoder_->state_, decoder_->error_state_.get(),
                             id(), Target());
  api()->glCopyTexSubImage2DFn(Target(),
                               0,   
                               0, 0, 0, 0, size_.width(), size_.height());
}
