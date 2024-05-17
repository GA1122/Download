void SharedMemoryHandleProvider::OnHandleDestroyed() {
  base::AutoLock lock(mapping_lock_);
  DCHECK_GT(map_ref_count_, 0);
  --map_ref_count_;
}
