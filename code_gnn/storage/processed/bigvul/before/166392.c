size_t GLES2Util::GetGLTypeSizeForPathCoordType(uint32_t type) {
  switch (type) {
    case GL_BYTE:
      return sizeof(GLbyte);   
    case GL_UNSIGNED_BYTE:
      return sizeof(GLubyte);   
    case GL_SHORT:
      return sizeof(GLshort);   
    case GL_UNSIGNED_SHORT:
      return sizeof(GLushort);   
    case GL_FLOAT:
      return sizeof(GLfloat);   
    default:
      return 0;
  }
}
