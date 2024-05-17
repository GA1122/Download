size_t GLES2Util::GetGLTypeSizeForGLPathNameType(uint32_t type) {
  switch (type) {
    case GL_BYTE:
      return sizeof(GLbyte);   
    case GL_UNSIGNED_BYTE:
      return sizeof(GLubyte);   
    case GL_SHORT:
      return sizeof(GLshort);   
    case GL_UNSIGNED_SHORT:
      return sizeof(GLushort);   
    case GL_INT:
      return sizeof(GLint);   
    case GL_UNSIGNED_INT:
      return sizeof(GLuint);   
    default:
      return 0;
  }
}
