GLES2DecoderPassthroughImpl::CreateAbstractTexture(GLenum target,
                                                   GLenum internal_format,
                                                   GLsizei width,
                                                   GLsizei height,
                                                   GLsizei depth,
                                                   GLint border,
                                                   GLenum format,
                                                   GLenum type) {
  DCHECK(target != GL_TEXTURE_CUBE_MAP);
  GLuint service_id = 0;
  api()->glGenTexturesFn(1, &service_id);
  scoped_refptr<TexturePassthrough> texture(
      new TexturePassthrough(service_id, target));

  std::unique_ptr<PassthroughAbstractTextureImpl> abstract_texture =
      std::make_unique<PassthroughAbstractTextureImpl>(texture, this);

  abstract_textures_.insert(abstract_texture.get());
  return abstract_texture;
}
