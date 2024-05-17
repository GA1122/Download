SharedMemoryHandleProvider::SharedMemoryHandleProvider() {
#if DCHECK_IS_ON()
  map_ref_count_ = 0;
#endif
}
