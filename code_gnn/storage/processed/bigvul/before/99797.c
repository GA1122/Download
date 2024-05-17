void WebPluginDelegateStub::CreateSharedBuffer(
    size_t size,
    base::SharedMemory* shared_buf,
    base::SharedMemoryHandle* remote_handle) {
  if (!shared_buf->Create(std::wstring(), false, false, size)) {
    NOTREACHED();
    return;
  }
  if (!shared_buf->Map(size)) {
    NOTREACHED();
    shared_buf->Close();
    return;
  }

#if defined(OS_WIN)
  BOOL result = DuplicateHandle(GetCurrentProcess(),
                                shared_buf->handle(),
                                channel_->renderer_handle(),
                                remote_handle, 0, FALSE,
                                DUPLICATE_SAME_ACCESS);
  DCHECK_NE(result, 0);

#else
  NOTIMPLEMENTED();
#endif
 }
