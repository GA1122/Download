error::Error GLES2DecoderPassthroughImpl::DoUnlockDiscardableTextureCHROMIUM(
    GLuint texture_id) {
  TexturePassthrough* texture_to_unbind = nullptr;
  if (!group_->passthrough_discardable_manager()->UnlockTexture(
          texture_id, group_.get(), &texture_to_unbind)) {
    InsertError(GL_INVALID_VALUE, "Texture ID not initialized");
    return error::kNoError;
  }

  if (texture_to_unbind != nullptr) {
    UpdateTextureBinding(texture_to_unbind->target(), texture_id, nullptr);
  }
  return error::kNoError;
}
