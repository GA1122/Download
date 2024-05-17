bool GLES2DecoderImpl::NeedsCopyTextureImageWorkaround(
    GLenum internal_format,
    int32_t channels_exist,
    GLuint* source_texture_service_id,
    GLenum* source_texture_target) {
  if (!workarounds().use_intermediary_for_copy_texture_image)
    return false;

  if (internal_format == GL_RGB || internal_format == GL_RGBA)
    return false;

  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (!framebuffer)
    return false;

  const Framebuffer::Attachment* attachment =
      framebuffer->GetReadBufferAttachment();
  if (!attachment)
    return false;

  if (!attachment->IsTextureAttachment())
    return false;

  TextureRef* texture =
      texture_manager()->GetTexture(attachment->object_name());
  if (!texture->texture()->HasImages())
    return false;

  if (channels_exist != GLES2Util::kRGBA && channels_exist != GLES2Util::kRGB)
    return false;

  *source_texture_target = texture->texture()->target();
  *source_texture_service_id = texture->service_id();
  return true;
}
