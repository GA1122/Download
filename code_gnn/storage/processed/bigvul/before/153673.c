GLuint GLES2Implementation::GetBoundBufferHelper(GLenum target) {
  GLenum binding = GLES2Util::MapBufferTargetToBindingEnum(target);
  GLint id = 0;
  bool cached = GetHelper(binding, &id);
  DCHECK(cached);
  return static_cast<GLuint>(id);
}
