uint8_t* SharedMemoryHandleProvider::Handle::data() const {
  return static_cast<uint8_t*>(owner_->shared_memory_->memory());
}
