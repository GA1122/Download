void GLES2Implementation::RemoveMappedBufferRangeByTarget(GLenum target) {
  GLuint buffer = GetBoundBufferHelper(target);
  RemoveMappedBufferRangeById(buffer);
}
