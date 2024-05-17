error::Error GLES2DecoderPassthroughImpl::DoLockDiscardableTextureCHROMIUM(
    GLuint texture_id) {
  if (!group_->passthrough_discardable_manager()->LockTexture(texture_id,
                                                              group_.get())) {
    InsertError(GL_INVALID_VALUE, "Texture ID not initialized");
    return error::kNoError;
  }

  return error::kNoError;
}
