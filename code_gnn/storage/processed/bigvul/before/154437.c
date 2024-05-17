void PassthroughResources::DestroyPendingTextures(bool has_context) {
  if (!has_context) {
    for (scoped_refptr<TexturePassthrough> iter :
         textures_pending_destruction) {
      iter->MarkContextLost();
    }
  }
  textures_pending_destruction.clear();
}
