void Resource::DidRemoveClientOrObserver() {
  if (!HasClientsOrObservers() && is_alive_) {
    is_alive_ = false;
    AllClientsAndObserversRemoved();

    if (HasCacheControlNoStoreHeader() && Url().ProtocolIs("https") &&
        IsMainThread())
      GetMemoryCache()->Remove(this);
  }
}
