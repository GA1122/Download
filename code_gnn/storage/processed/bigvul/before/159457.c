bool WebGLImageConversion::ComputeFormatAndTypeParameters(
    GLenum format,
    GLenum type,
    unsigned* components_per_pixel,
    unsigned* bytes_per_component) {
  switch (format) {
    case GL_ALPHA:
    case GL_LUMINANCE:
    case GL_RED:
    case GL_RED_INTEGER:
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_STENCIL:   
      *components_per_pixel = 1;
      break;
    case GL_LUMINANCE_ALPHA:
    case GL_RG:
    case GL_RG_INTEGER:
      *components_per_pixel = 2;
      break;
    case GL_RGB:
    case GL_RGB_INTEGER:
    case GL_SRGB_EXT:   
      *components_per_pixel = 3;
      break;
    case GL_RGBA:
    case GL_RGBA_INTEGER:
    case GL_BGRA_EXT:         
    case GL_SRGB_ALPHA_EXT:   
      *components_per_pixel = 4;
      break;
    default:
      return false;
  }
  switch (type) {
    case GL_BYTE:
      *bytes_per_component = sizeof(GLbyte);
      break;
    case GL_UNSIGNED_BYTE:
      *bytes_per_component = sizeof(GLubyte);
      break;
    case GL_SHORT:
      *bytes_per_component = sizeof(GLshort);
      break;
    case GL_UNSIGNED_SHORT:
      *bytes_per_component = sizeof(GLushort);
      break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
      *components_per_pixel = 1;
      *bytes_per_component = sizeof(GLushort);
      break;
    case GL_INT:
      *bytes_per_component = sizeof(GLint);
      break;
    case GL_UNSIGNED_INT:
      *bytes_per_component = sizeof(GLuint);
      break;
    case GL_UNSIGNED_INT_24_8_OES:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
      *components_per_pixel = 1;
      *bytes_per_component = sizeof(GLuint);
      break;
    case GL_FLOAT:   
      *bytes_per_component = sizeof(GLfloat);
      break;
    case GL_HALF_FLOAT:
    case GL_HALF_FLOAT_OES:   
      *bytes_per_component = sizeof(GLushort);
      break;
    default:
      return false;
  }
  return true;
}
