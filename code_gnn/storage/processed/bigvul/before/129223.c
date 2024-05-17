void BackTexture::Copy(const gfx::Size& size, GLenum format) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor("BackTexture::Copy",
                                     state_->GetErrorState());
  ScopedTextureBinder binder(state_, id_, GL_TEXTURE_2D);
  glCopyTexImage2D(GL_TEXTURE_2D,
                   0,   
                   format,
                   0, 0,
                   size.width(),
                   size.height(),
                   0);   
}
