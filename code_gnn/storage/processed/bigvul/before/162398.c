MojoResult Core::CreateSharedBufferWrapper(
    base::SharedMemoryHandle shared_memory_handle,
    size_t num_bytes,
    bool read_only,
    MojoHandle* mojo_wrapper_handle) {
  DCHECK(num_bytes);
  scoped_refptr<PlatformSharedBuffer> platform_buffer =
      PlatformSharedBuffer::CreateFromSharedMemoryHandle(num_bytes, read_only,
                                                         shared_memory_handle);
  if (!platform_buffer)
    return MOJO_RESULT_UNKNOWN;

  scoped_refptr<SharedBufferDispatcher> dispatcher;
  MojoResult result = SharedBufferDispatcher::CreateFromPlatformSharedBuffer(
      platform_buffer, &dispatcher);
  if (result != MOJO_RESULT_OK)
    return result;
  MojoHandle h = AddDispatcher(dispatcher);
  if (h == MOJO_HANDLE_INVALID)
    return MOJO_RESULT_RESOURCE_EXHAUSTED;
  *mojo_wrapper_handle = h;
  return MOJO_RESULT_OK;
}
