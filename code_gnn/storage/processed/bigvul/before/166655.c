bool WebGLRenderingContextBase::ValidateTexFuncDimensions(
    const char* function_name,
    TexImageFunctionType function_type,
    GLenum target,
    GLint level,
    GLsizei width,
    GLsizei height,
    GLsizei depth) {
  if (width < 0 || height < 0 || depth < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "width, height or depth < 0");
    return false;
  }

  switch (target) {
    case GL_TEXTURE_2D:
      if (width > (max_texture_size_ >> level) ||
          height > (max_texture_size_ >> level)) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "width or height out of range");
        return false;
      }
      break;
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      if (function_type != kTexSubImage && width != height) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "width != height for cube map");
        return false;
      }
      if (width > (max_cube_map_texture_size_ >> level)) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "width or height out of range for cube map");
        return false;
      }
      break;
    case GL_TEXTURE_3D:
      if (IsWebGL2OrHigher()) {
        if (width > (max3d_texture_size_ >> level) ||
            height > (max3d_texture_size_ >> level) ||
            depth > (max3d_texture_size_ >> level)) {
          SynthesizeGLError(GL_INVALID_VALUE, function_name,
                            "width, height or depth out of range");
          return false;
        }
        break;
      }
      FALLTHROUGH;
    case GL_TEXTURE_2D_ARRAY:
      if (IsWebGL2OrHigher()) {
        if (width > (max_texture_size_ >> level) ||
            height > (max_texture_size_ >> level) ||
            depth > max_array_texture_layers_) {
          SynthesizeGLError(GL_INVALID_VALUE, function_name,
                            "width, height or depth out of range");
          return false;
        }
        break;
      }
      FALLTHROUGH;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid target");
      return false;
  }
  return true;
}
