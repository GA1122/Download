void GLES2Implementation::CompleteLockDiscardableTexureOnContextThread(
    uint32_t texture_id) {
  helper_->LockDiscardableTextureCHROMIUM(texture_id);
}
