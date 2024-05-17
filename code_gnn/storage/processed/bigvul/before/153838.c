void GLES2Implementation::UnlockDiscardableTextureCHROMIUM(GLuint texture_id) {
  ClientDiscardableTextureManager* manager =
      share_group()->discardable_texture_manager();
  if (!manager->TextureIsValid(texture_id)) {
    SetGLError(GL_INVALID_VALUE, "glUnlockDiscardableTextureCHROMIUM",
               "Texture ID not initialized");
    return;
  }

  bool should_unbind_texture = false;
  manager->UnlockTexture(texture_id, &should_unbind_texture);
  if (should_unbind_texture)
    UnbindTexturesHelper(1, &texture_id);

  helper_->UnlockDiscardableTextureCHROMIUM(texture_id);
}
