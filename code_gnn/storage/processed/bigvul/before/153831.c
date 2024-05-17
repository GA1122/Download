bool GLES2Implementation::ThreadSafeShallowLockDiscardableTexture(
    uint32_t texture_id) {
  ClientDiscardableTextureManager* manager =
      share_group()->discardable_texture_manager();
  return manager->TextureIsValid(texture_id) &&
         manager->LockTexture(texture_id);
}
