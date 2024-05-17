void BackendImpl::BufferDeleted(int size) {
  buffer_bytes_ -= size;
  DCHECK_GE(size, 0);
}
