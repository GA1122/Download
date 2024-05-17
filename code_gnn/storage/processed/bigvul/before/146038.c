bool WebGL2RenderingContextBase::ValidateTexStorage(
    const char* function_name,
    GLenum target,
    GLsizei levels,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    TexStorageType function_type) {
  if (function_type == kTexStorageType2D) {
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_CUBE_MAP) {
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid 2D target");
      return false;
    }
  } else {
    if (target != GL_TEXTURE_3D && target != GL_TEXTURE_2D_ARRAY) {
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid 3D target");
      return false;
    }
  }

  if (function_type == kTexStorageType3D && target != GL_TEXTURE_2D_ARRAY &&
      compressed_texture_formats_etc2eac_.find(internalformat) !=
          compressed_texture_formats_etc2eac_.end()) {
    SynthesizeGLError(
        GL_INVALID_OPERATION, function_name,
        "target for ETC2/EAC internal formats must be TEXTURE_2D_ARRAY");
    return false;
  }

  if (supported_internal_formats_storage_.find(internalformat) ==
          supported_internal_formats_storage_.end() &&
      (function_type == kTexStorageType2D &&
       !compressed_texture_formats_.Contains(internalformat))) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid internalformat");
    return false;
  }

  if (width <= 0 || height <= 0 || depth <= 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "invalid dimensions");
    return false;
  }

  if (levels <= 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "invalid levels");
    return false;
  }

  if (target == GL_TEXTURE_3D) {
    if (levels > log2(std::max(std::max(width, height), depth)) + 1) {
      SynthesizeGLError(GL_INVALID_OPERATION, function_name, "to many levels");
      return false;
    }
  } else {
    if (levels > log2(std::max(width, height)) + 1) {
      SynthesizeGLError(GL_INVALID_OPERATION, function_name, "to many levels");
      return false;
    }
  }

  return true;
}
