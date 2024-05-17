bool GLES2Implementation::ThreadsafeDiscardableTextureIsDeletedForTracing(
    uint32_t texture_id) {
  ClientDiscardableTextureManager* manager =
      share_group()->discardable_texture_manager();
  return manager->TextureIsDeletedForTracing(texture_id);
}
