void Resource::WillAddClientOrObserver() {
  if (!HasClientsOrObservers()) {
    is_alive_ = true;
  }
}
