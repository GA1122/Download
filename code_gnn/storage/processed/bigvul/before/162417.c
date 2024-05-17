MojoResult Core::PassSharedMemoryHandle(
    MojoHandle mojo_handle,
    base::SharedMemoryHandle* shared_memory_handle,
    size_t* num_bytes,
    bool* read_only) {
  if (!shared_memory_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;

  scoped_refptr<Dispatcher> dispatcher;
  MojoResult result = MOJO_RESULT_OK;
  {
    base::AutoLock lock(handles_->GetLock());
    dispatcher = handles_->GetDispatcher(mojo_handle);
    if (!dispatcher || dispatcher->GetType() != Dispatcher::Type::SHARED_BUFFER)
      return MOJO_RESULT_INVALID_ARGUMENT;

    result = handles_->GetAndRemoveDispatcher(mojo_handle, &dispatcher);
    if (result != MOJO_RESULT_OK)
      return result;
  }

  SharedBufferDispatcher* shm_dispatcher =
      static_cast<SharedBufferDispatcher*>(dispatcher.get());
  scoped_refptr<PlatformSharedBuffer> platform_shared_buffer =
      shm_dispatcher->PassPlatformSharedBuffer();

  if (!platform_shared_buffer)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (num_bytes)
    *num_bytes = platform_shared_buffer->GetNumBytes();
  if (read_only)
    *read_only = platform_shared_buffer->IsReadOnly();
  *shared_memory_handle = platform_shared_buffer->DuplicateSharedMemoryHandle();

  shm_dispatcher->Close();
  return result;
}
