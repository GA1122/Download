void BackTexture::Copy() {
  DCHECK_NE(id(), 0u);
  ScopedGLErrorSuppressor suppressor("BackTexture::Copy",
                                     decoder_->state_.GetErrorState());
  ScopedTextureBinder binder(&decoder_->state_, id(), Target());
  api()->glCopyTexSubImage2DFn(Target(),
                               0,   
                               0, 0, 0, 0, size_.width(), size_.height());
}
