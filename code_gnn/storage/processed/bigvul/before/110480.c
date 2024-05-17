void GLES2DecoderImpl::DoGenerateMipmap(GLenum target) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info ||
      !texture_manager()->CanGenerateMipmaps(info)) {
    SetGLError(GL_INVALID_OPERATION,
               "glGenerateMipmaps", "Can not generate mips");
    return;
  }

  if (!texture_manager()->ClearTextureLevel(this, info, target, 0)) {
    SetGLError(GL_OUT_OF_MEMORY, "glGenerateMipmaps", "dimensions too big");
    return;
  }

  CopyRealGLErrorsToWrapper();
  if (!feature_info_->feature_flags().disable_workarounds) {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  }
  glGenerateMipmapEXT(target);
  if (!feature_info_->feature_flags().disable_workarounds) {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, info->min_filter());
  }
  GLenum error = PeekGLError();
  if (error == GL_NO_ERROR) {
    texture_manager()->MarkMipmapsGenerated(info);
  }
}
