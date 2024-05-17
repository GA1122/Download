bool DataPipeConsumerDispatcher::InitializeNoLock() {
  lock_.AssertAcquired();
  if (!shared_ring_buffer_.IsValid())
    return false;

  DCHECK(!ring_buffer_mapping_.IsValid());
  ring_buffer_mapping_ = shared_ring_buffer_.Map();
  if (!ring_buffer_mapping_.IsValid()) {
    DLOG(ERROR) << "Failed to map shared buffer.";
    shared_ring_buffer_ = base::UnsafeSharedMemoryRegion();
    return false;
  }

  base::AutoUnlock unlock(lock_);
  node_controller_->SetPortObserver(
      control_port_, base::MakeRefCounted<PortObserverThunk>(this));

  return true;
}
