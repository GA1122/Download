void SandboxIPCHandler::HandleMakeSharedMemorySegment(
    int fd,
    base::PickleIterator iter,
    const std::vector<base::ScopedFD>& fds) {
  base::SharedMemoryCreateOptions options;
  uint32_t size;
  if (!iter.ReadUInt32(&size))
    return;
  options.size = size;
  if (!iter.ReadBool(&options.executable))
    return;
  int shm_fd = -1;
  base::SharedMemory shm;
  if (shm.Create(options))
    shm_fd = shm.handle().GetHandle();
  base::Pickle reply;
  SendRendererReply(fds, reply, shm_fd);
}
