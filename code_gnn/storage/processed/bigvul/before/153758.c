bool IsReadbackUsage(GLenum usage) {
  return usage == GL_STREAM_READ || usage == GL_DYNAMIC_READ ||
         usage == GL_STATIC_READ;
}
