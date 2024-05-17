const uint8_t* SharedMemoryHandleProvider::Handle::const_data() const {
  return static_cast<const uint8_t*>(owner_->shared_memory_->memory());
}
