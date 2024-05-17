void PepperPlatformAudioInput::OnStreamCreated(
    base::SharedMemoryHandle handle,
    base::SyncSocket::Handle socket_handle,
    int length,
    int total_segments) {
#if defined(OS_WIN)
  DCHECK(handle);
  DCHECK(socket_handle);
#else
  DCHECK_NE(-1, handle.fd);
  DCHECK_NE(-1, socket_handle);
#endif
  DCHECK(length);
  DCHECK_EQ(1, total_segments);

  if (base::MessageLoopProxy::current().get() !=
      main_message_loop_proxy_.get()) {
    main_message_loop_proxy_->PostTask(
        FROM_HERE,
        base::Bind(&PepperPlatformAudioInput::OnStreamCreated,
                   this,
                   handle,
                   socket_handle,
                   length,
                   total_segments));
  } else {
    if (client_) {
      client_->StreamCreated(handle, length, socket_handle);
    } else {
      base::SyncSocket temp_socket(socket_handle);
      base::SharedMemory temp_shared_memory(handle, false);
    }
  }
}
