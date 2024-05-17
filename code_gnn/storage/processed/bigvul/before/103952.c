bool Texture::AllocateStorage(const gfx::Size& size, GLenum format) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor(decoder_);
  ScopedTexture2DBinder binder(decoder_, id_);

  glTexImage2D(GL_TEXTURE_2D,
               0,   
               format,
               size.width(),
               size.height(),
               0,   
               format,
               GL_UNSIGNED_BYTE,
               NULL);

  size_ = size;

  return glGetError() == GL_NO_ERROR;
}
