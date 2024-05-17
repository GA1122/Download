MojoResult DataPipeConsumerDispatcher::CloseNoLock() {
  lock_.AssertAcquired();
  if (is_closed_ || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;
  is_closed_ = true;
  ring_buffer_mapping_ = base::WritableSharedMemoryMapping();
  shared_ring_buffer_ = base::UnsafeSharedMemoryRegion();

  watchers_.NotifyClosed();
  if (!transferred_) {
    base::AutoUnlock unlock(lock_);
    node_controller_->ClosePort(control_port_);
  }

  return MOJO_RESULT_OK;
}
