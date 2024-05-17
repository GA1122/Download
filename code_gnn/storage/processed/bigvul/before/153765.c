bool GLES2Implementation::LockDiscardableTextureCHROMIUM(GLuint texture_id) {
  ClientDiscardableTextureManager* manager =
      share_group()->discardable_texture_manager();
  if (!manager->TextureIsValid(texture_id)) {
    SetGLError(GL_INVALID_VALUE, "glLockDiscardableTextureCHROMIUM",
               "Texture ID not initialized");
    return false;
  }
  if (!manager->LockTexture(texture_id)) {
    DeleteTexturesHelper(1, &texture_id);
    return false;
  }
  helper_->LockDiscardableTextureCHROMIUM(texture_id);
  return true;
}
