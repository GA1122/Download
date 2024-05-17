bool GLES2DecoderImpl::ValidateCompressedCopyTextureCHROMIUM(
    const char* function_name,
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

  bool valid_format =
      source_internal_format == GL_ATC_RGB_AMD ||
      source_internal_format == GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD ||
      source_internal_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ||
      source_internal_format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT ||
      source_internal_format == GL_ETC1_RGB8_OES;

  if (!valid_format) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "invalid internal format");
    return false;
  }

  return true;
}
