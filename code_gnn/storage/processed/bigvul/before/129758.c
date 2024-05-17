base::SharedMemory* ChildThread::AllocateSharedMemory(size_t buf_size) {
  return AllocateSharedMemory(buf_size, this);
}
