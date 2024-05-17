error::Error GLES2DecoderPassthroughImpl::BindTexImage2DCHROMIUMImpl(
    GLenum target,
    GLenum internalformat,
    GLint imageId) {
  if (target != GL_TEXTURE_2D) {
    InsertError(GL_INVALID_ENUM, "Invalid target");
    return error::kNoError;
  }

  gl::GLImage* image = group_->image_manager()->LookupImage(imageId);
  if (image == nullptr) {
    InsertError(GL_INVALID_OPERATION, "No image found with the given ID");
    return error::kNoError;
  }

  const BoundTexture& bound_texture =
      bound_textures_[static_cast<size_t>(TextureTarget::k2D)]
                     [active_texture_unit_];
  if (bound_texture.texture == nullptr) {
    InsertError(GL_INVALID_OPERATION, "No texture bound");
    return error::kNoError;
  }

  if (image->ShouldBindOrCopy() == gl::GLImage::BIND) {
    if (internalformat)
      image->BindTexImageWithInternalformat(target, internalformat);
    else
      image->BindTexImage(target);
  } else {
    image->CopyTexImage(target);
  }

  UpdateTextureSizeFromTarget(target);

  DCHECK(bound_texture.texture != nullptr);
  bound_texture.texture->SetLevelImage(target, 0, image);

  RemovePendingBindingTexture(target, active_texture_unit_);

  return error::kNoError;
}
