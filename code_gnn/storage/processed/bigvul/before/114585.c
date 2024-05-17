base::SharedMemoryHandle RenderThreadImpl::HostAllocateSharedMemoryBuffer(
    uint32 buffer_size) {
  base::SharedMemoryHandle mem_handle;
  Send(new ChildProcessHostMsg_SyncAllocateSharedMemory(
                buffer_size, &mem_handle));
  return mem_handle;
}
