void GLES2DecoderImpl::DoUnpremultiplyAndDitherCopyCHROMIUM(GLuint source_id,
                                                            GLuint dest_id,
                                                            GLint x,
                                                            GLint y,
                                                            GLsizei width,
                                                            GLsizei height) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoUnpremultiplyAndDitherCopyCHROMIUM");
  static const char kFunctionName[] = "glUnpremultiplyAndDitherCopyCHROMIUM";

  const GLint kLevel = 0;

  TextureRef* source_texture_ref = GetTexture(source_id);
  TextureRef* dest_texture_ref = GetTexture(dest_id);
  if (!source_texture_ref || !dest_texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName, "unknown texture id");
    return;
  }

  Texture* source_texture = source_texture_ref->texture();
  GLenum source_target = source_texture->target();
  Texture* dest_texture = dest_texture_ref->texture();
  GLenum dest_target = dest_texture->target();
  if ((source_target != GL_TEXTURE_2D &&
       source_target != GL_TEXTURE_RECTANGLE_ARB &&
       source_target != GL_TEXTURE_EXTERNAL_OES) ||
      (dest_target != GL_TEXTURE_2D &&
       dest_target != GL_TEXTURE_RECTANGLE_ARB)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, kFunctionName,
                       "invalid texture target");
    return;
  }

  GLenum source_type = 0;
  GLenum source_internal_format = 0;
  source_texture->GetLevelType(source_target, kLevel, &source_type,
                               &source_internal_format);

  GLenum dest_type = 0;
  GLenum dest_internal_format = 0;
  dest_texture->GetLevelType(dest_target, kLevel, &dest_type,
                             &dest_internal_format);
  GLenum format =
      TextureManager::ExtractFormatFromStorageFormat(dest_internal_format);

  if (format != GL_BGRA && format != GL_RGBA) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName, "invalid format");
    return;
  }

  if (dest_type != GL_UNSIGNED_SHORT_4_4_4_4) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, kFunctionName,
                       "invalid destination type");
    return;
  }

  CopySubTextureHelper(
      kFunctionName, source_id, kLevel, dest_target, dest_id, kLevel, x, y, x,
      y, width, height, GL_FALSE  ,
      GL_FALSE  ,
      GL_TRUE  , GL_TRUE  );
}
