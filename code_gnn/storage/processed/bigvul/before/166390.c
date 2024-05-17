size_t GLES2Util::GetGLTypeSizeForBuffers(uint32_t type) {
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
    case GL_FLOAT:
      return sizeof(GLfloat);   
    case GL_FIXED:
      return sizeof(GLfixed);   
    case GL_HALF_FLOAT:
      return sizeof(GLushort);   
    case GL_INT_2_10_10_10_REV:
      return sizeof(GLint);   
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      return sizeof(GLuint);   
    default:
      return 0;
  }
}
