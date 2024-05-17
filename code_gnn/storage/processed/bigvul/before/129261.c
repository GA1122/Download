void GLES2DecoderImpl::DoCompressedTexSubImage2D(
  GLenum target,
  GLint level,
  GLint xoffset,
  GLint yoffset,
  GLsizei width,
  GLsizei height,
  GLenum format,
  GLsizei image_size,
  const void * data) {
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCompressedTexSubImage2D", "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  GLenum type = 0;
  GLenum internal_format = 0;
  if (!texture->GetLevelType(target, level, &type, &internal_format)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCompressedTexSubImage2D", "level does not exist.");
    return;
  }
  if (internal_format != format) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glCompressedTexSubImage2D", "format does not match internal format.");
    return;
  }
  if (!texture->ValidForTexture(
      target, level, xoffset, yoffset, width, height, type)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glCompressedTexSubImage2D", "bad dimensions.");
    return;
  }

  if (!ValidateCompressedTexFuncData(
      "glCompressedTexSubImage2D", width, height, format, image_size) ||
      !ValidateCompressedTexSubDimensions(
      "glCompressedTexSubImage2D",
      target, level, xoffset, yoffset, width, height, format, texture)) {
    return;
  }


  glCompressedTexSubImage2D(
      target, level, xoffset, yoffset, width, height, format, image_size, data);
}
