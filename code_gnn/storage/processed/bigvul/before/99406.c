  void HandleMakeSharedMemorySegment(int fd, const Pickle& pickle, void* iter,
                                     std::vector<int>& fds) {
    uint32_t shm_size;
    if (!pickle.ReadUInt32(&iter, &shm_size))
      return;
    int shm_fd = -1;
    base::SharedMemory shm;
    if (shm.Create(L"", false, false, shm_size))
      shm_fd = shm.handle().fd;
    Pickle reply;
    SendRendererReply(fds, reply, shm_fd);
  }
