SharedMemoryHandleProvider::Handle::~Handle() {
#if DCHECK_IS_ON()
  owner_->OnHandleDestroyed();
#endif
}
