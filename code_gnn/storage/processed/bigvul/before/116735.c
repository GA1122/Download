base::SharedMemoryHandle MockRenderThread::HostAllocateSharedMemoryBuffer(
    uint32 buffer_size) {
  base::SharedMemory shared_buf;
  if (!shared_buf.CreateAndMapAnonymous(buffer_size)) {
    NOTREACHED() << "Cannot map shared memory buffer";
    return base::SharedMemory::NULLHandle();
  }
  base::SharedMemoryHandle handle;
  shared_buf.GiveToProcess(base::GetCurrentProcessHandle(), &handle);
  return handle;
}
