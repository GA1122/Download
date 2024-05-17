base::SharedMemory* ChildThread::AllocateSharedMemory(
    size_t buf_size,
    IPC::Sender* sender) {
  scoped_ptr<base::SharedMemory> shared_buf;
#if defined(OS_WIN)
  shared_buf.reset(new base::SharedMemory);
  if (!shared_buf->CreateAndMapAnonymous(buf_size)) {
    NOTREACHED();
    return NULL;
  }
#else
  base::SharedMemoryHandle shared_mem_handle;
  if (sender->Send(new ChildProcessHostMsg_SyncAllocateSharedMemory(
                           buf_size, &shared_mem_handle))) {
    if (base::SharedMemory::IsHandleValid(shared_mem_handle)) {
      shared_buf.reset(new base::SharedMemory(shared_mem_handle, false));
      if (!shared_buf->Map(buf_size)) {
        NOTREACHED() << "Map failed";
        return NULL;
      }
    } else {
      NOTREACHED() << "Browser failed to allocate shared memory";
      return NULL;
    }
  } else {
    NOTREACHED() << "Browser allocation request message failed";
    return NULL;
  }
#endif
  return shared_buf.release();
}
