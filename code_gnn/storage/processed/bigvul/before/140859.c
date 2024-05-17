bool GLES2DecoderImpl::ValidateCopyTextureCHROMIUM(
    const char* function_name,
    GLenum target,
    TextureRef* source_texture_ref,
    TextureRef* dest_texture_ref,
    GLenum dest_internal_format) {
  if (!source_texture_ref || !dest_texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "unknown texture id");
    return false;
  }

  if (GL_TEXTURE_2D != target) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "invalid texture target");
    return false;
  }

  Texture* source_texture = source_texture_ref->texture();
  Texture* dest_texture = dest_texture_ref->texture();
  if (source_texture == dest_texture) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "source and destination textures are the same");
    return false;
  }

  if (dest_texture->target() != GL_TEXTURE_2D ||
      (source_texture->target() != GL_TEXTURE_2D &&
       source_texture->target() != GL_TEXTURE_RECTANGLE_ARB &&
       source_texture->target() != GL_TEXTURE_EXTERNAL_OES)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                       "invalid texture target binding");
    return false;
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(source_texture->target(), 0, &source_type,
                               &source_internal_format);

  bool valid_dest_format = dest_internal_format == GL_RGB ||
                           dest_internal_format == GL_RGBA ||
                           dest_internal_format == GL_BGRA_EXT;
  bool valid_source_format = source_internal_format == GL_ALPHA ||
                             source_internal_format == GL_RGB ||
                             source_internal_format == GL_RGBA ||
                             source_internal_format == GL_LUMINANCE ||
                             source_internal_format == GL_LUMINANCE_ALPHA ||
                             source_internal_format == GL_BGRA_EXT;
  if (!valid_source_format || !valid_dest_format) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "invalid internal format");
    return false;
  }
  return true;
}
