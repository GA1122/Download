bool GLES2DecoderImpl::ValidateCopyTextureCHROMIUMTextures(
    const char* function_name,
    GLenum dest_target,
    TextureRef* source_texture_ref,
    TextureRef* dest_texture_ref) {
  if (!source_texture_ref || !dest_texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "unknown texture id");
    return false;
  }

  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  if (source_texture == dest_texture) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "source and destination textures are the same");
    return false;
  }

  if (dest_texture->target() !=
      GLES2Util::GLFaceTargetToTextureTarget(dest_target)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "target should be aligned with dest target");
    return false;
  }
  switch (dest_texture->target()) {
    case GL_TEXTURE_2D:
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_RECTANGLE_ARB:
      break;
    default:
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "invalid dest texture target binding");
    return false;
  }

  switch (source_texture->target()) {
    case GL_TEXTURE_2D:
    case GL_TEXTURE_RECTANGLE_ARB:
    case GL_TEXTURE_EXTERNAL_OES:
      break;
    default:
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "invalid source texture target binding");
    return false;
  }
  return true;
}
