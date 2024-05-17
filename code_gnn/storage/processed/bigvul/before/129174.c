void FramebufferManager::OnTextureRefDetached(TextureRef* texture) {
  for (TextureDetachObserverVector::iterator it =
           texture_detach_observers_.begin();
       it != texture_detach_observers_.end();
       ++it) {
    TextureDetachObserver* observer = *it;
    observer->OnTextureRefDetachedFromFramebuffer(texture);
  }
}
