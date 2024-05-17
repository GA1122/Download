base::SharedMemoryHandle GamepadProvider::DuplicateSharedMemoryHandle() {
  return gamepad_shared_buffer_->shared_memory()->handle().Duplicate();
}
