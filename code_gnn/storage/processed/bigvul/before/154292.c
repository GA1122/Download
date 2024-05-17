error::Error GLES2DecoderImpl::HandleUnlockDiscardableTextureCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::UnlockDiscardableTextureCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::UnlockDiscardableTextureCHROMIUM*>(
          cmd_data);
  GLuint texture_id = c.texture_id;
  ServiceDiscardableManager* discardable_manager =
      GetContextGroup()->discardable_manager();
  TextureRef* texture_to_unbind;
  if (!discardable_manager->UnlockTexture(texture_id, group_->texture_manager(),
                                          &texture_to_unbind)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glUnlockDiscardableTextureCHROMIUM",
                       "Texture ID not initialized");
  }
  if (texture_to_unbind)
    UnbindTexture(texture_to_unbind, SupportsSeparateFramebufferBinds());

  return error::kNoError;
}
