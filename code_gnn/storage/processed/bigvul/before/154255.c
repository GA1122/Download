error::Error GLES2DecoderImpl::HandleLockDiscardableTextureCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::LockDiscardableTextureCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::LockDiscardableTextureCHROMIUM*>(
          cmd_data);
  GLuint texture_id = c.texture_id;
  if (!GetContextGroup()->discardable_manager()->LockTexture(
          texture_id, group_->texture_manager())) {
    base::debug::DumpWithoutCrashing();
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glLockDiscardableTextureCHROMIUM",
                       "Texture ID not initialized");
  }
  return error::kNoError;
}
