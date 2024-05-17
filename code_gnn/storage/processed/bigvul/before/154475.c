bool PassthroughResources::HasTexturesPendingDestruction() const {
  return !textures_pending_destruction.empty();
}
