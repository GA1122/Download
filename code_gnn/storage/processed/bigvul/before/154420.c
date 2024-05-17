void GLES2DecoderPassthroughImpl::BindOnePendingImage(
    GLenum target,
    TexturePassthrough* texture) {
  if (!texture || !texture->is_bind_pending())
    return;

  gl::GLImage* image = texture->GetLevelImage(target, 0);

  if (!image)
    return;

  if (image->ShouldBindOrCopy() == gl::GLImage::BIND)
    image->BindTexImage(target);
  else
    image->CopyTexImage(target);

  texture->set_is_bind_pending(false);

  UpdateTextureSizeFromTexturePassthrough(texture, 0);
}
