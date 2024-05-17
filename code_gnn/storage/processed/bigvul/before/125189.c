void RenderMessageFilter::OnAllocateSharedMemory(
    uint32 buffer_size,
    base::SharedMemoryHandle* handle) {
  ChildProcessHostImpl::AllocateSharedMemory(
      buffer_size, peer_handle(), handle);
}
