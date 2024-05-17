void GLES2Implementation::InitializeDiscardableTextureCHROMIUM(
    GLuint texture_id) {
  ClientDiscardableTextureManager* manager =
      share_group()->discardable_texture_manager();
  if (manager->TextureIsValid(texture_id)) {
    SetGLError(GL_INVALID_VALUE, "glInitializeDiscardableTextureCHROMIUM",
               "Texture ID already initialized");
    return;
  }
  ClientDiscardableHandle handle =
      manager->InitializeTexture(helper_->command_buffer(), texture_id);
  if (!handle.IsValid())
    return;

  helper_->InitializeDiscardableTextureCHROMIUM(texture_id, handle.shm_id(),
                                                handle.byte_offset());
}
